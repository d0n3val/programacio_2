#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2String.h"
#include "p2Vector2.h"

class j1EntityManager;

class j1Entity
{
public:

	j1Entity() : name("Unnamed"), active(false), manager(NULL)
	{
		position.SetToZero();
	}

	void Init(j1EntityManager* manager)
	{
		ASSERT(manager);
		this->manager = manager;
	}

	bool IsActive() const
	{
		return active;
	}

	bool Activate()
	{
		if(active == false)
		{
			OnActivate();
			active = true;
		}
	}

	bool Deactivate()
	{
		if(active == true)
		{
			OnDeactivate();
			active = false;
		}
	}

	// Called before the first frame if it was activated before that
	virtual void Start() {}

	// Called each loop iteration
	virtual void FixedUpdate(float dt) {}

	// Called each logic iteration
	virtual void LogicUpdate(float dt) {}

	// Called before quitting
	virtual void CleanUp() {}

private:

	// Called every time the entity becomes active
	virtual void OnActivate() {}

	// Called every time the entity becomes inactive
	virtual void OnDeactivate() {}

public:

	p2String			name;
	p2Vector2			position;

private:

	bool				active;
	j1EntityManager*	manager;

};

#endif // __J1ENTITY_H__