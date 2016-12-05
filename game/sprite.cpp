#include "pch/pch.h"

#include "game/game.h"
#include "game/sprite.h"
#include "DiceInvaders.h"

//----------------------------------------------------------------------------
cSprite::~cSprite()
{
	if (m_sprite)
	{
		m_sprite->destroy();
	}
}

//----------------------------------------------------------------------------
void cSprite::init(const IGameObjectDef& gameObjectDef)
{
	IGameObject::init(gameObjectDef);

	m_sprite = cGame::get().getInterface().createSprite(def().spriteName);
	DI_assert(m_sprite != nullptr, "Could not create sprite %s ", def().spriteName);
}

//----------------------------------------------------------------------------
void cSprite::draw(float )
{
	bool phasedIn = true;
	
	if (m_blinkChangesPerSecond > 0)
	{
		const float totalTime = cGame::get().getInterface().getElapsedTime();
		const float totalTimeDecimalPart = totalTime - static_cast<int>(totalTime);
		phasedIn = sin(M_PI * totalTimeDecimalPart * m_blinkChangesPerSecond) >= 0.0f;
	}

	if (m_show && phasedIn)
	{
		m_sprite->draw(static_cast<int>(m_coord.x), static_cast<int>(m_coord.y));
	}
	else
	{
		// "Hiding" is just drawing it outside of the screen
		m_sprite->draw(cGame::get().getScreenWidth(), cGame::get().getScreenHeight());
	}
}
