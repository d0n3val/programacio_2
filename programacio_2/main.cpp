#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"

#include "SDL\include\SDL.h"
#pragma comment( lib, "SDL/lib/x86/SDL2.lib" )
#pragma comment( lib, "SDL/lib/x86/SDL2main.lib" )

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

int main( int argc, char* args[] )
{
	LOG("P2 engine starting ...");

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	MainState state = CREATE;
	int result = EXIT_FAILURE;
	p2App* p2 = NULL;

	while(state != EXIT)
	{
		switch(state)
		{

		// Allocate the engine --------------------------------------------
		case CREATE:
			LOG("CREATION PHASE ---------------");
			
			p2 = new p2App((argc>1) ? args[1] : "");

			if(p2 != NULL)
				state = AWAKE;
			else
				state = FAIL;
		break;

		// Awake all modules -----------------------------------------------
		case AWAKE:
			LOG("AWAKE PHASE ------------------");
			if(p2->Awake() == true)
				state = START;
			else
			{
				LOG("ERROR: Awake failed");
				state = FAIL;
			}
		break;

		// Call all modules before first frame  ----------------------------
		case START:
			LOG("START PHASE ------------------");
			if(p2->Start() == true)
			{
				state = LOOP;
				LOG("UPDATE PHASE-----------------");
			}
			else
			{
				state = FAIL;
				LOG("ERROR: Start failed");
			}
		break;

		// Loop all modules until we are asked to leave ---------------------
		case LOOP:
			if(p2->Update() == false)
				state = CLEAN;
		break;

		// Cleanup allocated memory -----------------------------------------
		case CLEAN:
			LOG("CLEANUP PHASE-----------------");
			if(p2->CleanUp() == true)
			{
				RELEASE(p2);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;
		break;

		// Exit with errors and shame ---------------------------------------
		case FAIL:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = EXIT;
		break;
		}
	}

	LOG("... Bye! :)\n");

	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	void* s = malloc(200);
	int *p = new int[100];
	_CrtDumpMemoryLeaks();

	// Dump memory leaks
	return result;
}