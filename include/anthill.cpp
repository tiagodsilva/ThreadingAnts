#include <iostream> 
#include <vector> 
#include <string> 

#include "headers/objects.hpp" 

/**  
* Constructor method for the `Anthill` object 
* @param colonyTile the tile correspondent with the colony  
* @param name the colony's name; it should be an identifier 
*/  
Anthill::Anthill(Tile * colonyTile, std::string colonyName) 
	: name(colonyName), tile(colonyTile) 
	{} 
		
/**  
* Instantiate `ants` ants in the game. 
* @param int ants 
*/  	
void Anthill::instantiateAnts(int ants) { 
	if (isInitialized) 
		return NULL; 
	initialAnts = ants; 
	// Insert ants in the tile 
	for (int i = 0; i < initalAnts; i++) { 
		tile.insertAnt(new Ant(tile->x, tile->y, this)); 
	} 

	isInitialized = true; 
} 

/**  
* Increment the anthill's food; it should be cautious to simultaneuous access.  
*/  
void Anthill::incrementFood() { 
	foodStorage++; 
} 

/**  
* Capture the anthill's name.  
* @return std::string name the anthill name 
*/  
std::string Anthill::getName() { 
	return name; 
} 

/**  
* Compute the anthill's tile.  
* @return Tile * tile the anthill's tile in the game 
*/  
Tile * getTile() { 
	return tile; 
} 
