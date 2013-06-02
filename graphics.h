#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defines.h"

class Graphics {
public:
							Graphics();
							~Graphics();
	void					DrawBackground();
	void					HandleInput();
	bool					IsRunning();
	void					Stage1Generation( Uint8 board[][18] );
	void					Stage2Generation( Uint8 board[][18], Uint8 board2[][144] );
	void					PutPixel( SDL_Surface* screen, int x, int y, Uint8 Rb, Uint8 Gb, Uint8 Bb );
	void					DrawScreen();
private:
	SDL_Surface*			Window;
	TTF_Font*				Font;
	SDL_Event				Event;
	bool					Running;
};

#endif
