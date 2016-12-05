#pragma once
/***************************************************************************************************
utils.h

general-purpose utility functions/types
 
by David Ramos
***************************************************************************************************/
template <typename T>
using tAlignedStorage = std::aligned_storage_t<sizeof(T), alignof(T)>;

//----------------------------------------------------------------------------
template <class T>
static T clamp(const T& low, const T& value, const T& high)
{
	return (value < low)
		? low
		: (value > high)
		? high
		: value;
}

//----------------------------------------------------------------------------
// Checks if value is inside the range [range_start, range_end], i.e., both ends are included 
template <class T>
static bool isWithinRange(const T& range_start, const T& value, const T& range_end)
{
	return (value >= range_start) && (value <= range_end);
}

//----------------------------------------------------------------------------
// Linear interpolation between two values
template <class T>
static T lerp(const T& start, const T& end, const float factor)
{
	return start + (end - start) * factor;
}

