#include <iostream> 
#include <vector> 

#include <chrono> 
#include <thread> 
#include <mutex> 

#include <string> 
#include <tuple> 
#include <ncurses.h>

#include "headers.hpp" 

// command line; these are their standard values 
#define NTHREADS std::string("5") // quantity of threads  
#define WIDTH std::string("5") // map's width 
#define HEIGHT std::string("5") // map's height 
#define FOV std::string("2") // ant's field of view 
#define ITERATIONS std::string("99") // quantity of iterations in the game 
#define PSURVIVAL std::string("12") // pheromone's lifetime 
#define UFOOD std::string("45") // food's update rate 
#define CFOOD std::string("3") // quantity of ants that eat the food conveniently 
#define FIGHT std::string("1") // Whether the ants should fight each other 

// Integer quantities 
int nThreads, width, height, 
    iterations, psurvival, fov, 
    ufood, cfood, fight; 

// Introduce, for the colonies, the quantities (x, y, nAnts); for the foods, 
// (x, y, volume), as those are parametrizable  
#define COLONIES std::string("4,4,32:1,4,23")  
#define FOODS std::string("2,1,32:4,1,45") 

std::vector<std::tuple<int, int, int>> colonies, foods; 

std::mutex iterMutex; 
/**  
* Parse the command line sent to the compiled file; it set the values that conform 
* the game. 
* @param InputParser * parser the command line parser 
*/  
void parse(InputParser * parser) { 
	// Check the preamble to describe the variable's names 
	nThreads = parser->parseInt("--nthreads", NTHREADS);
	width = parser->parseInt("--width", WIDTH); 
	height = parser->parseInt("--height", HEIGHT); 
	iterations = parser->parseInt("--iters", ITERATIONS); 
	psurvival = parser->parseInt("--psurvival", PSURVIVAL); 
	fov = parser->parseInt("--fov", FOV); 
	ufood = parser->parseInt("--ufood", UFOOD);
	cfood = parser->parseInt("--cfood", CFOOD); 
	colonies = parser->parseVector<int>("--colonies", COLONIES); 
	foods = parser->parseVector<int>("--foods", FOODS); 
	fight = parser->parseInt("--fight", FIGHT); 
} 

/**  
* Initialize the objects, anthills and food, in the game.  
* @param Map * map a pointer to the map in which we designed the game 
* @param int width, int height the map's dimensions 
*/  
void initializeGame(Map * map, int width, int height) { 
	// Initialize the anthills 
	int currAnthill = 0; 
	for (std::tuple<int, int, int> colony : colonies) { 
		int x = std::get<0>(colony); 
		int y = std::get<1>(colony); 
		int nAnts = std::get<2>(colony); 
		map->insertAnthill(x, y, "Spartans" + std::to_string(currAnthill + 1), nAnts); 
		currAnthill++; 
	} 
	// and the foods 
	
	for (std::tuple<int, int, int> food : foods) { 
		int x = std::get<0>(food); 
		int y = std::get<1>(food); 
		int volume = std::get<2>(food); 
		map->insertFood(x, y, volume, cfood); 
	} 

	// Compute the iterator for the vector that contemplates the ants 
	map->initializeAnts(); 
} 

/**  
* Play a stage, in the map `map`, of the game.  
*/  
void stage() { 
	// Play the game sequentially 
	while (!map->allAntsPlayed()) { 
		Ant * ant = map->computeNextAnt(); 
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

/**  
* Play a multithread game; it sequentially captures the next ant.  
*/  
void multithreadStage() { 
	// Identify the next ant
	while (GAME_ITERATION < iterations) { 
		Ant * ant = map->computeNextAnt();  
		ant->stage(); 
	} 
} 

/**  
* Play the game sequentially; the map should be initialized for the consistency of this 
* iterative procedure. 
*/  
void sequentialGame() { 
	// While the current iteration is plausible, play the game 
	map->print(); 
	const std::string LINES = concatStrings("+", width); 
	
	while (GAME_ITERATION < iterations) { 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		stage(); 
		GAME_ITERATION++; 
		// map->checkPheromones(); 
		map->print(); 
		map->prepareNextIter(nThreads); 
		std::cout << LINES << std::endl; 
	} 
} 

/**  
* Play the game with multiple threads; the map should, also, be initialized to start the procedures. 
*/  
void multithreadGame() { 
	// Play the game with multiple threads 
	std::vector<std::thread*> threadList; 

	// Instantiate the threads 
	for (int i = 0; i < nThreads; i++) 
		threadList.push_back(new std::thread(multithreadStage)); 
	
	const std::string LINES = concatStrings("+", width); 

	std::cout << LINES << "Initialization" << std::endl;
	while (GAME_ITERATION < iterations) { 	
					// User input 
		if (map->allAntsPlayed()) {
		 	// std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
			// map->checkPheromones(); 
			map->print(); 
			map->prepareNextIter(nThreads); 

			std::cout << LINES << GAME_ITERATION << std::endl; 
			GAME_ITERATION++; 
		} 	

		initscr(); 
		noecho(); 
		timeout(299); 
		int ch = getch(); 
		if (ch == 10) { 
			while (getch() != 10); 
		} 
		refresh(); 
		endwin(); 
	} 
} 

int main(int argc, char *argv[]) { 
	// Instantiate a parser to parse the command line 
	InputParser * parser = new InputParser(argc, argv); 
	
	parse(parser); 
	// This instance, `map`, is global 
	map = new Map(width, height, fov, psurvival, ufood, 
			(bool)(fight));
	initializeGame(map, width, height); 
	
	map->print(); 

	// sequentialGame(); 
	multithreadGame(); 

	return EXIT_SUCCESS; 
} 
