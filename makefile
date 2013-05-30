# Make file for Island Generation Tool
#
# Compile using this: "make"
# Run using "./main"

all: main
		@echo Compiling the program...
		@echo Use "./main" to run.
main: main.o
		g++ -o main main.o -Wall -ansi -pedantic -O1 -lSDL -lSDL_ttf -lSDL_gfx
main.o: main.cpp
		g++ -c main.cpp -Wall -ansi -pedantic -O1 -lSDL -lSDL_ttf -lSDL_gfx

