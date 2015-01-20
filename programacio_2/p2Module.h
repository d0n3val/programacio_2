#ifndef __P2MODULE_H__
    #define __P2MODULE_H__

class p2App;

class p2Module
{
public:

	p2Module() : active(false), update_on_pause(false), App(NULL) {}

	void Init(p2App* App) { this->App = App; active = true; }

    // Called before render is available
	virtual bool Awake() { return true; }

    // Called before the first frame
	virtual bool Start() { return true; }

    // Called each loop iteration
	virtual bool PreUpdate() { return true; }
	
	// Called each loop iteration
    virtual bool Update(float dt) { return true; }

	// Called each loop iteration
	virtual bool PostUpdate() { return true; }

    // Called before quitting
	virtual bool CleanUp() { return true; }

public:

	char	name[SHORT_STR];
	bool	active;
	bool	update_on_pause;
	p2App*	App;

};

#endif // __P2MODULE_H__