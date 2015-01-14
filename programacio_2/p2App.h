#ifndef __P2APP_H__
    #define __P2APP_H__

#include "p2List.h"
#include "p2Module.h"

class p2App
{
    public:

        // Constructor
        p2App();

        // Destructor
        virtual ~p2App();

        // Called before render is available
        bool Awake();

        // Called before the first frame
        bool Start();

        // Called each loop iteration
        bool Update();

        // Called before quitting
        bool CleanUp();

		void AddModule(p2Module* module);

    private:
        bool				quitting;
		p2list<p2Module*>	modules;
};

#endif