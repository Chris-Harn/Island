
Procedural Island Generator
======

![Generated Island](/screenshots/island50.bmp?raw=true)

Procedural island generator for an undisclosed simulation project. Written using C++, SDL, g++, VIM and Screen GNU.  Compiled in g++ on Xbuntu.  

Started May 29th, 2013. Completed June 28th, 2013 by Chris Harn.

## Controls

A new Island will be generated every 3 seconds. If you hit 'Space', the current island will pause for 3 seconds. Pressing 'Enter' will place a bmp of the current island in the screenshots folder.

## Dependencies

These need to be installed:
- SDL 1.2		From here www.libsdl.org/download-1.2.php
- SDL_gfx		From here www.ferzkopp.net/joomla/software-mainmenu-14/4-ferzkopps-linux-software/19-sdlgfx

These files need to be put in the directory:
- SDL.dll		Get from SDL 1.2 download
- sdlgfx.dll 	Get from SDL_gfx download

## Building

To build this project, you need to make sure all the needed files are include in the Island directory: SDL.dll and sdlgfx.dll.

To ompile and run:

    make
    ./main
