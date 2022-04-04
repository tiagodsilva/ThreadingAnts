#include <iostream> 
#include <vector> 
#include <string> 

// #include "tiles.cpp" 
#include "headers/objects.hpp" 
/**  
* Constructor method for Map.  
* @param mapWidth, mapHeight the map's dimensions 
*/  
Map::Map(int mapWidth, int mapHeight) 
	: width(mapWidth), height(mapHeight) { 
		// Instantiate an array of tiles 
		int area = mapWidth * mapHeight; 
		// and insert tiles in it 
		for (int i = 0; i < area; i++) { 
			// This is an array with a row major format; 
			// that is, tiles contains `mapWidth` arrays with length equal 
			// to `mapHeight`, 
			// |+++...+++|+++...+++|...|+++...+++| 
			//  mapWidth  mapWidth      mapWidth 
			// (each `mapWidth` tile corresponds to a row).  
			const int row = i/mapWidth; 
			const int column = i - mapWidth * row; 
			tiles.push_back(new Tile(column, row, false)); 
		} 
} 
		
/**  
* Compute the tile from the coordinates x and y.  
* @param int x, y the tile's coordinates 
*/  
Tile * Map::getTile(int x, int y) { 
	// Compute the tile's instance from coordinates x and y 
	if (x >= width || y >= height || x <= -1 || y <= -1) 
		return NULL; 

	const int i = y * width + x; 
	return tiles[i]; 
} 

std::vector<Tile*> Map::neighbors(int x, int y, 
		int xLeftOffset, 
		int yBottomOffset, 
		int xRightOffset, 
		int yTopOffset) { 
	// Vector to consolidate the neighborhood tiles 
	std::vector<Tile*> neighborsTiles; 
			
	// Iterate across the tile's neighbors 
	for (int col = xLeftOffset; col < xRightOffset; col++) { 
		// Compute the actual neighbor 
		for (int row = yBottomOffset; row < yTopOffset; row++) { 
			int currX = x + col;  
			int currY = y + row; 

			if (currX == x && currY == y) 
				continue; 
					
			Tile * currTile = getTile(currX, currY); 
			if (!currTile) 
				continue; 
					
			neighborsTiles.push_back(currTile); 
		} 
	} 
			
	return neighborsTiles; 
} 

/**  
* Print, in the terminal, the map's state as a text.  
*/   
void Map::print() { 
	// Print each tile in row major sequence 
	for (int row = 0; row < height; row++) { 
		for (int col = 0; col < width; col++) { 
			Tile * currTile = getTile(col, row); 
			std::string tileString = currTile->print(); 
			// Send the current string to the terminal 
			std::cout << tileString; 
		} 
		// Then, print the next row 
		std::cout << std::endl; 
	} 
} 

