// ----------------------------------------------------
// Circle class    -----------
// ----------------------------------------------------

#ifndef __P2CIRCLE_H__
#define __P2CIRCLE_H__

#include "p2Defs.h"
#include "p2Point.h"

#define PI 3.141521f

template<class TYPE>
class p2Circle
{
public:

	p2Point<TYPE> position;
	TYPE radius;

	void p2Circle(const TYPE& x, const TYPE& y, const TYPE& r)
	{
		position.x = x;
		position.y = y;
		radius = r;
	}

	// Intersection Check --------------------
	bool Intersects(const p2Circle& circle) const
	{
		return circle.position.DistanceTo(position) < circle.radius + radius;
	}

	TYPE GetArea() const
	{
		return radius * radius * PI;
	}

};

#endif // __P2CIRCLE_H__