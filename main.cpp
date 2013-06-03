#include "defines.h"

int main( int argc, char **argv ) {
	Graphics Engine;

	Uint8 board[ 18 ][ 18 ];
	Uint8 board2[ 144 ][ 144 ];

	while( Engine.IsRunning() ) {
		Engine.Stage1Generation( board );
		Engine.Stage2Generation( board, board2 );
		Engine.HandleInput();
		Engine.DrawScreen();
		
		SDL_Delay( 1000 );
	}

	return 0;
}
