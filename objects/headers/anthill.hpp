#include <iostream> 
#include <vector> 
#include <string> 

class Anthill { 
	private: 
		// the colony's name and coordinates, sequentially 
		std::string name; 
		Tile * tile; 
		
		// Whether the ants were inserted in the game 
		bool isInitialized = false; 
	public: 
		int initialAnts; 
		int foodStorage; 

		// Methods 
		Anthill(Tile * colonyTile, std::string colonyName); 
		void instantiateAnts(int numAnts); 
		void incrementFood(); 
}; 

