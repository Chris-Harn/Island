# Make file for Island Generation Tool
#
# Compile using this: "make"
# Run using "./main"

all: main
		@echo Compiling the program...
		@echo Use "./main" to run.
main: main.o graphics.o
		g++ -o main main.o graphics.o -Wall -ansi -pedantic -O1 -lSDL -lSDL_ttf -lSDL_gfx
main.o: main.cpp
		g++ -c main.cpp -Wall -ansi -pedantic -O1 -lSDL -lSDL_ttf -lSDL_gfx
graphics.o: graphics.cpp
		g++ -c graphics.cpp graphics.h -ansi -pedantic -O1 -lSDL -lSDL_ttf -lSDL_gfx -Wall -Wno-switch

clean:
		-rm -f *.o *.gch .*.swn .*.swo *.out *.txt *.swp main
