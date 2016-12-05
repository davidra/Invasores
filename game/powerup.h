#pragma once
/***************************************************************************************************
powerup.h

by David Ramos
***************************************************************************************************/
#include "game/gameobject.h"

enum class ePowerUpType
{
	LIFE_UP,
	RAPID_FIRE,
	INVULNERABILITY
};

//----------------------------------------------------------------------------
class cPowerUpDef : public IGameObjectDef
{
public:
	cPowerUpDef(ePowerUpType type = ePowerUpType::LIFE_UP, const char* visual = "|+|", const cAABB2D& hitBox = { {0.0f, 0.0f}, {32.0f, 32.0f} }, float speed = 160.0f)
		: type(type)
		, visual(visual)
		, hitBox(hitBox)
		, speed(speed)
	{}

	ePowerUpType	type;
	const char*		visual;
	cAABB2D			hitBox;
	float			speed;
};

//----------------------------------------------------------------------------
class cPowerUp : public IGameObject
{
	REGISTER_GAMEOBJECT(cPowerUp);

public:
	cPowerUp(const cCoord2D& coord)
		: m_coord(coord)
	{}

	void update(float elapsed) override;
	void draw(float elapsed) override;

private:
	cCoord2D m_coord;
};

extern const cPowerUpDef s_lifeUpPowerUp;
extern const cPowerUpDef s_rapidFirePowerUp;
extern const cPowerUpDef s_invulnerabilityPowerUp;