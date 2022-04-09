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

#define HEIGHT 9 
#define WIDTH 9 
#define FOV 1 

const std::string LINES = "++++++++++++++++++++++++++++++++"; 

/**  
* Initialize the anthills, and the foods, in the map `map`.  
* @param Map * map the map in which the game is going to move 
*/  
void initializeGame(Map * map) { 
	// Initialize a pair of anthills 	
	map->insertAnthill(1, 2, "Spartans", 99); 
	map->insertAnthill(8, 1, "Covenant", 45); 

	map->insertFood(4, 8, 32); 
} 

int main() { 
	map = new Map(WIDTH, HEIGHT, FOV); 
	initializeGame(map); 	

	while (true) { 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		map->print(); 
		std::cout << LINES << std::endl; 
	} 
	// map->print(); 
} 
