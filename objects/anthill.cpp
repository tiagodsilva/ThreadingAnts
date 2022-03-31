#include <iostream> 
#include <vector> 
#include <string> 

class Anthill { 
	private: 
		// The colony's name 
		std::string name; 
		// and its correspondent tile 
		Tile * tile; 

		bool isInitialized = false; 
	public: 
		// Convenient attributes to describe the anthill's state 
		int initialAnts; // Ants initially released in the game 
		int foodStorage; // The quantity of food in the current colony 

		/**  
		* Constructor method for the `Anthill` object 
		* @param colonyTile the tile correspondent with the colony  
		* @param name the colony's name; it should be an identifier 
		*/  
		Anthill(Tile * colonyTile, std::string colonyName) 
			: name(colonyName), tile(colonyTile) 
			{} 
		
		/**  
		* Instantiate `ants` ants in the game. 
		* @param int ants 
		*/  	
		void instantiateAnts(int * ants) { 
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
		void incrementFood() { 
			foodStorage++; 
		} 
};
