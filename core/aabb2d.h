#pragma once
/***************************************************************************************************
aabb2d.h

2D axis-aligned bounding box class
 
by David Ramos
***************************************************************************************************/
#include "core/coord2d.h"

class cAABB2D
{
public:
	cAABB2D() : m_min(), m_max() {}
	cAABB2D(const cCoord2D& aabbMin, const cCoord2D& aabbMax)
		: m_min(aabbMin)
		, m_max(aabbMax)
	{
		DI_assert(aabbMin <= aabbMax, "Incorrect values provided for 2D bounding box. aabbMin is not smaller than aabbMax");
	}

	// extends the bounding box to make sure it contains the specified coord
	void includeCoord(const cCoord2D& coord)
	{
		m_min = cCoord2D((std::min)(m_min.x, coord.x), (std::min)(m_min.y, coord.y));
		m_max = cCoord2D((std::max)(m_max.x, coord.x), (std::max)(m_max.y, coord.y));
	}

	// extends the bounding box to make sure it contains the specified coord
	void includeAABB2D(const cAABB2D& aabb2D)
	{
		includeCoord(aabb2D.m_min);
		includeCoord(aabb2D.m_max);
	}

	cCoord2D m_min;
	cCoord2D m_max;
};

//----------------------------------------------------------------------------
inline cAABB2D operator+ (const cAABB2D& aabb2D, const cCoord2D& coord)
{
	return cAABB2D(aabb2D.m_min + coord, aabb2D.m_max + coord);
}
inline cAABB2D operator+ (const cCoord2D& coord, const cAABB2D& aabb2D) { return aabb2D + coord; }

//----------------------------------------------------------------------------
inline bool overlap(const cAABB2D& lhs, const cAABB2D& rhs)
{
	// axis-aligned SAT
	const bool overlap_x = !((lhs.m_max.x < rhs.m_min.x) || (lhs.m_min.x > rhs.m_max.x));
	const bool overlap_y = !((lhs.m_max.y < rhs.m_min.y) || (lhs.m_min.y > rhs.m_max.y));

	return overlap_x && overlap_y;
}

//----------------------------------------------------------------------------
inline bool overlap(const cAABB2D& lhs, const cAABB2D& rhs, cAABB2D& overlappingArea)
{
	const bool overlapping = overlap(lhs, rhs);
	if (overlapping)
	{
		overlappingArea.m_min.x = (std::max)(lhs.m_min.x, rhs.m_min.x);
		overlappingArea.m_min.y = (std::max)(lhs.m_min.y, rhs.m_min.y);

		overlappingArea.m_max.x = (std::min)(lhs.m_max.x, rhs.m_max.x);
		overlappingArea.m_max.y = (std::min)(lhs.m_max.y, rhs.m_max.y);
	}

	return overlapping;
}