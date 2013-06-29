#include "graphics.h"

Graphics::Graphics() {
	
	assert( SDL_Init( SDL_INIT_VIDEO ) != -1 && "SDL_Init failed to initalize." );

	assert( TTF_Init() != -1 && "TTF_Init failed to initalize." );

	Window = SDL_SetVideoMode( 576, 576, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE );

	assert( Window != NULL && "Window failed to set up correctly." );

	SDL_WM_SetCaption( "Procedural Island Generation", 0 );
	SDL_ShowCursor( SDL_ENABLE );

	Running = true;
			
	Font = TTF_OpenFont( "arial.ttf", 12 );
	
	assert( Font != NULL && "arial.ttf failed to load." );
	
	srand( time( 0 ) );
	
	FileNum = 50;
}

Graphics::~Graphics() {
	printf( "Closing down all systems.\n" );
	SDL_FreeSurface( Window );
	TTF_CloseFont( Font );

	TTF_Quit();
	SDL_Quit();
	printf( "Everything closed successfully.\n" );
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
					case SDLK_SPACE:
						SDL_Delay( 3000 );
					 	break;		
					case SDLK_RETURN:
						PrintPicture();
						break;
					default:
						break;
				}
			case SDL_MOUSEBUTTONDOWN:
				switch( Event.button.button ) {
					case SDL_BUTTON_LEFT:
						printf( "Left Mouse Button was pressed.\n" );
						printf( "Mouse clicked at pixel X: %d Y: %d\n", Event.motion.x, Event.motion.y );
						break;
					case SDL_BUTTON_RIGHT:
						printf( "Right Mouse Button was pressed.\n" );
						printf( "Mouse clicked at pixel X: %d Y: %d\n", Event.motion.x, Event.motion.y );
						break;
					default:
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

void Graphics::Stage1Generation( Uint8 board[][18] ) {
	// Generates an 18 by 18 map that will serve as the base for the island
	// This map will only have water = 0 and grass = 2 to start out
	
	// Set everything to water
	for( int x = 0; x < 18; x++ ) {
		for( int y = 0; y < 18; y++ ) {
			board[ x ][ y ] = 0;
		}
	}
	
	// Draw the inital 6 by 6 square of grass in the middle
	for( int x = 0; x < 6; x++ ) {
		for( int y = 0; y < 6; y++ ) {
			board[ x + 6 ][ y + 6 ] = 2;
		}
	}		

	// Start Randomly growing the land
	// The more land surrounding it, the more the stand land will spawn
	int xCoord = 5, yCoord = 5, grow = 0;
	int amount;
	int count = 0;

	// Variables that control the island generation
	int seed = 25; 	// Generates a number between 0 and n - 1 // 25
	int number = 24; // Amount the total must be higher than  // 24
	int addition = 7; // Amount added for each land found
	int division = 2; // Amount that the final addition amount is divided by
	int plus = 1; // Amount added to the seed

	for( int i = 0; i < 3; i++ ) {		
		for( int x = 0; x < 8 + grow * 2; x++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; 	m < 3; m++ ) {
					if( board[ xCoord + x - 1 + m ][ yCoord - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ){
				board[ xCoord + x ][ yCoord ] = 2;
			}
			count = 0;
		}

		xCoord += 7 + grow * 2;

		for( int y = 0; y < 8 + grow * 2; y++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; m < 3; m++ ) {
					if( board[ xCoord - 1 + m ][ yCoord + y - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ) {
				board[ xCoord ][ yCoord + y ] = 2;
			}
			count = 0;	
		}

		yCoord += 7 + grow * 2;
		for( int x = 0; x < 8 + grow * 2; x++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; m < 3; m++ ) {
					if( board[ xCoord - x - 1 + m ][ yCoord - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ){
				board[ xCoord - x ][ yCoord ] = 2;
			}
			count = 0;
		}
		xCoord -= 7 +  grow * 2;
		for( int y = 0; y < 8 + grow * 2; y++ ) {
			for( int n = 0; n < 3; n++ ) {
				for( int m = 0; m < 3; m++ ) {
					if( board[ xCoord - 1 + m ][ yCoord - y - 1 + n ] == 2 ) {
						count += addition;
					}
				}
			}
			amount = plus + count / division + rand() % seed;
			if( amount > number ) {
				board[ xCoord ][ yCoord - y ] = 2;
			}
			count = 0;
		}
		yCoord -= 7 + grow * 2;
		xCoord -= 1;
		yCoord -= 1;

		grow += 1;
		count = 0;
	}
}

void Graphics::Stage2Generation( Uint8 board[][18], Uint8 board2[][144] ) {
	// Expand the map from 18x18 to 144 to 144 and give it other types of
	// of land
	// 0 = water, 1 = sand, 2 = grass, 3 = forest, 4 = rock
	Uint8 board3[ 576 ][ 576 ];
	
	// Begin cutting up the island. Cutting out the outside 50% of each
	// grid square
	for( int y = 0; y < 18; y++ ) {
		for( int x = 0; x < 18; x++ ) {

			for( int n = 0; n < 8; n++ ) {
				for( int m = 0; m < 8; m++ ) {
					if( m < 2 || n < 2 || m > 5 || n > 5 ) {
						board2[ m + x * 8 ][ n + y * 8 ] = 0;
					} 
					else {
						board2[ m + x * 8 ][ n + y * 8 ] = board[ x ][ y ];
					}
				}
			}

		}
	}

	// Begin filling in between the now cut up squares	
	// First we draw in the horizontal squares
	for( int y = 0; y < 144; y++ ) {
		for( int x = 2; x < 144; x += 8 ) {
			if( board2[ x ][ y ] == 2 && board2[ x + 8 ][ y ] == 2 ) {
				for( int i = x + 4; i < x + 8; i++ ) {
					board2[ i ][ y ] = 2;
				}
			}				
		}
	}
	
	for( int x = 0; x < 144; x++ ) {
		for( int y = 2; y < 144; y += 8 ) {
			if( board2[ x ][ y ] == 2 && board2[ x ][ y + 8 ] == 2 ) {
				for( int i = y + 4; i < y + 8; i++ ) {
					board2[ x ][ i ] = 2;
				}
			}				
		}
	}
	
	// Variables that control the island generation
	int seed = 40; 	// Generates a number between 0 and n - 1 // 25
	int number = 40; // Amount the total must be higher than  // 24
	int addition = 7; // Amount added for each land found
	int division = 1; // Amount that the final addition amount is divided by
	int plus = 1; // Amount added to the seed
	int count, amount;

	// Once again generate new land based on surrounding pieces of land
	for( int i = 0; i < 3; i++ ) {
		for( int y = 0; y < 144; y++ ) {
			for( int x = 0; x < 144; x++ ) {	
				if( board2[ x ][ y ] == 0 ) {
					count = 0;
					// Count the land in a 3x3 grid of the space
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; 	m < 3; m++ ) {
							if( board2[ x - 1 + m ][ y - 1 + n ] == 2 ) {
								count += addition;
							}
						}
					}
					amount = plus + count / division + rand() % seed;
					if( amount > number ){
						board2[ x ][ y ] = 2;
					}
				}
			}
		}
	}	

	// Clean up the outside portion of the map
	for( int y = 0; y < 144; y++ ) {
		for( int x = 0; x < 144; x++ ) {
			if( x < 14 || y < 14 || x > 129 || y > 129 ) {
				board2[ x ][ y ] = 0;
			}
		}
	}

	// Turn all land that borders water in to sand
	for( int i = 0; i < 3; i++ ) {
		for( int y = 0; y < 144; y++ ) {
			for( int x = 0; x < 144; x++ ) {	
				if( board2[ x ][ y ] == 2 ) {
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; m < 3; m++ ) {
							if( board2[ x - 1 + m ][ y - 1 + n ] == 0 ) {
								board2[ x ][ y ] = 1;
							}
						}
					}
				}
			}
		}
	}	

	// One again generate new sand based on surrouding pieces of sand
	int xStart = 5, yStart = 5;
	int xTotal = 139, yTotal = 139;
	for( int i = 0; i < 72; i++ ) {
		for( int x = xStart; x < xTotal; x++ ) {
			if( board2[ x ][ yStart ] == 2 ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yStart - 1 + n ] == 1 ) {
							amount = 1 + rand() % 100;
							if( amount > 70 ){
								board2[ x ][ yTotal ] = 1;
								// board2[ x ][ yStart ] = 1;
							}
						}
					}
				}
			}
		}

		for( int y = yStart; y < yTotal; y++ ) {
			if( board2[ xTotal ][ y ] == 2 ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xTotal - 1 + m ][ y - 1 + n ] == 1 ) {
							amount = 1 + rand() % 100;							
							if( amount > 85 ) {
								board2[ xTotal ][ y ] = 1;
							}
						}
					}
				}
			}
		}

		for( int x = xTotal; x > xStart; x-- ) {
			if( board2[ x ][ yTotal ] == 2 ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yTotal - 1 + n ] == 1 ) {
							amount = 1 + rand() % 100;	
							if( amount > 70 ) {
								board2[ x ][ yStart ] = 1;
								// board2[ x ][ yTotal ] = 1;
							}	
						}
					}
				}
			}
		}

		for( int y = yTotal; y > yStart; y-- ) {
			if( board2[ xStart ][ y ] == 2 ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xStart - 1 + m ][ y - 1 + n ] == 1 ) {
							amount = 1 + rand() % 100;
							if( amount > 85 ) {
								board2[ xStart ][ y ] = 1;
							}
						}
					}
				}
			}
		}
		
		xStart++;
		xTotal--;
		yStart++;
		yTotal--;
	}

	// Variables that control the sand generation
	seed = 25; 	// Generates a number between 0 and n - 1 
	number = 34; // Amount the total must be higher than 
	addition = 18; // Amount added for each land found
	division = 3; // Amount that the final addition amount is divided by
	plus = 1; // Amount added to the seed

	// Once again generate new sand based on surrounding pieces of land
	for( int i = 0; i < 1; i++ ) {
		for( int y = 0; y < 144; y++ ) {
			for( int x = 0; x < 144; x++ ) {	
				if( board2[ x ][ y ] == 2 ) {
					count = 0;
					// Count the land in a 3x3 grid of the space
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; 	m < 3; m++ ) {
							if( board2[ x - 1 + m ][ y - 1 + n ] == 1 ) {
								count += addition;
							}
						}
					}
					if( count != 0 ) {
						amount = plus + count / division + rand() % seed;
						if( amount > number ){
							board2[ x ][ y ] = 1;
						}
					}
				}
			}
		}
	}	
	
	// Clear the first map to use to record possibities for forest
	for( int y = 0; y < 18; y++ ) {
		for( int x = 0; x < 18; x++ ) {
			board[ x ][ y ] = 0;
		}
	}

	// Now use the first map to record where to place forest
	for( int y = 0; y < 18; y++ ) {
		for( int x = 0; x < 18; x++ ) {

			for( int n = 0; n < 8; n++ ) {
				for( int m = 0; m < 8; m++ ) {
					if( board2[ m + x * 8 ][ n + y * 8 ] == 2 ) {
						board[ x ][ y ] += 1;
					} 
				}
			}

		}
	}
	
	// Variables that control the forest generation
	seed = 25; 	// Generates a number between 0 and n - 1 
	number = 24; // Amount the total must be higher than 
	addition = 7; // Amount added for each land found
	division = 2; // Amount that the final addition amount is divided by
	plus = 1; // Amount added to the seed

	int locationX, locationY;
	int maxX, maxY;
	// Now start drawing the forest
	for( int y = 0; y < 18; y++ ) {
		for( int x = 0; x < 18; x++ ) {
			if( board[ x ][ y ] > 27 && 1 + rand() % 8 > 2 ) {
				locationX = 1 + rand() % 8;
				locationY = 1 + rand() % 8; 
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						board2[ locationX + x * 8 - 1 + m ][ locationY + y * 8 - 1 + n ] = 3;
					}
				}

				// board2[ locationX + x * 8 ][ locationY + y * 8 ] = 3;
				maxX = maxY = 2;	
				for( int t = 0; t < 2 + rand() % 6; t++ ) {
					for( int n = 0; n < maxY + 2; n++ ) {
						for( int m = 0; m < maxX + 2; m++ ) {
							count = 0;
							// Count the land in a 3x3 grid of the space
							for( int q = 0; q < 3; q++ ) {
								for( int w = 0; w < 3; w++ ) {
									if( board2[ locationX + x * 8 -maxX + m + q - 1 ][ locationY + y * 8 - maxY + w - 1 ] == 3 ) {
										count += addition;
									}
								}
							}
							amount = plus + count / division + rand() % seed;
							if( amount > number && board2[ locationX + x * 8 - maxX + m ][ locationY + y * 8 - maxY + n ] != 0 && board2[ locationX + x * 8 - maxX + m ][ locationY + y * 8 - maxY + n ] != 1 ) {
								board2[ locationX + x * 8 - maxX + m ][ locationY + y * 8 - maxY + n ] = 3;
							}
						}
				  	}	
					maxX = maxY += 2;
				}

			} 
		}
	}
	
	// Pass over every piece of forest and get rid of the jagged edges
	// Variables that control the forest generation
	seed = 25; 	// Generates a number between 0 and n - 1 
	number = 40	; // Amount the total must be higher than 
	addition = 7; // Amount added for each land found
	division = 3; // Amount that the final addition amount is divided by
	plus = 1; // Amount added to the seed

	// Once again generate new forest based on surrounding pieces of land
	for( int times = 0; times < 2; times++ ) {
		xStart = 5, yStart = 5;
		xTotal = 139, yTotal = 139;

		for( int i = 0; i < 72; i++ ) {
			for( int x = xStart; x < xTotal; x++ ) {
				if( board2[ x ][ yStart ] == 2 || board2[ x ][ yStart ] == 1 ) {
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; m < 3; m++ ) {
							if( board2[ x - 1 + m ][ yStart - 1 + n ] == 3 ) {
								amount = 1 + rand() % 100;
								if( amount > 75 ){
									board2[ x ][ yStart ] = 3;
								}
							}
						}
					}
				}
			}

			for( int y = yStart; y < yTotal; y++ ) {
				if( board2[ xTotal ][ y ] == 2 || board2[ xTotal ][ y ] == 1 ) {
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; m < 3; m++ ) {
							if( board2[ xTotal - 1 + m ][ y - 1 + n ] == 3 ) {
								amount = 1 + rand() % 100;							
								if( amount > 75 ) {
									board2[ xTotal ][ y ] = 3;
								}
							}
						}
					}
				}
			}

			for( int x = xTotal; x > xStart; x-- ) {
				if( board2[ x ][ yTotal ] == 2 || board2[ x ][ yTotal ] == 1 ) {
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; m < 3; m++ ) {
							if( board2[ x - 1 + m ][ yTotal - 1 + n ] == 3 ) {
								amount = 1 + rand() % 100;	
								if( amount > 75 ) {
									board2[ x ][ yTotal ] = 3;
								}	
							}
						}
					}
				}
			}

			for( int y = yTotal; y > yStart; y-- ) {
				if( board2[ xStart ][ y ] == 2 || board2[ xStart ][ y ] == 1 ) {
					for( int n = 0; n < 3; n++ ) {
						for( int m = 0; m < 3; m++ ) {
							if( board2[ xStart - 1 + m ][ y - 1 + n ] == 3 ) {
								amount = 1 + rand() % 100;
								if( amount > 75 ) {
									board2[ xStart ][ y ] = 3;
								}
							}
						}
					}
				}
			}
			
			xStart++;
			xTotal--;
			yStart++;
			yTotal--;
		}
	}

	// Create Rock Formations to break up the shape of the map
	for( int times = 0; times < 15 + rand() % 15; times++ ) {
		locationX = 16 + rand() % 112;
		locationY = 16 + rand() % 112;

		if( board2[ locationX ][ locationY ] == 1 ) {
			locationX = 16 + rand() % 112;
			locationY = 16 + rand() % 112;
		}

		board2[ locationX ][ locationY ] = 4;
	
		xStart = locationX - 1;
		yStart = locationY - 1;
		xTotal = locationX + 1;
		yTotal = locationY + 1;
	
		for( int expand = 0; expand < 2 + rand() % 8; expand++ ) {
			for( int x = xStart; x < xTotal + 1; x++ ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yStart - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ x ][ yStart ] = 4;
							}
						}
					}
				}
			}
	
			for( int y = yStart; y < yTotal + 1; y++ ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xTotal - 1 + m ][ y - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ xTotal ][ y ] = 4;
							}
						}
					}
				}
	
			}
			
			for( int x = xTotal; x > xStart - 1; x-- ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yTotal - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ x ][ yTotal ] = 4;
							}
						}
					}
				}
			}
	
			for( int y = yTotal; y > yStart - 1; y-- ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xStart - 1 + m ][ y - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ xStart ][ y ] = 4;
							}
						}
					}
				}
	
			}

			xStart--;
			yStart--;
			xTotal++;
			yTotal++;
		}	
	}

	// Create a mountain range in the center
	int choice;
	for( int times = 0; times < 40 + rand() % 20; times++ ) {
		choice = 1 + rand() % 4;
		switch( choice ) {
			case 1:
				locationX = 62 + rand() % 16;
				locationY = 56;
				break;
			case 2:
				locationX = 84;
				locationY = 62 + rand() % 16;
				break;
			case 3:
				locationX = 78 - rand() % 16;
				locationY = 84;
				break;
			case 4:
				locationX = 56;
				locationY = 62 + rand() % 16;
				break;
			default:
				locationX = 14 + rand() % 130;
				locationY = 14 + rand() % 130;
				break;
		}

		board2[ locationX ][ locationY ] = 4;
	
		xStart = locationX - 1;
		yStart = locationY - 1;
		xTotal = locationX + 1;
		yTotal = locationY + 1;
	
		for( int expand = 0; expand < 2 + rand() % 8; expand++ ) {
			for( int x = xStart; x < xTotal + 1; x++ ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yStart - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 90 ) {
								board2[ x ][ yStart ] = 4;
							}
						}
					}
				}
			}
	
			for( int y = yStart; y < yTotal + 1; y++ ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xTotal - 1 + m ][ y - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 90 ) {
								board2[ xTotal ][ y ] = 4;
							}
						}
					}
				}
	
			}
			
			for( int x = xTotal; x > xStart - 1; x-- ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yTotal - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 90 ) {
								board2[ x ][ yTotal ] = 4;
							}
						}
					}
				}
			}
	
			for( int y = yTotal; y > yStart - 1; y-- ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xStart - 1 + m ][ y - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 90 ) {
								board2[ xStart ][ y ] = 4;
							}
						}
					}
				}
	
			}

			xStart--;
			yStart--;
			xTotal++;
			yTotal++;
		}	
	}
	// Fill in holes in rock formations
	for( int y = 16; y < 122; y++ ) {
		for( int x = 16; x < 122; x++ ) {
			if( board2[ x ][ y ] == 4 && board2[ x + 2 ][ y ] == 4 ) {
				board2[ x + 1 ][ y ] = 4;
			}
	
			if( board2[ x ][ y ] == 4 && board2[ x ][ y + 2 ] == 4 ) {
				board2[ x ][ y + 1 ] = 4;
			}
		}
	}

	// Variables that control the rock generation
	seed = 100; 	// Generates a number between 0 and n - 1 
	number = 96 + addition + addition; // Amount the total must be higher than 
	addition = 8; // Amount added for each land found
	division = 1; // Amount that the final addition amount is divided by
	plus = 1; // Amount added to the seed

	// Once again generate new rock based on surrounding pieces of land
	for( int i = 0; i < 5; i++ ) {
		for( int y = 5; y < 134; y++ ) {
			for( int x = 5; x < 134; x++ ) {	
				count = 0;
				// Count the land in a 3x3 grid of the space
				for( int n = 1; n < 4; n += 2 ) {
					for( int m = 1; m < 4; m += 2 ) {
						if( board2[ x - 1 + m ][ y - 1 + n ] == 4 ) {
							count += addition;
						}
					}
				}
				if( count != 0 ) {
					amount = plus + count / division + rand() % seed;
					if( amount > number ){
						board2[ x ][ y ] = 4;
					}
				}
			}
		}
	}	
	
	// Fill in holes in rock formations
	for( int y = 16; y < 122; y++ ) {
		for( int x = 16; x < 122; x++ ) {
			if( board2[ x ][ y ] == 4 && board2[ x + 2 ][ y ] == 4 ) {
				board2[ x + 1 ][ y ] = 4;
			}
	
			if( board2[ x ][ y ] == 4 && board2[ x ][ y + 2 ] == 4 ) {
				board2[ x ][ y + 1 ] = 4;
			}
		}
	}
	
	/*
	// Create Cloud Formations to break up the shape of the map
	for( int times = 0; times < 10 + rand() % 12; times++ ) {
		locationX = 16 + rand() % 112;
		locationY = 16 + rand() % 112;

		board2[ locationX ][ locationY ] = 4;
	
		xStart = locationX - 1;
		yStart = locationY - 1;
		xTotal = locationX + 1;
		yTotal = locationY + 1;
	
		for( int expand = 0; expand < 2 + rand() % 8; expand++ ) {
			for( int x = xStart; x < xTotal + 1; x++ ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yStart - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ x ][ yStart ] = 4;
							}
						}
					}
				}
			}
	
			for( int y = yStart; y < yTotal + 1; y++ ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xTotal - 1 + m ][ y - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ xTotal ][ y ] = 4;
							}
						}
					}
				}
	
			}
			
			for( int x = xTotal; x > xStart - 1; x-- ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ x - 1 + m ][ yTotal - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ x ][ yTotal ] = 4;
							}
						}
					}
				}
			}
	
			for( int y = yTotal; y > yStart - 1; y-- ) {
				for( int n = 0; n < 3; n++ ) {
					for( int m = 0; m < 3; m++ ) {
						if( board2[ xStart - 1 + m ][ y - 1 + n ] == 4 ) {
							if( 1 + rand() % 100 > 80 ) {
								board2[ xStart ][ y ] = 4;
							}
						}
					}
				}
	
			}

			xStart--;
			yStart--;
			xTotal++;
			yTotal++;
		}	
	}

	// Fill in holes in cloud formations
	for( int y = 16; y < 122; y++ ) {
		for( int x = 16; x < 122; x++ ) {
			if( board2[ x ][ y ] == 4 && board2[ x + 2 ][ y ] == 4 ) {
				board2[ x + 1 ][ y ] = 4;
			}
	
			if( board2[ x ][ y ] == 4 && board2[ x ][ y + 2 ] == 4 ) {
				board2[ x ][ y + 1 ] = 4;
			}
		}
	}

	// Variables that control the cloud generation
	seed = 100; 	// Generates a number between 0 and n - 1 
	number = 95 + addition; // Amount the total must be higher than 
	addition = 8; // Amount added for each land found
	division = 1; // Amount that the final addition amount is divided by
	plus = 1; // Amount added to the seed

	// Once again generate new cloud based on surrounding pieces of land
	for( int i = 0; i < 100; i++ ) {
		for( int y = 5; y < 134; y++ ) {
			for( int x = 5; x < 134; x++ ) {	
				count = 0;
				// Count the land in a 3x3 grid of the space
				for( int n = 0; n < 3; n += 2 ) {
					for( int m = 0; m < 3; m += 2 ) {
						if( board2[ x - 1 + m ][ y - 1 + n ] == 4 ) {
							count += addition;
						}
					}
				}
				if( count != 0 ) {
					amount = plus + count / division + rand() % seed;
					if( amount > number ){
						board2[ x ][ y ] = 4;
					}
				}
			}
		}
	}	
	*/

	// Expand the map from 144x144 to 576x576 so that we can get the best
	// picture when it renders.
	// 0 = water, 1 = sand, 2 = grass, 3 = forest, 4 = rock 
	for( int y = 0; y < 144; y++ ) {
		for( int x = 0; x < 144; x++ ) {

			for( int n = 0; n < 4; n++ ) {
				for( int m = 0; m < 4; m++ ) {
					board3[ m + x * 4 ][ n + y * 4 ] = board2[ x ][ y ];
				}
			}

		}
	}


	// Render the new island to screen, pixel by pixel
	if( SDL_MUSTLOCK( Window ) ) {
		assert( SDL_LockSurface( Window ) < 0 );
	}

	for( int x = 0; x < 576; x++ ) {
		for( int y = 0; y < 576; y++ ) {
			switch( board3[ x ][ y ] ) {
				case 0:
					PutPixel( Window, x, y, 54, 73, 255 );
					break;
				case 1:
					PutPixel( Window, x, y, 254, 255, 135 );
					break;
				case 2:
					PutPixel( Window, x, y, 85, 200, 67 );
					break;
				case 3:
					PutPixel( Window, x, y, 7, 159, 15 );
					break;
				case 4:
					PutPixel( Window, x, y, 64, 66, 77 );
					break;
				default:
					break;
			}	
		}
	}
		
	if( SDL_MUSTLOCK( Window ) ) {
		SDL_UnlockSurface( Window );
	}

	SDL_UpdateRect( Window, 0, 0, 100, 100 );
}

