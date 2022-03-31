#include <iostream> 
#include <vector> 
#include <queue> 
#include <string> 
#include "ants.cpp" 

// Tile ** tiles; 

class Tile { 
	private: 
		// Tile's coordinates 
		int x; 
		int y; 
	
		// An array containg the arrays in the current tile 
		std::map<std::string, 
			std::queue<Ant*>> ants; 

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
			if (ifFood || isAnthill) 
				return; 
			// Capture ant's colony 
			std::string colony = ant->colony->name; 

			// Check whether there is already an key correspoding to the 
			// colony with name `colony` 
			if (ants.find(colony) == ants.end()) { 
				ants[colony] = new std::queue<Ant*>;  
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
			if (isFood || isAnthill) 
				return; 

			// Extract the ant from the queue 
			Ant * thisAnt = extractAnt(ant); 
			delete thisAnt; 
	
			// Kill the ant 
			delete ant; 
		} 
		
		// Extracting an ant is correlated to killing it; however, we do not delete the instance 
		/**  
		* Extracts an ant for the current tile; it was directed to other tile. 
		* @param Ant * ant the ant that is going to be extracted from this tile 
		*/  
		Ant * extractAnt(Ant * ant) { 
			if (isFood || isAnthill) 
				return NULL; 

			// Identify the ant's colony; ants from the same colony are indistinguishable 
			std::string colony = ant->colony->name; 
			
			// Check whether the colony still contains insects 
			if (ants[colony].size() < 1) 
				delete ants[colony]; 

			return ants[colony].pop(); 
		} 		
		/**  
		* Compute the quantity of ants from each colony in the current tile.  
		*/  
		std::map<std::string, int> numAnts() { 
			if (isFood || isAnthill) 
				return NULL; 
			// Instantiate a map with the quantities of ants for each colony 
			std::map<std::string, int> nAnts; 
			
			// Iterate across each colony in the current tile 
			for (std::map<std::string, std::queue<Ant*>>::iterator iter = ants.begin; 
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
			if (isFood || isAnthill) 
				return NULL; 
			pheromone++; 
		} 	

		std::string print() { 
			// Check the tile's status 
			int nAnts = numAnts(); 
			// Sum the quantity of antos 
			int totalAnts = 1e-19; 
			
			for (std::map<std::string, int>::iterator iter = nAnts.begin(); 
					iter != nAnts.end(); ++iter) { 
				totalAnts += iter->second; 
			} 
			
			std::string tileString = "|" + std::to_string(totalAnts) + "," + 
				std::to_string(pheromone) + "|" 

			return tileString; 
		} 
} 
		
