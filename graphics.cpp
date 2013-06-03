#include "graphics.h"

Graphics::Graphics() {
	
	assert( SDL_Init( SDL_INIT_VIDEO ) != -1 && "SDL_Init failed to initalize." );

	assert( TTF_Init() != -1 && "TTF_Init failed to initalize." );

	Window = SDL_SetVideoMode( 144, 144, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE );

	assert( Window != NULL && "Window failed to set up correctly." );

	SDL_WM_SetCaption( "Procedural Island Generation", 0 );
	SDL_ShowCursor( SDL_ENABLE );

	Running = true;
	
	Font = TTF_OpenFont( "arial.ttf", 12 );
	
	assert( Font != NULL && "arial.ttf failed to load." );
	
	srand( time( 0 ) );
}

Graphics::~Graphics() {
	printf( "Closing down all systems.\n" );
	SDL_FreeSurface( Window );
	TTF_CloseFont( Font );

	TTF_Quit();
	SDL_Quit();
	printf( "Everything closed successfully.\n" );
}

void Graphics::HandleInput() {
	while( SDL_PollEvent( &Event ) ) {
		switch( Event.type ) {
			case SDL_QUIT:
				Running = false;
				break;
		 	case SDL_KEYDOWN:
				switch( Event.key.keysym.sym ) {
					case SDLK_ESCAPE:
						Running = false;
						break;
					default:
						break;
				}
			case SDL_MOUSEBUTTONDOWN:
				switch( Event.button.button ) {
					case SDL_BUTTON_LEFT:
						printf( "Left Mouse Button was pressed.\n" );
						printf( "Mouse clicked at pixel X: %d Y: %d\n", Event.motion.x, Event.motion.y );
						break;
					case SDL_BUTTON_RIGHT:
						printf( "Right Mouse Button was pressed.\n" );
						printf( "Mouse clicked at pixel X: %d Y: %d\n", Event.motion.x, Event.motion.y );
						break;
					default:
						break;
				}	
			default:
				break;
		}
	}
}

bool Graphics::IsRunning() {
	return Running;
}

void Graphics::Stage1Generation( Uint8 board[][18] ) {
	// Generates an 18 by 18 map that will serve as the base for the island
	// This map will only have water = 0 and grass = 2 to start out
	
	// Set everything to water
	for( int x = 0; x < 18; x++ ) {
		for( int y = 0; y < 18; y++ ) {
			board[ x ][ y ] = 0;
		}
	}
	
	// Draw the inital 6 by 6 square of grass in the middle
	for( int x = 0; x < 6; x++ ) {
		for( int y = 0; y < 6; y++ ) {
			board[ x + 6 ][ y + 6 ] = 2;
		}
	}		

	// Start Randomly growing the land
	// The more land surrounding it, the more the stand land will spawn
	int xCoord = 5, yCoord = 5, grow = 0;
	int amount;
	int count = 0;

	// Variables that control the island generation
	int seed = 24; 	// Generates a number between 0 and n - 1
	int number = 22; // Amount the total must be higher than
	int addition = 20; // Amount added for each land found
	int division = 5; // Amount that the final addition amount is divided by
	int plus = 0; // Amount added to the seed

	for( int i = 0; i < 3; i++ ) {		
		for( int x = 0; x < 8 + grow * 2; x++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; 	m < 3; m++ ) {
					if( board[ xCoord + x - 1 + m ][ yCoord - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ){
				board[ xCoord + x ][ yCoord ] = 2;
			}
			count = 0;
		}

		xCoord += 7 + grow * 2;

		for( int y = 0; y < 8 + grow * 2; y++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; m < 3; m++ ) {
					if( board[ xCoord - 1 + m ][ yCoord + y - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ) {
				board[ xCoord ][ yCoord + y ] = 2;
			}
			count = 0;	
		}

		yCoord += 7 + grow * 2;
		for( int x = 0; x < 8 + grow * 2; x++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; m < 3; m++ ) {
					if( board[ xCoord - x - 1 + m ][ yCoord - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ){
				board[ xCoord - x ][ yCoord ] = 2;
			}
			count = 0;
		}
		xCoord -= 7 +  grow * 2;
		for( int y = 0; y < 8 + grow * 2; y++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; m < 3; m++ ) {
					if( board[ xCoord - 1 + m ][ yCoord - y - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ) {
				board[ xCoord ][ yCoord - y ] = 2;
			}
			count = 0;
		}
		yCoord -= 7 + grow * 2;
		xCoord -= 1;
		yCoord -= 1;

		grow += 1;
		count = 0;
	}
}

void Graphics::Stage2Generation( Uint8 board[][18], Uint8 board2[][144] ) {
	// Expand the map from 18x18 to 144 to 144 and give it other types of
	// of land
	// 0 = water, 1 = sand, 2 = grass, 3 = forest, 4 = rock, 5 = mountain
	for( int y = 0; y < 18; y++ ) {
		for( int x = 0; x < 18; x++ ) {

			for( int n = 0; n < 8; n++ ) {
				for( int m = 0; m < 8; m++ ) {
					board2[ m + x * 8 ][ n + y * 8 ] = board[ x ][ y ];
				}
			}

		}
	}

	if( SDL_MUSTLOCK( Window ) ) {
		assert( SDL_LockSurface( Window ) < 0 );
	}
	
	for( int x = 0; x < 144; x++ ) {
		for( int y = 0; y < 144; y++ ) {
			switch( board2[ x ][ y ] ) {
				case 0:
					PutPixel( Window, x, y, 54, 73, 255 );
					break;
				case 2:
					PutPixel( Window, x, y, 85, 200, 67 );
					break;
				default:
					break;
			}	
		}
	}
		
	if( SDL_MUSTLOCK( Window ) ) {
		SDL_UnlockSurface( Window );
	}

	SDL_UpdateRect( Window, 0, 0, 100, 100 );
}

void Graphics::PutPixel( SDL_Surface* screen, int x, int y, Uint8 Rb, Uint8 Gb, Uint8 Bb ) {
	Uint8* pixels = ( Uint8* ) screen->pixels;
	Uint8* pixel = pixels + y * screen->pitch + x;
	*pixel = SDL_MapRGB( screen->format, Rb, Gb, Bb );
}

void Graphics::DrawScreen() {
	SDL_Flip( Window );
}
