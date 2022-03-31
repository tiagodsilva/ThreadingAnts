#include <iostream> 
#include <vector> 
#include <queue> 
#include "ants.cpp" 

Tile ** tiles; 

class Tile { 
	private: 
		// Tile's coordinates 
		int x; 
		int y; 
	
		// An array containg the arrays in the current tile 
		std::map<std::string, 
			std::queue<Ant>> ants; 

		// Attribute to assert whether the tile contains an anthill or it is 
		// a food coordinates 
		bool isAnthill; 
		bool isFood; 
	public: 
		// A set of attributes should be public 
		int pheromone; // The quantity of pheromones 
		
		/**  
		* Constructor method for `Tile`.  
		* @param int x_tile, y_tile the tile's coordinates 
		*/  
		Tile(int x_tile, int y_tile, bool containsAnthill = false, 
				bool containsFood = false) 
			: x(x_tile), y(y_tile), isAnthill(containsAnthill), isFood(containsFood)  
			{} 
	
		/**  
		* Insert an ant in the array `ants`; we should be carefull to 
		* multithreading access.  
		* @param Ant ant the ant to be inserted in the tile 
		*/  
		void insertAnt(Ant * ant) { 
			// Capture ant's colony 
			std::string colony = ant->colony->name; 

			// Check whether there is already an key correspoding to the 
			// colony with name `colony` 
			if (ants.find(colony) == ants.end()) { 
				ants[colony] = new std::queue<Ant>;  
			} else { 
				ants[colony].push(ant); 
			} 
		} 
		
		/**  
		* Kill an ant; as it contemplates shared data structures, it should control 
		* multiple accesses.  
		* @param Ant * ant the killed ant 
		*/  
		void killAnt(Ant * ant) { 
			// The ants from the same colony are indistinguishable 
			std::string colony = ant->colony->name; 

			// And, in particular, we should extract any of them from the 
			// corresponding queue 
			ants[colony].pop(); 

			if (ants[colony].size() < 1) { 
				delete ants[colony]; 
			} 
		} 
		
		/**  
		* Compute the quantity of ants from each colony in the current tile.  
		*/  
		std::map<std::string, int> numAnts() { 
			// Instantiate a map with the quantities of ants for each colony 
			std::map<std::string, int> nAnts; 
			
			// Iterate across each colony in the current tile 
			for (std::map<std::string, std::queue<Ant>>::iterator iter = ants.begin; 
					iter != ants.end(); ++iter) { 
				std::string colony = iter->first; 
				// Compute the quantity of ants 
				int currAnts = (iter->second).size(); 

				nAnts[colony] = currAnts; 
			} 
			// Compute the map 			
			return nAnts; 
		} 
			
		/**  
		* Increment the pheromone's density in this tile. 
		*/  
		void incrementPheromone() { 
			pheromone++; 
		} 	
} 
		
