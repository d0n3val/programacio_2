#include <stdlib.h>

#include "p2Defs.h"
#include "p2Log.h"
#include "p2PerfTimer.h"
#include "p2App.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

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

	MainState state = CREATE;
	int result = EXIT_FAILURE;
	p2App* p2 = NULL;
	p2PerfTimer ptimer;

	while(state != EXIT)
	{
		ptimer.Start();

		switch(state)
		{

		// Allocate the engine --------------------------------------------
		case CREATE:
			LOG("CREATION PHASE ===============================");

			p2 = new p2App((argc>1) ? args[1] : "");

			if(p2 != NULL)
				state = AWAKE;
			else
				state = FAIL;

			LOG("PHASE FINISHED: %.2f ms", ptimer.ReadMs());
		break;

		// Awake all modules -----------------------------------------------
		case AWAKE:
			LOG("AWAKE PHASE ===============================");
			if(p2->Awake() == true)
				state = START;
			else
			{
				LOG("ERROR: Awake failed");
				state = FAIL;
			}

			LOG("PHASE FINISHED: %.2f ms", ptimer.ReadMs());
		break;

		// Call all modules before first frame  ----------------------------
		case START:
			LOG("START PHASE ===============================");
			if(p2->Start() == true)
			{
				state = LOOP;
				LOG("PHASE FINISHED: %.2f ms", ptimer.ReadMs());
				LOG("UPDATE PHASE ===============================");
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
			LOG("CLEANUP PHASE ===============================");
			if(p2->CleanUp() == true)
			{
				RELEASE(p2);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			LOG("PHASE FINISHED: %.2f ms", ptimer.ReadMs());
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

	// Dump memory leaks
	return result;
}