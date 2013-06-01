#include "graphics.h"

Graphics::Graphics() {
	if( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
		printf( "SDL_INIT_VIDEO failed %s\n", SDL_GetError() );
		exit( 1 );
	}

	if( TTF_Init() == -1 ) {
		printf( "TTF_Init failed to initalize.\n" );
		exit( 1 );
	}

	Window = SDL_SetVideoMode( 640, 480, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE  );

	if( Window == NULL ) {
		printf( "Window failed to set up correctly.\n" );
		exit( 1 );
	}

	SDL_WM_SetCaption( "Procedural Island Generation", 0 );
	SDL_ShowCursor( SDL_ENABLE );

	Running = true;
	
	Font = TTF_OpenFont( "arial.ttf", 12 );
}

Graphics::~Graphics() {
	printf( "Closing down all systems.\n" );
	SDL_FreeSurface( Window );
	TTF_CloseFont( Font );

	TTF_Quit();
	SDL_Quit();
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
				}
			case SDL_MOUSEBUTTONDOWN:
				switch( Event.button.button ) {
					case SDL_BUTTON_LEFT:
						printf( "Left Mouse Button was pressed.\n" );
						break;
					case SDL_BUTTON_RIGHT:
						printf( "Right Mouse Button was pressed.\n" );
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
	
	// Draw the inital 8 by 8 square in the middle
	for( int x = 0; x < 8; x++ ) {
		for( int y = 0; y < 8; y++ ) {
			board[ x + 6 ][ y + 6 ] = 2;
		}
	}		

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
