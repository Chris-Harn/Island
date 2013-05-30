#include "defines.h"

int main( int argc, char **argv ) {
	Graphics Engine;

	Engine.Stage1Generation();
	
	while( Engine.IsRunning() ) {

		Engine.HandleInput();
	}

	return 0;
}
