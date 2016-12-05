#pragma once
/***************************************************************************************************
coord2d.h

2D coordinate class
 
by David Ramos
***************************************************************************************************/
class cCoord2D
{
public:
	constexpr cCoord2D() : x(), y() {}
	explicit constexpr cCoord2D(float val) : x(val), y(val) {}
	constexpr cCoord2D(float x, float y)
		: x(x)
		, y(y)
	{}

	float x;
	float y;
};

inline constexpr bool operator <(const cCoord2D& lhs, const cCoord2D& rhs)
{
	return (lhs.x < rhs.x)
		&& (lhs.y < rhs.y);
}

inline constexpr bool operator >(const cCoord2D& lhs, const cCoord2D& rhs)
{
	return (lhs.x > rhs.x)
		&& (lhs.y > rhs.y);
}

inline constexpr bool operator <=(const cCoord2D& lhs, const cCoord2D& rhs)
{
	return (lhs.x <= rhs.x)
		&& (lhs.y <= rhs.y);
}

inline constexpr bool operator >=(const cCoord2D& lhs, const cCoord2D& rhs)
{
	return (lhs.x >= rhs.x)
		&& (lhs.y >= rhs.y);
}

inline constexpr cCoord2D operator+ (const cCoord2D& lhs, const cCoord2D& rhs)
{
	return cCoord2D(lhs.x + rhs.x, lhs.y + rhs.y);
}

