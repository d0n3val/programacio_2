// ----------------------------------------------------
// p2Vector2 class definition for points in 2d space --
// ----------------------------------------------------

#include <math.h>
#include "p2Vector2.h"

// Creators -------------------------------------------
p2Vector2& p2Vector2::create(float x, float y)
{
	this->x = x;
	this->y = y;

	return(*this);
}

p2Vector2& p2Vector2::create(const p2Vector2& v)
{
	this->x = v.x;
	this->y = v.y;

	return(*this);
}

// Math ------------------------------------------------
p2Vector2 p2Vector2::operator -(const p2Vector2 &v ) const
{
	p2Vector2 r;

	r.x = x - v.x;
	r.y = y - v.y;

	return(r);
}

p2Vector2 p2Vector2::operator +(const p2Vector2 &v ) const
{
	p2Vector2 r;

	r.x = x + v.x;
	r.y = y + v.y;

	return(r);
}

p2Vector2 p2Vector2::operator -=(const p2Vector2 &v )
{
	x -= v.x;
	y -= v.y;

	return(*this);
}

p2Vector2 p2Vector2::operator +=(const p2Vector2 &v )
{
	x += v.x;
	y += v.y;

	return(*this);
}

p2Vector2 p2Vector2::operator *(float f) const
{
	p2Vector2 r;

	r.x = x * f;
	r.y = y * f;

	return(r);
}

p2Vector2 p2Vector2::operator /(float f) const
{
	p2Vector2 r;

	r.x = x / f;
	r.y = y / f;

	return(r);

	/*
	// Optimization ? must test ...
	float inv = 1.0f / f;
	p2Vector2 r;

	r.x = x * inv;
	r.y = y * inv;

	return(r);
	*/
}

p2Vector2 p2Vector2::operator /=(float f)
{
	x /= f;
	y /= f;

	return(*this);
}

p2Vector2 p2Vector2::operator *=(float f)
{
	x *= f;
	y *= f;

	return(*this);
}

bool p2Vector2::operator ==(const p2Vector2& v) const
{
	return (x == v.x && y == v.y);
}

bool p2Vector2::operator !=(const p2Vector2& v) const
{
	return (x != v.x || y != v.y);
}

// Utils ------------------------------------------------
bool p2Vector2::IsZero() const
{
	return (x == 0.0f && y == 0.0f); 
}

p2Vector2& p2Vector2::SetToZero()
{
	x = y = 0.0f;
	return(*this);
}

p2Vector2& p2Vector2::Negate()
{
	x = -x;
	y = -y;

	return(*this);
}

// Distances ---------------------------------------------
float p2Vector2::DistanceTo(const p2Vector2& v) const
{
	float fx = x - v.x;
	float fy = y - v.y;

	return sqrtf((fx*fx) + (fy*fy));
}

float p2Vector2::DistanceNoSqrt(const p2Vector2& v) const
{
	float fx = x - v.x;
	float fy = y - v.y;

	return (fx*fx) + (fy*fy);
}

float p2Vector2::DistanceManhattan(const p2Vector2& v) const
{
	return fabsf(v.x - x) + fabsf(v.y - y);
}
