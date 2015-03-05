// ----------------------------------------------------
// Vector class represents point in space   -----------
// ----------------------------------------------------

#ifndef __P2VECTOR_H__
#define __P2VECTOR_H__

#include "p2Defs.h"

class p2Vector2
{
public: 

	float x, y;

	// Creators
	p2Vector2& create(float x, float y);
	p2Vector2& create(const p2Vector2& v);

	// Math
	p2Vector2 operator -(const p2Vector2& v) const;
	p2Vector2 operator +(const p2Vector2& v) const;

	const p2Vector2& operator -=(const p2Vector2& v);
	const p2Vector2& operator +=(const p2Vector2& v);

	// Compare
	bool operator ==(const p2Vector2& v) const;
	bool operator !=(const p2Vector2& v) const;

	// Utils
	bool IsZero() const;
	p2Vector2& SetToZero();
	p2Vector2& Negate();

	// Distance
	float DistanceTo(const p2Vector2& v) const;
	float DistanceNoSqrt(const p2Vector2& v) const;
	float DistanceManhattan(const p2Vector2& v) const;
};

#endif // __P2VECTOR_H__