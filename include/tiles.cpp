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
	: x(x_tile), y(y_tile), pheromones(new std::list<Pheromone*>), 
	isAnthill(false), isFood(containsFood), pheromone(1e-19) 
	{
		ants = new std::map<std::string, std::stack<Ant*>*>; 
	} 

/**  
* Constructor method for `Tile`; it contains an anthill.  
* @param int x_tile, int y_tile the tile's coordinates 
* @param std::string anthillName the colony's name of the tile's anthill 
*/  
Tile::Tile(int x_tile, int y_tile, std::string anthillName) 
	: x(x_tile), y(y_tile),  pheromones(new std::list<Pheromone*>), 
	isAnthill(true), anthillName(anthillName), isFood(false), 
	pheromone(1e-19) 
 	{
		ants = new std::map<std::string, std::stack<Ant*>*>; 
	} 

/**  
* Insert an ant in the array `ants`; we should be carefull to 
* multithreading access.  
* @param Ant ant the ant to be inserted in the tile 
*/  
void Tile::insertAnt(Ant * ant) { 
	std::lock_guard<std::mutex> lk(tileMutex); 
	if (isFood) 
		throw BorderError("This tile is not suitable for movement!"); 
	
	// std::lock_guard<std::mutex> lk(tileMutex); 
	// tileMutex.lock(); 
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
	// tileMutex.unlock(); 
} 
		
/**  
* Kill an ant; as it contemplates shared data structures, it should control 
* multiple accesses.  
* @param Ant * ant the killed ant 
*/  
void Tile::killAnt(Ant * ant) { 
	if (isFood || isAnthill) 
		return; 

	tileMutex.lock(); 
	// Extract the ant from the queue 
	Ant * thisAnt = extractAnt(ant); 
	delete thisAnt; 
	
	// Kill the ant 
	delete ant; 
	tileMutex.unlock(); 
} 
		
// Extracting an ant is correlated to killing it; however, we do not delete the instance 
/**  
* Extracts an ant for the current tile; it was directed to other tile. 
* @param Ant * ant the ant that is going to be extracted from this tile 
*/  
Ant * Tile::extractAnt(Ant * ant) { 
	std::lock_guard<std::mutex> lk(tileMutex); 
	// std::lock_guard<std::mutex> lk(deathMutex); 
	// Identify the ant's colony; ants from the same colony are indistinguishable 
	std::string colony = ant->getAnthill()->getName(); 
	
	// Compute an ant 
	Ant * currAnt = ants->find(colony)->second->top(); 
	ants->find(colony)->second->pop(); 

	if (ants->find(colony)->second->size() < 1) 
		ants->erase(ants->find(colony)); 

	return currAnt; 
} 		

/**  
* Compute the quantity of ants from each colony in the current tile.  
*/  
std::map<std::string, int> Tile::numAnts() {  
	std::lock_guard<std::mutex> lk(tileMutex); 
	// Instantiate a map with the quantities of ants for each colony 
	std::map<std::string, int> nAnts; 

	// Iterate across each colony in the current tile 
	for (std::map<std::string, std::stack<Ant*>*>::iterator iter = ants->begin(); 
			iter != ants->end(); ++iter) { 
		std::string colony = iter->first; 
		// Compute the quantity of ants 
		int currAnts = (iter->second)->size(); 

		if (currAnts >= 1) 
			nAnts.insert({colony, currAnts}); 
	} 
	// Compute the map 			
	return nAnts; 
} 
			
/**  
* Increment the pheromone's density in this tile. 
*/  
void Tile::incrementPheromone() { 
	std::lock_guard<std::mutex> lk(pherMutex); 
	pheromones->push_back(new Pheromone(map->getPSurvival(), GAME_ITERATION)); 
	pheromone = pheromones->size(); 
} 	

std::string Tile::print() { 

	if (isFood) { 
		// Identify the food properties of this tile 
		Food * food = map->getFood(x, y); 
		int foodVolume = food->getVolume(); 
		return "|" + std::string("Food") + "," + 
			std::to_string(foodVolume) + "|"; 
	} 

	// Check the tile's status 
	std::map<std::string, int> nAnts = numAnts(); 
	// Sum the quantity of antos 
	int totalAnts = 1e-19; 
			
	for (std::map<std::string, int>::iterator iter = nAnts.begin(); 
			iter != nAnts.end(); ++iter) { 
		totalAnts += iter->second; 
	} 
	
	if (isAnthill) { 
		Anthill * currAnthill = map->getAnthill(anthillName); 
		int foodStorage = currAnthill->foodStorage; 
		return "|" + std::string(anthillName) + "," + 
			std::to_string(foodStorage) + "," + std::to_string(totalAnts) + "|"; 
	} 

	std::string tileString = "|" + std::to_string(totalAnts) + "," + 
		std::to_string(pheromone) + "|"; 

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
	std::lock_guard<std::mutex> lk(tileMutex); 
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
	std::lock_guard<std::mutex> lk(tileMutex); 
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

/**  
* Capture the phermones that are beyond their lifetime; they should be extracted from the 
* list `pheromones`.  
*/  
void Tile::checkPheromones() { 
	std::lock_guard<std::mutex> lk(pherMutex); 
	// Check the pheromones in the current tile 
	std::list<Pheromone*>::iterator it = pheromones->begin(); 

	while (it != pheromones->end()) { 
		bool dead = (*it)->kill(GAME_ITERATION); 

		// Assert that the pheromone is within its lifetime 
		if (dead) { 
			pheromones->erase(it++); 
			pheromone = pheromones->size();   
		} else {  
			++it; 
		} 
	} 
} 

/**  
* Increment the quantity of deaths in an anthill; the correspondent stack 
* is going to be modified in the next game iteration. 
* @param std::string anthill the anthill's name 
*/  
void Tile::incrementDeaths(std::string anthill) { 
	std::lock_guard<std::mutex> lk(deathMutex); 
	// Update the quantity of deaths for this anthill 
	if (deaths.find(anthill) == deaths.end()) { 
		deaths[anthill] = 1; 
	} else { 
		deaths[anthill] += 1; 
	} 
} 

/**  
* Apply the map `deaths` to iteratively kill ants in this tile.  
*/  
void Tile::killAnts() { 
	std::lock_guard<std::mutex> lk(deathMutex); 
	// Sequentially kill the ants in each anthill 
	std::map<std::string, int>::iterator it; 

	for (it = deaths.begin(); it != deaths.end(); ++it) { 
		// Capture the quantity of deaths 
		std::string anthillName = it->first; 
		int nDeaths = it->second; 

		// and kill the ants 
		std::stack<Ant*> * pStack = ants->find(anthillName)->second; 

		for (int death = 0; death < min<int>(nDeaths, pStack->size()); death++) { 
			Ant * deadAnt = pStack->top(); 
			deadAnt->isDead = true; 
			// pStack->pop();  
		} 

		deaths[anthillName] = 1e-19; 
	} 
} 

