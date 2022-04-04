#include <iostream> 
#include "../ants.cpp" 
#include "../anthill.cpp" 
#include "../food.cpp" 
#include "../map.cpp" 
#include "../regions.cpp" 
#include "../tiles.cpp" 

#define IS_TRUE(x) if(!(x)) { std::cout << __FUNCTION__ < " inadequate behaviour in line " << __LINE__ << std::endl; } 

void testTile(int x, int y) { 
	// the variable `map` is global 
	Tile * tile = map->getTile(x, y); 
} 

int main() { 
	// Instantiate the ants 
	int mapWidth = 5; 
	int mapHeight = 4; 
	int fov = 2; 
	map = new Map(mapWidth, mapHeight, fov); 
	
	// Insert anthills and foods 
	std::string colonyName = "Spartans"; 
	int nAnts = 32; // the anthill's attributes 
	map->insertAnthill(1, 3, colonyName, nAnts); 
	map->insertFood(4, 3, 32); 
	
	std::cout << map->getTile(1, 3) << std::endl; 
	std::cout << map->getTile(1, 9) << std::endl; // it should provide 
	// a nullptr, in this case 
	std::cout << map->getTile(1, 3)->numAnts()[colonyName] << std::endl; 
	map->print(); 

	// Assert tile's methods 
	Tile * tile = map->getTile(1, 3); 
	
} 
