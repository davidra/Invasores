#include "pch/pch.h"

#include "game/game.h"
#include "game/aliensquadron.h"
#include "game/alien.h"
#include "game/shiprocket.h"
#include "DiceInvaders.h"

const cAlienSquadronDef s_defaultAlienSquadron;

cAlienSquadron::tCellIdx cAlienSquadron::INVALID_CELL_IDX = {UINT_MAX, UINT_MAX};

//----------------------------------------------------------------------------
void cAlienSquadron::update(float elapsed)
{
	spawnAliensIfNeeded();

	// Update orientation and position based on if we are going beyond the sides of the screen
	switch (m_orientation)
	{
	case O_LEFT_TO_RIGHT: 
	{
		if (m_area.m_max.x > cGame::get().getScreenWidth())
		{
			m_orientation = O_RIGHT_TO_LEFT;
			m_gridPos.y += def().vrtStep;
		}
	} break;
	case O_RIGHT_TO_LEFT:
	{
		if (m_area.m_min.x < 0.0f)
		{
			m_orientation = O_LEFT_TO_RIGHT;
			m_gridPos.y += def().vrtStep;
		}
	} break;
	}

	constexpr float tolerance = 0.1f;

	// Clamp delta to protect against huge elapsed time values (for example, when debugging)
	const float minDelta = (0.0f - tolerance)							- m_area.m_min.x;
	const float maxDelta = (cGame::get().getScreenWidth() + tolerance)	- m_area.m_max.x;
	const float speed = lerp(def().hrzSpeed, def().hrzMaxSpeed, 1.0f - (static_cast<float>(m_aliensAlive) / m_alienPresence.size()));
	const float deltaX = clamp(minDelta, speed * elapsed * static_cast<float>(m_orientation), maxDelta);
	m_gridPos.x += deltaX;

	positionAliens();

	computeAttackArea();

	if (m_area.m_max.y >= cGame::get().getScreenHeight())
	{
		cGame::get().onAlienSquadronLanded();
	}
}

//----------------------------------------------------------------------------
bool cAlienSquadron::collideRocket(const cShipRocket& rocket)
{
	bool collision = false;

	const cAABB2D damageBox(rocket.getPos() + rocket.def().damageBox);

	// Discard if does not overlap our area bounding box>	DICEInvaders.exe!cAlienSquadron::collideRocket(const cShipRocket & rocket={...}) Line 102	C++

	cAABB2D overlappingArea;
	if (overlap(m_area, damageBox, overlappingArea))
	{
		tCellIdx minAlienCell = getCellForCoord(overlappingArea.m_min);
		tCellIdx maxAlienCell = getCellForCoord(overlappingArea.m_max);
		DI_assert((minAlienCell != INVALID_CELL_IDX) || (maxAlienCell != INVALID_CELL_IDX), "We overlapped the squadron area, so we should have found a valid cell. What happened?");

		if (minAlienCell == INVALID_CELL_IDX)
		{
			minAlienCell = maxAlienCell;
		}
		else if (maxAlienCell == INVALID_CELL_IDX)
		{
			maxAlienCell = minAlienCell;
		}

		// Refine collisions against all the involved grids
		for (unsigned i = minAlienCell.row; i <= maxAlienCell.row; ++i)
		{
			for (unsigned j = minAlienCell.column; j <= maxAlienCell.column; ++j)
			{
				tGameObjectId& alienObjectId = m_alienPresence[(i * def().columns) + j];
				if (alienObjectId != INVALID_GAMEOBJECT_ID)
				{
					cAlien* const alien = static_cast<cAlien*>(cGame::get().getGameObjectManager().getGameObject(alienObjectId));
					const cAABB2D alienHitBox(alien->getPos() + alien->def().hitBox);
					if (overlap(damageBox, alienHitBox))
					{
						cGame::get().onAlienDestroyed(alienObjectId);

						destroyAlien(alienObjectId);

						collision = true;
					}
				}
			}
		}
	}

	return collision;
}

