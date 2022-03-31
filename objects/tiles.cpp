#include <iostream> 
#include <vector> 
#include <queue> 
#include "ants.cpp" 

class Tile { 
	private: 
		// Tile's coordinates 
		int x; 
		int y; 
	
		// An array containg the arrays in the current tile 
		std::map<std::string, 
			std::queue<Ant>> ants; 
	public: 
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

		void killAnt(Ant * ant) { 
			// The ants from the same colony are indistinguishable 
			std::string colony = ant->colony->name; 

			// And, in particular, we should extract any of them from the 
			// corresponding queue 
			ants[colony].pop(); 
		} 

} 
		
