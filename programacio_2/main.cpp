#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "p2App.h"
#include "SDL.h"

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

	while(state != EXIT)
	{
		switch(state)
		{

		// Allocate the engine --------------------------------------------
		case CREATE:
			LOG("INFO: Creating engine");
			p2 = new p2App();

			if(p2 != NULL)
				state = AWAKE;
			else
				state = FAIL;
		break;

		// Awake all modules -----------------------------------------------
		case AWAKE:
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
			if(p2->Start() == true)
				state = LOOP;
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
			LOG("INFO: Cleaning Up");
			if(p2->CleanUp() == true)
			{
				delete(p2);
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

	LOG("... Bye! :)");
	return result;
}