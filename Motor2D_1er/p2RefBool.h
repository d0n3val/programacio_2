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
	inline void Set()
	{
		++value;
	}
	inline void Unset()
	{
		if(value > 0)
		{
			--value;
		}
	}

private:
	int	value;
};

#endif // __j1REFBOOL_H__