// ----------------------------------------------------
// LIFO implementation as a dynamic array  ------------
// ----------------------------------------------------

#ifndef __p2Stack_H__
#define __p2Stack_H__

#include "p2Defs.h"

#define STACK_BLOCK_SIZE 16

template<class VALUE>
class p2Stack
{
private:

	VALUE*			data;
	unsigned int	mem_capacity;
	unsigned int	num_elements;

public:

	// Constructors
	p2Stack() : mem_capacity(0), num_elements(0), data(NULL)
	{
		Alloc(STACK_BLOCK_SIZE);
	}

	p2Stack(unsigned int capacity) : mem_capacity(0), num_elements(0), data(NULL)
	{
		Alloc(capacity);
	}

	// Destructor
	~p2Stack()
	{
		delete[] data;
	}

	// Data Management
	void Push(const VALUE& element)
	{
		if(num_elements >= mem_capacity)
		{
			Alloc(mem_capacity + STACK_BLOCK_SIZE);
		}

		data[num_elements++] = element;
	}

	bool Pop(VALUE& result)
	{
		if(num_elements > 0)
		{
			result = data[--num_elements];
			return true;
		}
		return false;
	}

	void Clear()
	{
		num_elements = 0;
	}

	const VALUE* Peek(unsigned int index) const
	{
		VALUE* result = NULL;

		if(index < num_elements)
			return result = &data[index];

		return result;
	}

	// Utils
	unsigned int GetCapacity() const
	{
		return mem_capacity;
	}

	unsigned int Count() const
	{
		return num_elements;
	}

private:

	// Private Utils
	void Alloc(unsigned int mem)
	{
		VALUE* tmp = data;

		mem_capacity = mem;
		data = new VALUE[mem_capacity];

		num_elements = MIN(mem_capacity, num_elements);

		if(tmp != NULL)
		{
			for(unsigned int i = 0; i < num_elements; ++i)
				data[i] = tmp[i];

			delete[] tmp;
		}
	}
};

#endif // __p2Stack_H__