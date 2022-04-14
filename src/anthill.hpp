#include <iostream> 
#include <vector> 
#include <string> 

#include "objects.hpp" 

/**  
* Constructor method for the `Anthill` object 
* @param colonyTile the tile correspondent with the colony  
* @param name the colony's name; it should be an identifier 
*/  
Anthill::Anthill(int x, int y, std::string colonyName) 
	: name(colonyName), x_pos(x), y_pos(y)   
	{} 
		
/**  
* Instantiate `ants` ants in the game. 
* @param int ants 
* @param fov the ant's field of view 
*/  	
void Anthill::instantiateAnts(int ants) { 
	if (isInitialized) 
		return; 
	initialAnts = ants; 
	// Insert ants in the tile 
	Tile * thisTile = map->getTile(x_pos, y_pos); 
	for (int i = 0; i < initialAnts; i++) { 
		thisTile->insertAnt(new Ant(x_pos, y_pos, this)); 
	} 
	
	std::cout << thisTile << std::endl; 
	isInitialized = true; 
} 

/**  
* Increment the anthill's food; it should be cautious to simultaneuous access.  
*/  
void Anthill::incrementFood() { 
	std::unique_lock<std::mutex> foodMutex;
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
* Compute the x coordinate of the anthill, which is correspondent to the horizontal 
* axis.  
* @return int x_pos the x coordinate 
*/  
int Anthill::getX() { 
	return x_pos; 
} 

/**  
* Identify the y coordinate of the anthill, a pointer to the vertical axis.  
* @return int y_pos the y coordinate 
*/  
int Anthill::getY() { 
	return y_pos; 
} 
