#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h> 
#include <stack> 
// #include "tiles.cpp" 
#include "headers/objects.hpp" 
#include "headers/exceptions.hpp"

/**  
* Constructor method for Map.  
* @param mapWidth, mapHeight the map's dimensions 
*/  
Map::Map(int mapWidth, int mapHeight, int fov) 
	: width(mapWidth), height(mapHeight), fov(fov) { 
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
	if (x >= width || y >= height || x <= -1 || y <= -1) { 
		throw BorderError("The coordinates (" + std::to_string(x) + " ," 
				+ std::to_string(y) + ") are inappropriate!"); 
	} 
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
	for (int col = xLeftOffset; col < xRightOffset + 1; col++) { 
		// Compute the actual neighbor 
		for (int row = yBottomOffset; row < yTopOffset + 1; row++) { 
			int currX = x + col;  
			int currY = y + row; 

			if (currX == x && currY == y) 
				continue; 
					
			try { 
				Tile * currTile = getTile(currX, currY); 
				neighborsTiles.push_back(currTile); 
			} catch (BorderError& e) { 
				continue; 
			} 
					
			// neighborsTiles.push_back(currTile); 
		} 
	} 
			
	return neighborsTiles; 
} 

/**  
* Print, in the terminal, the map's state as a text.  
*/   
void Map::print() { 
	// Print each tile in row major sequence 
	for (int row = height - 1; row > -1; row--) { 
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
void Map::insertAnthill(int x, int y, std::string anthillName, int nAnts) { 
	// Identify the tiles correspondent to the coordinates `x` and `y`
	const int i = y * width + x; 
	Tile * currTile = tiles[i]; 

	if (anthillMap.count(anthillName) == 1) { 
		printf("The anthill with name %s is already in the map!", 
				anthillName.c_str()); 
	} 
	anthillMap.insert({anthillName, new Anthill(x, y, anthillName)}); 

	// Modify the vector containing the tiles 
	tiles[i] = new Tile(x, y, anthillName); 
	
	anthillMap[anthillName]->instantiateAnts(nAnts, fov); 

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
		foods[std::make_pair(x, y)] = new Food(x, y, initialVolume); 
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

/**  
* Compute the food object in a tile; this tile has coordinates `x` and `y`.  
* @param int x, int y the tile's coordinates 
*/  
Food * Map::getFood(int x, int y) { 
	return foods[std::make_pair(x, y)]; 
} 

/**  
* Instantiate a vector with the foods, instances of `Food`, in the map.  
* @return std::vector<Food*> a vector with the foods in the game 
*/  
std::vector<Food*> Map::getFoods() { 
	// Iterate across the map `food`; it is appropriate 
	std::map<std::pair<int, int>, Food*>::iterator iterator; 
	
	std::vector<Food*> foodsVector; 

	for (iterator = foods.begin(); iterator != foods.end(); 
			++iterator) { 
		// food's coordinates 
		std::pair<int, int> coords = iterator->first; 
		// and the actual instance 
		Food * food = iterator->second; 
		foodsVector.push_back(food); 
	} 

	return foodsVector; 
} 

/**  
* Identify an ant in the tile, in this map, with coordinates `x` and `y`.  
* @param int x, int y the tile's coordinates 
* @param std::string colony the colony's name 
*/  
Ant * Map::getAntInTile(int x, int y, std::string colony) { 
	// Capture the appropriate tile 
	Tile * tile = getTile(x, y); 
	// and an ant from it 
	std::map<std::string, std::stack<Ant*>*> * tileAnts = tile->ants;  

	// Check whether there are ants from this colony in the map 
	if (tileAnts->find(colony) != tileAnts->end()) { 
		std::stack<Ant*> * colonyAnts = tileAnts->find(colony)->second; 
		return colonyAnts->top(); 
	} else { 
		throw AntNotFound("There is no ant in the tile (" + \ 
				std::to_string(x) + "," + 
				std::to_string(y) + 
				") from the colony " + colony); 
	} 
	
	return NULL; 
} 

/**  
* Contemplate, from any colony, an ant in the tile with coordinates `x` and `y`. 
* @param int x, int y the tiles' coordinates 
*/  
Ant * Map::getAnyAnt() { 
	// Identify the correspondent tile 	
	for (Tile * tile : tiles) { 
		try { 
			Ant * ant = tile->getAnt(); 
			return ant; 
		} catch (AntNotFound& e) { 
			continue; 
		} 

	} 

	throw AntNotFound("There are no ants in the map!"); 
	// and capture an ant 
	// return tile->getAnt();
} 

/**  
* Restore all Food `objects` in the map; their volume is restored to the initial value.  
*/  
void Map::restoreFoods() { 
	// Capture the foods in the game 
	std::vector<Food*> foods = getFoods(); 

	// and restore them 
	for (int i = 0; i < foods.size(); i++) 
		foods[i]->restore(); 
} 

/**  
* Identify all the ants in the game; it is suitable to execute a stage, in which all ants should act. 
*/  
std::pair<std::vector<Ant*>::iterator, 
	std::vector<Ant*>::iterator> getAllAnts() { 
		// Iterate across the tiles in the game 
		std::vector<Ant*> * antsInGame = new std::vector<Ant*>; 
		for (Tile * tile : tiles) { 
			// Identify the ants in the current tile 
			std::vector<Ant*> ants = tile->getAnts(); 
			for (ant : ants) 
				antsInGame->push_back(ant); 
		} 

		return std::make_pair(antsInGame->begin(), antsInGame->end()); 
} 
