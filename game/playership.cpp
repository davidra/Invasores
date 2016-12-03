#include "pch/pch.h"

#include "game/game.h"
#include "game/playership.h"
#include "game/shiprocket.h"
#include "DiceInvaders.h"

const cPlayerShipDef s_defaultPlayerShipDef;

//----------------------------------------------------------------------------
cPlayerShip::cPlayerShip(int initialXPos) 
	: cSprite(initialXPos, 0)
{}

//----------------------------------------------------------------------------
void cPlayerShip::init(const IGameObjectDef& gameObjectDef)
{
	cSprite::init(gameObjectDef);

	m_vrtPos = def().vertPos;
}

//----------------------------------------------------------------------------
void cPlayerShip::update(float elapsed)
{
	cSprite::update(elapsed);

	const float moveDelta = elapsed * def().speed;

	cGame& game = cGame::get();

	IDiceInvaders::KeyStatus keys;
	game.getInterface().getKeyStatus(keys);
	if (keys.right)
	{
		m_hrzPos += moveDelta;
	}
	else if (keys.left)
	{
		m_hrzPos -= moveDelta;
	}

	m_hrzPos = clamp<float>(0, m_hrzPos, game.getScreenWidth() - IDiceInvaders::SPRITE_SIZE);

	const float currentTime = game.getInterface().getElapsedTime();
	if (keys.fire && ((currentTime - m_lastShot) > def().firePeriod))
	{
		game.getGameObjectManager().createGameObject<cShipRocket>(s_defaultShipRocket, m_hrzPos, m_vrtPos - IDiceInvaders::SPRITE_SIZE);
		m_lastShot = currentTime;
	}
}
