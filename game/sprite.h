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
	void			show() { m_show = true; }
	void			hide() { m_show = false; }
	bool			isHidden() const { return m_show; }
	void			blink(unsigned changesPerSecond) { m_blinkChangesPerSecond = changesPerSecond; }

protected:
	ISprite*	m_sprite				= nullptr;
	cCoord2D	m_coord;
	bool		m_show					= true;
	unsigned	m_blinkChangesPerSecond = 0;		// Number of times the sprite will alternate between hidden and shown per second
};