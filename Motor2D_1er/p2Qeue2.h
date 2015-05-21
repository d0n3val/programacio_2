// ----------------------------------------------------
// FIFO implementation with single linked list  -------
// ----------------------------------------------------

#ifndef __p2Qeue2_H__
#define __p2Qeue2_H__

#include "p2Defs.h"
#include "p2DynArray.h"

template<class VALUE>
class p2Qeue2
{
public:
	
	uint left_index = 0;

	p2DynArray<VALUE> data;

	/**
	* Get Size
	*/
	unsigned int Count() const
	{
		return data.Count() - left_index;
	}

	/**
	* push up front
	*/
	void Push(const VALUE& item)
	{
		data.PushBack(item);
	}

	/**
	* Pops last item
	*/
	bool Pop(VALUE& item)
	{
		if(left_index < data.Count())
		{
			item = data[left_index++];
			return true;
		}

		return false;
	}

	/**
	* Reads one of the values
	*/
	const VALUE* Peek(unsigned int index) const
	{
		index += left_index;
		if(index < data.Count())
			return &data[index];

		return NULL;
	}

	/**
	* Destroy and free all mem
	*/
	void Clear()
	{
		data.Clear();
	}

};
#endif /*__p2Qeue2_H__*/