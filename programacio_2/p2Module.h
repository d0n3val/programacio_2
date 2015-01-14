#ifndef __P2MODULE_H__
    #define __P2MODULE_H__

class p2App;

class p2Module
{
public:

	void Init(p2App* App) { this->App = App; Active = true; }

    // Called before render is available
    virtual bool Awake() = 0;

    // Called before the first frame
    virtual bool Start() = 0;

    // Called each loop iteration
    virtual bool Update(float dt) = 0;

    // Called before quitting
    virtual bool CleanUp() = 0;

public:

	bool	Active;
	p2App*	App;
	
};

#endif // __P2MODULE_H__