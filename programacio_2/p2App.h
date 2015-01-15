#ifndef __P2APP_H__
    #define __P2APP_H__

#include "p2List.h"
#include "p2RefBool.h"
#include "p2Timer.h"
#include "p2IniReader.h"
#include "p2Module.h"

// Modules
#include "p2Window.h"
#include "p2Input.h"

class p2App
{
    public:

        // Constructor
        p2App(const char* );

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

		// Add a new module to handle
		void AddModule(p2Module* module);

	public:

		p2RefBool	pause;
		p2IniReader	config;
		Uint32		frames;
		int			fps_counter;
		int			last_frame_ms;
		int			last_fps;
		int			capped_ms;

		// Modules
		p2Window*	win;
		p2Input*	input;

    private:

        bool				quitting;
		p2Timer				ms_timer;
		p2Timer				fps_timer;
		p2list<p2Module*>	modules;
};

#endif