#pragma once
/***************************************************************************************************
playership.h

Player's ship class
 
by David Ramos
***************************************************************************************************/
#include "game/gameobject.h"
#include "game/sprite.h"

//----------------------------------------------------------------------------
class cPlayerShipDef : public cSpriteDef
{
public:
	constexpr cPlayerShipDef(float vertPos = 480 - 32, float speed = 160.0f, float firePeriod = 0.4f, float invulnerableTimeAfterHit = 1.5f, 
		const cAABB2D& hitBox = { {2.0f, 7.0f}, {30.0f, 25.0f} }, const char* spriteName = "data/player.bmp")
		: cSpriteDef(spriteName)
		, vertPos(vertPos)
		, speed(speed)
		, firePeriod(firePeriod)
		, invulnerableTimeAfterHit(invulnerableTimeAfterHit)
		, hitBox(hitBox)
	{}

	float			vertPos;
	float			speed;
	float			firePeriod; // period of the rate of fire, in seconds
	float			invulnerableTimeAfterHit;
	const cAABB2D	hitBox;		// sprite-local box where the ship can be hit
};

//----------------------------------------------------------------------------
class cPlayerShip : public cSprite
{
	REGISTER_GAMEOBJECT(cPlayerShip);

public:
	cPlayerShip(float initialXPos);

	void init(const IGameObjectDef& gameObjectDef) override;
	void update(float elapsed) override;

	void onHit();
	void onInvulnerable(float timer);
	void onRapidFirePowerUp(float timer) { m_rapidFireTimer = timer; }

	bool isInvulnerable() const { return m_invulnerableTimer > 0.0f; }

private:
	float		m_lastShot			= 0.0f;
	float		m_invulnerableTimer = 0.0f;
	float		m_rapidFireTimer	= 0.0f;
};

extern const cPlayerShipDef s_defaultPlayerShipDef;