#include "graphics.h"

Graphics::Graphics() {
	if( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
		printf( "SDL_INIT_VIDEO failed %s\n", SDL_GetError() );
	}

	if( TTF_Init() == -1 ) {
		printf( "TTF_Init failed to initalize.\n" );
	}

	Window = SDL_SetVideoMode( windowWidth, windowHeight, 8, SDL_HWSURFACE | SDL_DOUBLEBUF );

	if( Window == NULL ) {
		printf( "Window failed to set up correctly.\n" );
	}

	SDL_MW_SetCaption( caption, 0 );
	SDL_ShowCursor( SDL_ENABLE );

	Running = true;
	
	font = TTF_OpenFont( "arial.ttf", 12 );
}

Graphics::~Graphics() {
	SDL_FreeSurface( Window );
	TTF_CloseFont( font );

	TTF_Quit();
	SDL_Quit();
}
