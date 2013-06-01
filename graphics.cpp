#include "graphics.h"

Graphics::Graphics() {
	
	assert( SDL_Init( SDL_INIT_VIDEO ) != -1 && "SDL_Init failed to initalize." );

	assert( TTF_Init() != -1 && "TTF_Init failed to initalize." );

	Window = SDL_SetVideoMode( 640, 480, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE  );

	assert( Window != NULL && "Window failed to set up correctly." );

	WorkSurface = NULL;

	SDL_WM_SetCaption( "Procedural Island Generation", 0 );
	SDL_ShowCursor( SDL_ENABLE );

	Running = true;
	
	Font = TTF_OpenFont( "arial.ttf", 12 );
	
	assert( Font != NULL && "arial.ttf failed to load." );

	SDL_FillRect( Window, NULL, SDL_MapRGB( Window->format, 0, 0, 0 ) );
	SDL_Flip( Window );
}

Graphics::~Graphics() {
	printf( "Closing down all systems.\n" );
	SDL_FreeSurface( Window );
	SDL_FreeSurface( WorkSurface );
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
						break;
					case SDL_BUTTON_RIGHT:
						printf( "Right Mouse Button was pressed.\n" );
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

void Graphics::Stage1Generation() {
	// Generates a 16 by 16 map that will serve as the base for the island
	// 0 = water, 1 = sand, 2 = grass, 3 = forest, 4 = rock, 5 = mountain
	Uint8 board[ 16 ][ 16 ];
	// 	mUint8 board[ 128 ][ 128 ];

	// Set everything to water
	for( int x = 0; x < 16; x++ ) {
		for( int y = 0; y < 16; y++ ) {
			board[ x ][ y ] = 0;
		}
	}
	
	// Draw the inital 8 by 8 square of grass in the middle
	for( int x = 0; x < 8; x++ ) {
		for( int y = 0; y < 8; y++ ) {
			board[ x + 6 ][ y + 6 ] = 2;
		}
	}		

	Uint32 yellow = SDL_MapRGB( Window->format, 0xff, 0xff, 0x00 );

	/*
		if( SDL_MUSTLOCK( WorkSurface ) ) {
			if( SDL_LockSurface( WorkSurface ) == false ) {
				printf( "Screen was not able to lock." );
				exit( 1 );
			}
		}

		Uint8 yellow = SDL_MapRGB( screen->format, 0xff, 0xff, 0x00 );

		PutPixel( WorkSurface, x, y, yellow );

		if( SDL_MUSTLOCK( WorkSurface ) ) {
			SDL_UnlockSurface( WorkSurface );
		}

		SDL_UpdateRect( screen, x, y, 1, 1 );
	*/
}

void Graphics::PutPixel( SDL_Surface* screen, int x, int y ) {
	Uint8* pixels = ( Uint8* ) screen->pixels;
	Uint8* pixel = pixels + y * screen->pitch + x;
	*pixel = SDL_MapRGB( screen->format, 0xff, 0xff, 0xff ); // make white
}
