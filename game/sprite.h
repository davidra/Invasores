#pragma once
/***************************************************************************************************
sprite.h

Generic sprite gameobject
 
by David Ramos
***************************************************************************************************/
#include "game/gameobject.h"

class ISprite;

//----------------------------------------------------------------------------
class cSpriteDef : public IGameObjectDef
{
public:
	cSpriteDef(const char* spriteName)
		: spriteName(spriteName)
	{}

	const char* spriteName = "";
};

//----------------------------------------------------------------------------
class cSprite : public IGameObject
{
	REGISTER_GAMEOBJECT(cSprite);

public:
	cSprite(float hrzPos, float vrtPos)
		: m_hrzPos(hrzPos)
		, m_vrtPos(vrtPos)
	{}

	void init(const IGameObjectDef& gameObjectDef) override;
	void update(float) override {};
	void draw() override;

protected:
	ISprite*	m_sprite = nullptr;
	float		m_hrzPos = 0;
	float		m_vrtPos = 0;
};