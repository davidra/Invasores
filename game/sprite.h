#pragma once
/***************************************************************************************************
sprite.h

Generic sprite gameobject
 
by David Ramos
***************************************************************************************************/
#include "game/gameobject.h"

struct ISprite;

//----------------------------------------------------------------------------
class cSpriteDef : public IGameObjectDef
{
public:
	constexpr cSpriteDef(const char* spriteName)
		: spriteName(spriteName)
	{}

	const char* spriteName = "";
};

//----------------------------------------------------------------------------
class cSprite : public IGameObject
{
	REGISTER_GAMEOBJECT(cSprite);

public:
	cSprite(const cCoord2D& coord)
		: m_coord(coord)
	{}

	~cSprite() override;

	void			init(const IGameObjectDef& gameObjectDef) override;
	void			update(float) override {};
	void			draw(float elapsed) override;
	const cCoord2D&	getPos() const { return m_coord; }

protected:
	ISprite*	m_sprite = nullptr;
	cCoord2D	m_coord;
};