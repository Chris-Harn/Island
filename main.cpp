#include "defines.h"

int main( int argc, char **argv ) {
	Graphics Engine;
	while( Engine.IsRunning() ) {

		Engine.HandleInput();
	}

	return 0;
}
