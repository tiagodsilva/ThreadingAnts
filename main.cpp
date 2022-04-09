#include <iostream> 
#include <vector> 

#include <chrono> 
#include <thread> 

#include <string> 

#include "include/headers.h" 

#define HEIGHT 5 
#define WIDTH 5 
#define FOV 2 

/**  
* Initialize the objects, anthills and food, in the game.  
* @param Map * map a pointer to the map in which we designed the game 
*/  
void initializeGame(Map * map) { 
	// Initialize the anthills 
	map->insertAnthill(1, 1, "Spartans", 1); 
	
	// and the foods 
	map->insertFood(WIDTH - 1, 1, 32); 
	map->insertFood(1, HEIGHT - 1, 32); 
} 

/**  
* Play a stage, in the map `map`, of the game.  
*/  
void stage() { 
	// Capture pointers to the ants 
	std::pair<std::vector<Ant*>::iterator, 
		std::vector<Ant*>::iterator> ants = map->getAllAnts(); 
	
	// and lead them to their actions 
	for (; ants.first != ants.second; ++ants.first) { 
		Ant * ant = *(ants.first); 
		ant->stage(); 
	} 
} 

/**  
* Concat a string with itself; equivalent to `string` * `integer` in Python.  
* @param std::string string the string that is going to be concatenated 
* @param int concats the quantity of concatenations 
*/  
std::string concatStrings(const std::string string, int concats) { 
	// Concat the string `string` with itself 
	std::string concatElement = std::string(); 
	for (int i = 0; i < concats; i++) 
		concatElement += string; 
	return concatElement; 
} 

int main() { 
	// This instance, `map`, is global 
	map = new Map(WIDTH, HEIGHT, FOV); 
	initializeGame(map); 

	const std::string LINES = concatStrings(std::string("+"), WIDTH); 

	while (true) { 
		// To update the display, we should control the iterations 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		stage(); 
		map->print(); 
		std::cout << LINES << std::endl; 
	} 

	return EXIT_SUCCESS; 
} 
