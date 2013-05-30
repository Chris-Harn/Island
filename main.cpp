#include "defines.h"

int main( int argc, char **argv ) {
	Graphics Engine;
	while( Engine.Running ) {

		Engine.Running = false;
		SDL_Delay( 2000 );
	}

	return 0;
}
