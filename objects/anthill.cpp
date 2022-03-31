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
} 
