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
	printf( "Closing down all systems." );
	SDL_FreeSurface( Window );
	TTF_CloseFont( Font );

	TTF_Quit();
	SDL_Quit();
}
