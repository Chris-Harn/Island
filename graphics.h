#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics {
public:
							Graphics();
							~Graphics();
	void					DrawBackground();
private:
	SDL_Surface*			Window;
	SDL_Surface*			WorkSurface1
	SDL_Surface*			WorkSurface2
	SDL_Surface*			WorkSurface3
	TTF_Font*				Font;
	bool					Running;
};

#endif
