#include <iostream> 
#include <vector> 

class Tile; 

class Food { 
	private: 
		Tile * tile; // the tile in which the food is in the game 
		int volume; // the amount of volume 

		int initialVolume; // the initial volume of food in this tile; 
		// it is appropriate to restore the volume 
	public: 
		// methods 
		Food(Tile * foodTile, int initVolume); 
		consume(); 
		restore(); 
};  
