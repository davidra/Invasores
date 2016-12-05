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
	m_sprite->draw(static_cast<int>(m_coord.x), static_cast<int>(m_coord.y));
}
