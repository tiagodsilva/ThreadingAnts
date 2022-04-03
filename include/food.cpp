#include <iostream> 
#include <vector> 

#include "headers/objects.hpp" 
/**  
* Constructor method for `Food`.  
* @param Tile * foodTile the tile in which it was placed in the game 
* @param int initVolume the initial volume of the food in this tile 
*/  
Food::Food(Tile * foodTile, int initVolume) 
	: tile(foodTile), initialVolume(initVolume), volume(initVolume) 
	{} 
		
/**  
* Consume the food; it should be designed as in the Philosophers' dilemma 
* (eat or don't eat).  
* @return true if there was food to be consumed; 
* in the other scenario, `false` is the variable. 
*/  
bool Food::consume() { 
	if (volume >= 1) {  
		volume--; 
		return true; 
	} else { 
		return false; 
	} 
} 

/**  
* Restore the initial volume, as asserted in the parameter `initialVolume`.  
*/  
void Food::restore() { 
	volume = initialVolume; 
} 
