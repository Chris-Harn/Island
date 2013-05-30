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
private:
	SDL_Surface*			Window;
	SDL_Surface*			WorkSurface;
	TTF_Font*				Font;
	SDL_Event				Event;
	bool					Running;
};

#endif
