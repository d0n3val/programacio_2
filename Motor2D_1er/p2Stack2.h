// ----------------------------------------------------
// LIFO implementation as a linked list (not optimal) -
// ----------------------------------------------------

#ifndef __p2Stack2_H__
#define __p2Stack2_H__

#include "p2Defs.h"
#include "p2List.h"

template<class VALUE>
class p2Stack2
{
private:

	p2List<VALUE> data;

public:

	// Data Management
	void Push(const VALUE& element)
	{
		data.add(element);
	}

	bool Pop(VALUE& result)
	{
		if(data.count() > 0)
		{
			result = data.end->data;
			data.del(data.end);
			return true;
		}
		return false;
	}

	void Clear()
	{
		data.clear();
	}

	uint Count()
	{
		return data.count();
	}

	const VALUE* Peek(unsigned int index) const
	{
		if(index < data.count())
			return &data[index];

		return NULL;
	}

};

#endif // __p2Stack2_H__