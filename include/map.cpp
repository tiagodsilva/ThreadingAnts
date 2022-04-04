#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h> 

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
			const int row = i/mapHeight; 
			const int column = i - mapHeight * row; 
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

/**  
* Insert an anthill in the tile with coordinates `x` and `y`.  
* @param int x, int y the tile's coordinates 
* @param std::string anthillName the colony's name 
*/  
void Map::insertAnthill(int x, int y, std::string anthillName) { 
	// Identify the tiles correspondent to the coordinates `x` and `y`
	const int i = y * width + x; 
	Tile * currTile = tiles[i]; 

	if (anthillMap.count(anthillName) == 1) { 
		printf("The anthill with name %s is already in the map!", 
				anthillName.c_str()); 
	} 
	anthillMap[anthillName] = new Anthill(x, y, anthillName); 

	// Modify the vector containing the tiles 
	tiles[i] = new Tile(x, y, anthillName); 
	
	// and allocate the RAM 
	delete currTile; 
} 

/**  
* Insert food in the tile correspondent to the coordinates `x` and `y`.  
* @param int x, int y 
* @param int initialVolume the intiial food's volume  
*/  
void Map::insertFood(int x, int y, int initialVolume) { 
	// Accordingly to the method `insertAnthill`, 
	// identify and modify the container 
	const int i = y * width + x; 
	Tile * currTile = tiles[i]; 
	if (!currTile->isFood) { 
		tiles[i] = new Tile(x, y, true); 
		foods.push_back(new Food(x, y, initialVolume)); 
	} 
	delete currTile; 
} 

/**  
* Identify, using the anthill's name, its characteristics.  
* @param std::string anthillName the anthill's name 
* @return Anthill * anthill the anthill object 
*/  
Anthill * Map::getAnthill(std::string anthillName) { 
	return anthillMap[anthillName]; 
} 
