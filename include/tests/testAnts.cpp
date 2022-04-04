#include <iostream> 
#include "../ants.cpp" 
#include "../anthill.cpp" 
#include "../food.cpp" 
#include "../map.cpp" 
#include "../regions.cpp" 
#include "../tiles.cpp" 

#define IS_TRUE(x) if(!(x)) { std::cout << __FUNCTION__ < "inadequate behaviour in line " << __LINE__ << std::endl; } 

void testTile(int x, int y) { 
	// the variable `map` is global 
	std::string colonyName = "Spartans"; 
	Tile * tile = map->getTile(x, y); 
} 

int main() { 
	// Instantiate the ants 
	int mapWidth = 5; 
	int mapHeight = 4; 
	map = new Map(mapWidth, mapHeight); 

	std::cout << map->getTile(1, 3) << std::endl; 

	map->print(); 

	// Assert tile's methods 
	Tile * tile = map->getTile(1, 3); 
	
} 
