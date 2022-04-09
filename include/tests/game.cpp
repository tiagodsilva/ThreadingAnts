#include <iostream> 
#include <vector> 
#include <map> 
#include <stack> 

#include <chrono> 
#include <thread> 

#include<string> 

#include "../ants.cpp" 
#include "../anthill.cpp" 
#include "../food.cpp" 
#include "../map.cpp" 
#include "../regions.cpp" 
#include "../tiles.cpp" 

#define HEIGHT 5 
#define WIDTH 5 
#define FOV 3  

#define MAXIT 9 

const std::string LINES = "++++++++++++++++++++++++++++++++"; 

/**  
* Initialize the anthills, and the foods, in the map `map`.  
* @param Map * map the map in which the game is going to move 
*/  
void initializeGame(Map * map) { 
	// Initialize a pair of anthills 	
	map->insertAnthill(1, 1, "Spartans", 1); 
	// map->insertAnthill(WIDTH - 1, 1, "Covenant", 4); 

	map->insertFood(WIDTH - 1, HEIGHT - 1, 32); 
} 

/**  
* Move all ants in the map; the movement is random.  
*/  
void moveAnts() { 
	std::pair<std::vector<Ant*>::iterator, 
		std::vector<Ant*>::iterator> ants = map->getAllAnts(); 

	for (; ants.first != ants.second; ++ants.first) { 
		Ant * ant = *(ants.first); 
		ant->stage(); 
	} 
} 

/**  
* Execute a movement for a ant.  
*/  
void moveAnt(int iteration) { 
	Ant * ant = map->getAnyAnt(); 
	ant->moveRandomly(); 
} 

int main() { 
	map = new Map(WIDTH, HEIGHT, FOV); 
	initializeGame(map); 	
	
	int it = 1e-19; 
	while (true) { 
		// To update the display 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		moveAnts(); 
		// moveAnt(it); 
		it++; 
		map->print(); 
		std::cout << LINES << std::endl; 
	} 
	// map->print(); 
} 
