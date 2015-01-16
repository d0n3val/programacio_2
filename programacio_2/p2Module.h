#ifndef __P2MODULE_H__
    #define __P2MODULE_H__

class p2App;

class p2Module
{
public:

	p2Module() { Active = false; UpdateOnPause = false; App = NULL; }

	void Init(p2App* App) { this->App = App; Active = true; }

    // Called before render is available
	virtual bool Awake() { return true; }

    // Called before the first frame
	virtual bool Start() { return true; }

    // Called each loop iteration
	virtual bool PreUpdate() { return true; }
	
	// Called each loop iteration
    virtual bool Update(float dt) = 0;

	// Called each loop iteration
	virtual bool PostUpdate() { return true; }

    // Called before quitting
	virtual bool CleanUp() { return true; }

public:

	bool	Active;
	bool	UpdateOnPause;
	p2App*	App;
	
};

#endif // __P2MODULE_H__