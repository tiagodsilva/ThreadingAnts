#include <iostream> 
#include "../ants.cpp" 
#include "../anthill.cpp" 
#include "../food.cpp" 
#include "../map.cpp" 
#include "../regions.cpp" 
#include "../tiles.cpp" 

int main() { 
	// Instantiate the ants 
	int mapWidth = 45; 
	int mapHeight = 32; 
	map = new Map(mapWidth, mapHeight); 

	std::cout << map->getTile(1, 3) << std::endl; 
} 
