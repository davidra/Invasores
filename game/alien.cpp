#include "pch/pch.h"

#include "game/game.h"
#include "game/alien.h"
#include "game/alienbomb.h"
#include "game/aliensquadron.h"
#include "game/playership.h"
#include "DiceInvaders.h"

const cAlienDef s_defaultAlien;
const cAlienDef s_firstLineAlien(cAABB2D{ { 2.0f, 5.0f },{ 30.0f, 27.0f } }, 200, std::make_tuple(1.0f, 10.0f), true, "data/enemy2.bmp");

//----------------------------------------------------------------------------
void cAlien::init(const IGameObjectDef& gameObjectDef)
{
	cSprite::init(gameObjectDef);

	m_timeForNextBomb = generateRandomBombTimer();
}

//----------------------------------------------------------------------------
void cAlien::update(float elapsed)
{
	// Collision with the player?
	const cPlayerShip* const playerShip = cGame::get().getPlayerShip();
	if (!playerShip->isInvulnerable())
	{
		const cAABB2D damageBox = getPos() + def().hitBox;
		const cAABB2D playerHitBox = playerShip->getPos() + cAABB2D(cCoord2D(0.0f), cCoord2D(static_cast<float>(IDiceInvaders::SPRITE_SIZE)));
		if (overlap(damageBox, playerHitBox))
		{
			cGame::get().onPlayerHit();
		}
	}

	// Throw bombs?
	if (def().throwBombs)
	{
		m_timeForNextBomb -= elapsed;
		if (m_timeForNextBomb <= 0.0f)
		{
			cGame::get().getGameObjectManager().createGameObject<cAlienBomb>(s_defaultAlienBomb, cCoord2D(getPos().x, getPos().y + IDiceInvaders::SPRITE_SIZE));
			m_timeForNextBomb = generateRandomBombTimer();
		}
	}
}

//----------------------------------------------------------------------------
void cAlien::updatePosition()
{
	const float cellWidth = static_cast<float>(m_squadron.def().hrzInterSpace + IDiceInvaders::SPRITE_SIZE);
	const float cellHeight = static_cast<float>(m_squadron.def().vrtInterSpace + IDiceInvaders::SPRITE_SIZE);
	const cCoord2D posInSquadron(m_column * cellWidth, m_row * cellHeight);

	m_coord = m_squadron.getPos() + posInSquadron;
}

//----------------------------------------------------------------------------
float cAlien::generateRandomBombTimer() const
{
	static std::mt19937 mersenne_twister_generator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

	const auto distribution = std::uniform_real_distribution<float>(std::get<0>(def().bombWindow), std::get<1>(def().bombWindow));
	return distribution(mersenne_twister_generator);
}