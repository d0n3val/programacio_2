#include <stdlib.h>

#include "p2Defs.h"
#include "p2Log.h"
#include "p2PerfTimer.h"
#include "j1App.h"
#include "p2SString.h"
#include "p2DynArray.h"

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

int fibonnacci(uint pos)
{
	int n = 1;
	int prev = 0;

	for(uint i = 0; i < pos; ++i)
	{
		n += prev;
		prev = n - prev;
	}

	return n;
}

int fibonnacci_ofuscated(uint pos)
{
	uint n = 1;
	for(uint i = 0, p = i; i < pos; ++i, n += p, p = n - p);
	return n;
}

int fibonnacci_recursive(uint pos)
{
	if(pos == 0 || pos == 1)
		return 1;
	else
		return fibonnacci_recursive(pos - 1) + fibonnacci_recursive(pos - 2);
}


int main(int argc, char* args[])
{
	LOG("J1 engine starting ...");

	int a = fibonnacci(10);
	int b = fibonnacci_ofuscated(10);
	int c = fibonnacci_recursive(10);

	MainState state = CREATE;
	int result = EXIT_FAILURE;
	j1App* j1 = NULL;
	j1PerfTimer ptimer;

	while(state != EXIT)
	{
		ptimer.Start();

		switch(state)
		{

			// Allocate the engine --------------------------------------------
			case CREATE:
			LOG("CREATION PHASE ===============================");

			j1 = new j1App((argc > 1) ? args[1] : "");

			if(j1 != NULL)
				state = AWAKE;
			else
				state = FAIL;

			LOG("PHASE FINISHED: %.2f ms", ptimer.ReadMs());
			break;

			// Awake all modules -----------------------------------------------
			case AWAKE:
			LOG("AWAKE PHASE ===============================");
			if(j1->Awake() == true)
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
			if(j1->Start() == true)
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
			if(j1->Update() == false)
				state = CLEAN;
			break;

			// Cleanup allocated memory -----------------------------------------
			case CLEAN:
			LOG("CLEANUP PHASE ===============================");
			if(j1->CleanUp() == true)
			{
				RELEASE(j1);
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