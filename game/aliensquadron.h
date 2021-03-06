#pragma once
/***************************************************************************************************
aliensquadron.h

Gameobject class for the alien squadron
 
by David Ramos
***************************************************************************************************/
#include "game/gameobject.h"
#include "game/gameobjectmanager.h"

class cShipRocket;

//----------------------------------------------------------------------------
class cAlienSquadronDef : public IGameObjectDef
{
public:
	constexpr cAlienSquadronDef(unsigned rows = 4, unsigned columns = 9, float hrzSpeed = 60.0f, float hrzMaxSpeed = 200.0f, unsigned vrtStep = 32,
		unsigned hrzInterSpace = 8, unsigned vrtInterSpace = 0, const cCoord2D& initialGridPos = {0.0f, 32.0f})
		: rows(rows)
		, columns(columns)
		, hrzSpeed(hrzSpeed)
		, hrzMaxSpeed(hrzMaxSpeed)
		, vrtStep(vrtStep)
		, hrzInterSpace(hrzInterSpace)
		, vrtInterSpace(vrtInterSpace)
		, initialGridPos(initialGridPos)
	{}

	unsigned	rows;
	unsigned	columns;
	float		hrzSpeed;		// base horizontal speed
	float		hrzMaxSpeed;	// max horizontal speed
	unsigned	vrtStep;		// vertical downward step when moving down
	unsigned	hrzInterSpace;	// horizontal space between aliens, in pixels
	unsigned	vrtInterSpace;	// vertical space between aliens, in pixels
	cCoord2D	initialGridPos;	// position of where the squadron starts when spawned
};

//----------------------------------------------------------------------------
class cAlienSquadron : public IGameObject
{
	REGISTER_GAMEOBJECT(cAlienSquadron);

public:
	virtual void	update(float elapsed) override;
	virtual void	draw(float ) override {}

	const cCoord2D& getPos() const { return m_gridPos; }

	bool			collideRocket(const cShipRocket& rocket);
	void			destroyAllAliens();

private:
	struct tCellIdx
	{
		constexpr tCellIdx(unsigned row, unsigned column)
			: row(row)
			, column(column)
		{}

		constexpr bool operator== (const tCellIdx& rhs) const { return (row == rhs.row) && (column == rhs.column); }
		constexpr bool operator!= (const tCellIdx& rhs) const { return !(*this == rhs); }

		unsigned row;
		unsigned column;
	};
	static tCellIdx INVALID_CELL_IDX;

	tCellIdx			getCellForCoord(const cCoord2D& coord) const;
	void				spawnAliensIfNeeded();
	void				positionAliens() const;
	void				computeAttackArea();
	void				destroyAlien(tGameObjectId& alienObjectId);


	cAABB2D				m_area				= {};				// Defines the bounding box enclosing the area where there are aliens alive
	cCoord2D			m_gridPos			= {};				// top-left coordinate of where the alien grid starts, it does not change just because aliens are destroyed
	typedef std::vector<tGameObjectId> tAlienContainer;
	tAlienContainer		m_alienPresence;						// Aliens of this squadron defined in growing row, column order
	unsigned			m_aliensAlive		= 0;				// Number of aliens currently alive

	enum eOrientation
	{
		O_RIGHT_TO_LEFT = -1,
		O_LEFT_TO_RIGHT = 1
	};
	eOrientation		m_orientation		= O_LEFT_TO_RIGHT;	// Orientation of horizontal movement
};

extern const cAlienSquadronDef s_defaultAlienSquadron;