#include "defines.h"

int main( int argc, char **argv ) {
	Graphics Engine;

	Uint8 board[ 18 ][ 18 ];
	Uint8 board2[ 144 ][ 144 ];

	Engine.Stage1Generation( board );
	Engine.Stage2Generation( board, board2 );
	
	while( Engine.IsRunning() ) {
		Engine.HandleInput();
		Engine.DrawScreen();
	}

	return 0;
}
