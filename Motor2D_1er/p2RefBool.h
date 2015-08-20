#ifndef __j1REFBOOL_H__
#define __j1REFBOOL_H__

class p2RefBool
{
public:
	p2RefBool()
	{
		value = 0;
	}
	inline bool Get()
	{
		return value > 0;
	}
	inline bool Set()
	{
		++value;
		return value == 1;
	}
	inline bool Unset()
	{
		if(value > 0)
		{
			--value;
		}

		return value == 0;
	}

private:
	int	value;
};

#endif // __j1REFBOOL_H__