#include "pch/pch.h"

#include "game/alien.h"
#include "game/aliensquadron.h"
#include "DiceInvaders.h"

const cAlienDef s_defaultAlien;
const cAlienDef s_firstLineAlien(cAABB2D{ { 2.0f, 5.0f },{ 30.0f, 27.0f } }, 200, true, "data/enemy2.bmp");

//----------------------------------------------------------------------------
void cAlien::update(float elapsed)
{
	if (def().throwBombs)
	{
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