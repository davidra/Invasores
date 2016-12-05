#pragma once
/***************************************************************************************************
alien.h

Gameobject class for the alien invaders
 
by David Ramos
***************************************************************************************************/
#include "game/sprite.h"

class cAlienSquadron;

//----------------------------------------------------------------------------
class cAlienDef : public cSpriteDef
{
public:
	cAlienDef(const cAABB2D& hitBox = { {4.0f, 5.0f}, {28.0f, 27.0f} }, unsigned scoreReward = 100, bool throwBombs = false, const char* spriteName = "data/enemy1.bmp")
		: cSpriteDef(spriteName)
		, hitBox(hitBox)
		, scoreReward(scoreReward)
		, throwBombs(throwBombs)
	{}

	cAABB2D		hitBox;			// sprite-local bounding box defining the hittable area of the alien
	unsigned	scoreReward;	// base reward when destroying this alien
	bool		throwBombs;		// indicates if this alien can throw bombs to the player
};

//----------------------------------------------------------------------------
class cAlien : public cSprite
{
	REGISTER_GAMEOBJECT(cAlien);

public:
	cAlien(unsigned row, unsigned column, cAlienSquadron& alienSquadron)
		: cSprite(cCoord2D(0.0f, 0.0f))
		, m_row(row)
		, m_column(column)
		, m_squadron(alienSquadron)
	{}

	void	update(float elapsed) override;
	void	updatePosition();

private:
	unsigned		m_row;
	unsigned		m_column;
	cAlienSquadron&	m_squadron;	// It is safe to store a reference parent alien squadron (as opposed to just a gameobjectid). 
								// An Alien can only exist as part of a squadron, and when a squadron is destroyed it destroys all its aliens
};

extern const cAlienDef s_firstLineAlien;
extern const cAlienDef s_defaultAlien;