//----------------------------------------------------------------------------
void cAlienSquadron::destroyAllAliens()
{
	for (tGameObjectId& alienObjectId : m_alienPresence)
	{
		if (alienObjectId != INVALID_GAMEOBJECT_ID)
		{
			destroyAlien(alienObjectId);
		}
	}

	DI_assert(m_aliensAlive == 0, "All aliens should be dead by now");
}

//----------------------------------------------------------------------------
cAlienSquadron::tCellIdx cAlienSquadron::getCellForCoord(const cCoord2D& coord) const
{
	tCellIdx cellIdx(INVALID_CELL_IDX);

	const float gridHrzSize = static_cast<float>(IDiceInvaders::SPRITE_SIZE + def().hrzInterSpace);
	const float gridVrtSize = static_cast<float>(IDiceInvaders::SPRITE_SIZE + def().vrtInterSpace);

	if (isWithinRange(m_gridPos.x, coord.x, m_gridPos.x + (gridHrzSize * def().columns))
		&& isWithinRange(m_gridPos.y, coord.y, m_gridPos.y + (gridVrtSize * def().rows)))
	{
		cellIdx.row = (std::min)(static_cast<unsigned>((coord.y - m_gridPos.y) / gridVrtSize), def().rows - 1);
		cellIdx.column = (std::min)(static_cast<unsigned>((coord.x - m_gridPos.x) / gridHrzSize), def().columns - 1);
	}

	return cellIdx;
}

//----------------------------------------------------------------------------
void cAlienSquadron::spawnAliensIfNeeded()
{
	if (m_aliensAlive == 0)
	{
		m_gridPos = def().initialGridPos;
		m_orientation = O_LEFT_TO_RIGHT;
		m_area = {};

		m_alienPresence.clear();
		const unsigned numAliens = def().rows * def().columns;
		m_alienPresence.reserve(numAliens);

		std::generate_n(std::back_inserter(m_alienPresence), numAliens,
			[this]
		{
			const size_t count = m_alienPresence.size();
			const size_t row = count / def().columns;
			const size_t column = count % def().columns;
			const cAlienDef& alienDef = (row == 0) ? s_firstLineAlien : s_defaultAlien;
			return cGame::get().getGameObjectManager().createGameObject<cAlien>(alienDef, row, column, *this);
		});

		m_aliensAlive = m_alienPresence.size();
	}
}

//----------------------------------------------------------------------------
void cAlienSquadron::positionAliens() const
{
	for (tGameObjectId alienObjectId : m_alienPresence)
	{
		if (alienObjectId != INVALID_GAMEOBJECT_ID)
		{
			cAlien* const alien = static_cast<cAlien*>(cGame::get().getGameObjectManager().getGameObject(alienObjectId));
			alien->updatePosition();
		}
	}
}

//----------------------------------------------------------------------------
void cAlienSquadron::computeAttackArea()
{
	bool resetArea = true;
	for (tGameObjectId alien : m_alienPresence)
	{
		if (alien != INVALID_GAMEOBJECT_ID)
		{
			const cCoord2D& alienPos = static_cast<cAlien*>(cGame::get().getGameObjectManager().getGameObject(alien))->getPos();
			constexpr cCoord2D alienSize(static_cast<float>(IDiceInvaders::SPRITE_SIZE));
			const cAABB2D alienAABB2D(alienPos, alienPos + alienSize);
			if (resetArea)
			{
				resetArea = false;
				m_area = alienAABB2D;
			}
			else
			{
				m_area.includeAABB2D(alienAABB2D);
			}
		}
	}
}

//----------------------------------------------------------------------------
void cAlienSquadron::destroyAlien(tGameObjectId& alienObjectId)
{
	IGameObject* const alien = cGame::get().getGameObjectManager().getGameObject(alienObjectId);

	--m_aliensAlive;
	alien->setPendingDestroy();
	alienObjectId = INVALID_GAMEOBJECT_ID;
}