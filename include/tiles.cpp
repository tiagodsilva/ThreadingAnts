#include <iostream> 
#include <vector> 
#include <stack> 
#include <string> 
// #include "ants.cpp" 
// #include "map.cpp" 
// Tile ** tiles; 

#include "headers/objects.hpp" 
#include "headers/exceptions.hpp" 
/**  
* Constructor method for `Tile`.  
* @param int x_tile, y_tile the tile's coordinates 
*/ 
Tile::Tile(int x_tile, int y_tile, bool containsFood) 
	: x(x_tile), y(y_tile), isAnthill(false), isFood(containsFood), 
	pheromones(new std::list<Pheromone*>) 	
	{
		ants = new std::map<std::string, std::stack<Ant*>*>; 
	} 

/**  
* Constructor method for `Tile`; it contains an anthill.  
* @param int x_tile, int y_tile the tile's coordinates 
* @param std::string anthillName the colony's name of the tile's anthill 
*/  
Tile::Tile(int x_tile, int y_tile, std::string anthillName) 
	: x(x_tile), y(y_tile), isAnthill(true), anthillName(anthillName), 
	isFood(false), pheromones(new std::list<Pheromone*>) 
 	{
		ants = new std::map<std::string, std::stack<Ant*>*>; 
	} 

/**  
* Insert an ant in the array `ants`; we should be carefull to 
* multithreading access.  
* @param Ant ant the ant to be inserted in the tile 
*/  
void Tile::insertAnt(Ant * ant) { 
	if (isFood) 
		throw BorderError("This tile is not suitable for movement!"); 

	// Capture ant's colony 
	std::string colony = ant->getAnthill()->getName(); 

	// Check whether there is already an key correspoding to the 
	// colony with name `colony` 
	if (ants->find(colony) == ants->end()) { 
		ants->insert({colony, new std::stack<Ant*>});  
		ants->find(colony)->second->push(ant); 
	} else { 
		ants->find(colony)->second->push(ant); 
	} 
} 
		
/**  
* Kill an ant; as it contemplates shared data structures, it should control 
* multiple accesses.  
* @param Ant * ant the killed ant 
*/  
void Tile::killAnt(Ant * ant) { 
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
Ant * Tile::extractAnt(Ant * ant) { 
	// Identify the ant's colony; ants from the same colony are indistinguishable 
	std::string colony = ant->getAnthill()->getName(); 
	
	// Compute an ant 
	Ant * currAnt = ants->find(colony)->second->top(); 
	ants->find(colony)->second->pop(); 
	return currAnt; 
} 		

/**  
* Compute the quantity of ants from each colony in the current tile.  
*/  
std::map<std::string, int> Tile::numAnts() {  
	// Instantiate a map with the quantities of ants for each colony 
	std::map<std::string, int> nAnts; 

	// Iterate across each colony in the current tile 
	for (std::map<std::string, std::stack<Ant*>*>::iterator iter = ants->begin(); 
			iter != ants->end(); ++iter) { 
		std::string colony = iter->first; 
		// Compute the quantity of ants 
		int currAnts = (iter->second)->size(); 

		nAnts.insert({colony, currAnts}); 
	} 
	// Compute the map 			
	return nAnts; 
} 
			
/**  
* Increment the pheromone's density in this tile. 
*/  
void Tile::incrementPheromone() { 
	pheromones.push_back(new Pheromone(map->psurvival, GAME_ITERATION)); 
} 	

std::string Tile::print() { 

	if (isFood) { 
		// Identify the food properties of this tile 
		Food * food = map->getFood(x, y); 
		int foodVolume = food->getVolume(); 
		return "|" + std::string("Food") + "," + 
			std::to_string(foodVolume) + "|"; 
	} 

	if (isAnthill) { 
		Anthill * currAnthill = map->getAnthill(anthillName); 
		int foodStorage = currAnthill->foodStorage; 
		return "|" + std::string(anthillName) + "," + 
			std::to_string(foodStorage) + "|"; 
	} 

	// Check the tile's status 
	std::map<std::string, int> nAnts = numAnts(); 
	// Sum the quantity of antos 
	int totalAnts = 1e-19; 
			
	for (std::map<std::string, int>::iterator iter = nAnts.begin(); 
			iter != nAnts.end(); ++iter) { 
		totalAnts += iter->second; 
	} 
			
	std::string tileString = "|" + std::to_string(totalAnts) + "," + 
		std::to_string(pheromones.size()) + "|"; 

	return tileString; 
} 

/**  
* Compute the tile's coordinates.  
* @return int x the tile's horizontal coordinate 
*/  
int Tile::getX() { 
	return x; 
} 

/**  
* Identify the tile's coordinates. 
* @return int y the tile's vertical coordinate  
*/  
int Tile::getY() { 
	return y; 
} 		

/**  
* Check whether the tiles are distinct; use, as parameter of distinction, the coordinates. 
* @param Tile tile_l, Tile tile_r the tiles that we are going to confront 
* @return bool if the tiles are distinct  
* 
*/  
bool operator!=(Tile tile_l, Tile tile_r) { 
	return tile_l.getX() != tile_r.getX() || tile_l.getY() != tile_r.getY(); 
} 

/**  
* Cativates, from any colony, an ant.  
*/  
Ant * Tile::getAnt() { 
	// Identify the ants in this tile 
	std::map<std::string, std::stack<Ant*>*>::iterator iter; 
	for (iter = ants->begin(); iter != ants->end(); ++iter) { 
		std::stack<Ant*> * currAnts = iter->second; 

		if (!currAnts->empty()) 
			return currAnts->top(); 
		else 
			continue;  
	} 
	// Inform, in this scenario, that there are no ants in this tile 
	throw AntNotFound("There are no ants in the tile (" + 
			std::to_string(x) + "," + std::to_string(y) + "!"); 
} 

/** 
* Identify all ants in the current tile; we should traverse a stack. 
*/  
std::vector<Ant*> Tile::getAnts() { 
	// Instantiate a continer for the ants 
	std::vector<Ant*> * vecAnts = new std::vector<Ant*>; 		
	
	// and iterate across the map `ants` 
	std::map<std::string, std::stack<Ant*>*>::iterator iter; 
	for (iter = ants->begin(); iter != ants->end(); ++iter) { 
		std::stack<Ant*>* stackAnts = iter->second; 
		traverseStack(stackAnts, vecAnts); 
	} 

	return *vecAnts; 
} 

/**  
* Traverse a stack; it is convenient when the quantity of stack operations (top, push) 
* is in effect larger than the quantity of vector operations (push), as in our scenario.  
* @param std::stack<Ant*>* the stack we aim to traverse 
* @param std::vector<Ant*>* the vector in which we are going to place the stack's components 
*/  
void Tile::traverseStack(std::stack<Ant*>* st, std::vector<Ant*>* vt) { 
	// Check the initial condition 
	if (st->empty()) 
		return; 
	
	Ant * ant = st->top(); 
	vt->push_back(ant); 
	st->pop(); 
	traverseStack(st, vt); 
	st->push(ant); 
} 
