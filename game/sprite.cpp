#include "pch/pch.h"

#include "game/game.h"
#include "game/sprite.h"
#include "DiceInvaders.h"

//----------------------------------------------------------------------------
void cSprite::init(const IGameObjectDef& gameObjectDef)
{
	IGameObject::init(gameObjectDef);

	m_sprite = cGame::get().getInterface().createSprite(def().spriteName);
	DI_assert(m_sprite != nullptr, "Could not create sprite %s ", def().spriteName);
}

//----------------------------------------------------------------------------
void cSprite::draw()
{
	m_sprite->draw(m_hrzPos, m_vrtPos);
}
