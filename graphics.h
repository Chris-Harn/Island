#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defines.h"

class Graphics {
public:
							Graphics();
							~Graphics();
	void					DrawBackground();
	bool					Running;
private:
	SDL_Surface*			Window;
	SDL_Surface*			WorkSurface;
	TTF_Font*				Font;
};

#endif
