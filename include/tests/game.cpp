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
#define FOV 1 

const std::string LINES = "++++++++++++++++++++++++++++++++"; 

/**  
* Initialize the anthills, and the foods, in the map `map`.  
* @param Map * map the map in which the game is going to move 
*/  
void initializeGame(Map * map) { 
	// Initialize a pair of anthills 	
	map->insertAnthill(1, 1, "Spartans", 99); 
	map->insertAnthill(WIDTH - 1, 1, "Covenant", 45); 

	map->insertFood(WIDTH - 1, HEIGHT - 1, 32); 
} 

int main() { 
	map = new Map(WIDTH, HEIGHT, FOV); 
	initializeGame(map); 	

	while (true) { 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		Ant * ant = map->getAnyAnt();  
		ant->moveRandomly(); 
		map->print(); 
		std::cout << LINES << std::endl; 
	} 
	// map->print(); 
} 
