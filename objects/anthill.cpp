#include <iostream> 
#include <vector> 
#include <string> 

class Anthill { 
	private: 
		// The colony's name 
		std::string name; 
		// and its correspondent tile 
		Tile * tile; 
	public: 
		// Convenient attributes to describe the anthill's state 
		int totalAnts; // Ants initially released in the game 
		int foodStorage; // The quantity of food in the current colony 

		/**  
		* Constructor method for the `Anthill` object 
		* @param colonyTile the tile correspondent with the colony  
		* @param name the colony's name; it should be an identifier 
		*/  
		Anthill(Tile * colonyTile, std::string colonyName) 
			: name(colonyName), tile(colonyTile) 
			{} 
		


} 