void Graphics::PutPixel( SDL_Surface* screen, int x, int y, Uint8 Rb, Uint8 Gb, Uint8 Bb ) {
	Uint8* pixels = ( Uint8* ) screen->pixels;
	Uint8* pixel = pixels + y * screen->pitch + x;
	*pixel = SDL_MapRGB( screen->format, Rb, Gb, Bb );
}

void Graphics::DrawScreen() {
	SDL_Flip( Window );
}

void Graphics::PrintPicture() {
	std::fstream a_file( "./screenshots/count.txt" );
	

	if( !a_file.is_open() ) {
		printf("Could not find the file. Creating count.txt.\n");
		std::ostringstream convert;
		std::ofstream a_file( "./screenshots/count.txt" );
		FileNum = 100;

		convert << FileNum;
		a_file << convert.str();
		a_file.close();
	}
	else {
		a_file.close();
		
		std::string Result;
		printf("Found count.txt. Updating the number.\n");
		a_file >> Result;

		std::istringstream convert( Result );
		convert >> FileNum;

		printf("Found file num: %d.\n", FileNum);
		FileNum += 1;
		a_file.close();

		std::ofstream a_file( "./screenshots/count.txt", std::ios::trunc );
		a_file << FileNum;
		a_file.close();
	}

	std::string saveFile;
	std::ostringstream fileNumber;
	fileNumber << FileNum;
	saveFile = "./screenshots/island" + fileNumber.str() + ".bmp";

	char *NameOfFile = (char*)saveFile.c_str();

	SDL_SaveBMP( Window, NameOfFile );	
	
	printf("New FileNum = %d.\n", FileNum );
	printf("Printed out file %s.\n", NameOfFile );
}
