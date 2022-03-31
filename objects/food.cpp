#include <iostream> 
#include <vector> 

class Food { 
	private: 
		Tile * tile; // the tile in which the food was placed in the game 
		int volume; // the amount of volume in this food; it is measured in arbitrary units 

		int initialVolume; // The initial volume of food in this tile; 
		// it is appropriate for the restoration 
	public: 
		/**  
		* Constructor method for `Food`.  
		* @param Tile * foodTile the tile in which it was placed in the game 
		* @param int initVolume the initial volume of the food in this tile 
		*/  
		Food(Tile * foodTile, int initVolume) 
			: tile(foodTile), initialVolume(initVolume), volume(initVolume) 
			{} 
		
			
} 
