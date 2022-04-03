#include <iostream> 
#include<map> 
#include<vector> 
#include<queue> 

class Ant; 

class Tile { 
	private: 
		// Tile's coordinates 
		int x; 
		int y; 

		// Array with the ants in this tile for each colony 
		std::map<std::string, std::queue<Ant*>> ants; 

		// Ramifications of this tile 
		bool isAnthill; 
		bool isFood; 

	public: 
		int pheromone; // the intensity of pheromone in these coordinates 

		Tile(int x_tile, int y_tile, bool containsAnthill, bool containsFood); 
		void insertAnt(Ant * ant); 
		void killAnt(Ant * ant); 
		Ant * extractAnt(Ant * ant); 
		std::map<std::string, int> numAnts(); 
		void incrementPheromone(); 
		std::string print(); 
}; 
