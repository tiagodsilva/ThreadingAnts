#include <iostream> 
#include <vector> 

#include <chrono> 
#include <thread> 

#include <string> 

#include "include/headers.h" 

// command line; these are their standard values 
#define NTHREADS std::string("9") 

#define HEIGHT std::string("5")  
#define WIDTH std::string("5")  
#define FOV std::string("2")  

#define ITERATIONS std::string("99")  

#define PSURVIVAL std::string("12") 
#define UFOOD std::string("12") 
#define VFOOD std::string("32") 

#define CFOOD std::string("99") 

/**  
* Initialize the objects, anthills and food, in the game.  
* @param Map * map a pointer to the map in which we designed the game 
* @param int width, int height the map's dimensions 
*/  
void initializeGame(Map * map, int width, int height) { 
	// Initialize the anthills 
	map->insertAnthill(1, 1, "Spartans", 1); 
	
	// and the foods 
	map->insertFood(width - 1, 1, 32); 
	map->insertFood(1, height - 1, 32); 
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


int main(int argc, char *argv[]) { 
	// Instantiate a parser to parse the command line 
	InputParser * parser = new InputParser(argc, argv); 

	// Parse command line 
	
	// number of threads 
	const int nThreads = std::atoi(parser->parse("--nthreads", NTHREADS).c_str()); 

	// map's dimensions 
	const int width = std::atoi(parser->parse("--width", WIDTH).c_str()); 
	const int height = std::atoi(parser->parse("--height", HEIGHT).c_str()); 

	// iterations in the simulation 
	const int iterations = std::atoi(parser->parse("--iters", ITERATIONS).c_str()); 
	
	// pheromone's survival interval 
	const int psurvival = std::atoi(parser->parse("--psurvival", PSURVIVAL).c_str()); 

	// field of view 
	const int fov = std::atoi(parser->parse("--fov", FOV).c_str());

	// the food's restoration rate 
	const int ufood = std::atoi(parser->parse("--ufood", UFOOD).c_str()); 

	// the food's volume 
	const int vfood = std::atoi(parser->parse("--vfood", VFOOD).c_str()); 

	// This instance, `map`, is global 
	map = new Map(width, height, fov, psurvival); 
	initializeGame(map, width, height); 
	const std::string LINES = concatStrings(std::string("+"), width);
	
	// Initial iteration 
	int it = int(1e-19); 

	while (true) { 
		// To update the display, we should control the iterations 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		stage(); 

		if (it % ufood == 1) 
			map->restoreFoods(); 
		it++; 
		GAME_ITERATION++; 
		map->checkPheromones(); 
		map->print(); 
		std::cout << LINES << std::endl; 
	} 

	return EXIT_SUCCESS; 
} 
