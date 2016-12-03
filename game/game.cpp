#include "pch/pch.h"

#include "game.h"
#include "playership.h"
#include "DiceInvaders.h"

std::unique_ptr<cGame> cGame::s_instance;

//----------------------------------------------------------------------------
void cGame::init(IDiceInvaders* diceInvadersInterface, int width, int height)
{
	DI_assert(diceInvadersInterface != nullptr, "Fatal error: Null diceInvadersInterface!");
	m_invadersInterface = diceInvadersInterface;

	m_invadersInterface->init(width, height);
	m_width = width;
	m_height = height;

	// create the player's ship
	m_gameObjectManager.createGameObject<cPlayerShip>(s_defaultPlayerShipDef, width / 2);
}

//----------------------------------------------------------------------------
void cGame::update(float elapsed)
{
	m_gameObjectManager.update(elapsed);
}

//----------------------------------------------------------------------------
void cGame::draw()
{
	m_gameObjectManager.draw();
}

//----------------------------------------------------------------------------
void cGame::shutDown()
{
	m_gameObjectManager.destroyAllGameObjects();
	m_invadersInterface->destroy();
}