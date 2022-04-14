#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h> 
#include <stack> 
// #include "tiles.cpp" 
#include "objects.hpp" 
#include "exceptions.hpp"

/**  
* Constructor method for Map.  
* @param mapWidth, mapHeight the map's dimensions 
*/  
Map::Map(int mapWidth, int mapHeight, int fov, int psurvival, int ufood, bool fight)
	: width(mapWidth), height(mapHeight), fov(fov), psurvival(psurvival), 
	ufood(ufood), fight(fight) { 
		// Instantiate an array of tiles 
		int area = mapWidth * mapHeight; 
		// and insert tiles in it 
		for (int i = 0; i < area; i++) { 
			// This is an array with a row major format; 
			// that is, tiles contains `mapWidth` arrays with length equal 
			// to `mapHeight`, 
			// |+++...+++|+++...+++|...|+++...+++| 
			//  mapWidth  mapWidth      mapWidth 
			// (each `mapWidth` tile corresponds to a row).  
			const int row = i/mapHeight; 
			const int column = i - mapHeight * row; 
			tiles.push_back(new Tile(column, row, false)); 
		} 
} 
		
/**  
* Compute the tile from the coordinates x and y.  
* @param int x, y the tile's coordinates 
*/  
Tile * Map::getTile(int x, int y) { 
	// Compute the tile's instance from coordinates x and y 
	if (x >= width || y >= height || x <= -1 || y <= -1) { 
		throw BorderError("The coordinates (" + std::to_string(x) + " ," 
				+ std::to_string(y) + ") are inappropriate!"); 
	} 
	const int i = y * width + x; 
	return tiles[i]; 
} 

/**  
* Compute the neighboring tiles; verify, in particular, that the movement to the foods' tiles 
* is controled by the map.  
* @param int x, int y the coordinates of the tile whose neighbors we are going to compute 
*/  
std::vector<Tile*> Map::neighbors(int x, int y) { 
	// Vector to consolidate the neighborhood tiles 
	std::vector<Tile*> neighborsTiles; 
			
	// Iterate across the tile's neighbors 
	for (int col = -1; col < 2; col++) { 
		// Compute the actual neighbor 
		for (int row = -1; row < 2; row++) { 
			int currX = x + col;  
			int currY = y + row; 
			
			if ((currX == x && currY == y) ||
					(col != 0 && row != 0)) 
				continue; 
					
			try { 
				Tile * currTile = getTile(currX, currY); 
				if (!currTile->isFood) 
					neighborsTiles.push_back(currTile); 
			} catch (BorderError& e) { 
				continue; 
			} 
					
			// neighborsTiles.push_back(currTile); 
		} 
	} 
			
	return neighborsTiles; 
} 


/**  
* Print, in the terminal, the map's state as a text.  
*/   
void Map::print() { 
	std::lock_guard<std::mutex> lk(ioMutex); 
	// Print each tile in row major sequence 
	// and the current quantity of ants for each colony 
	printAnthillsChars(); 
	for (int row = height - 1; row > -1; row--) { 
		for (int col = 0; col < width; col++) { 
			Tile * currTile = getTile(col, row); 
			std::string tileString = currTile->print(); 
			// Send the current string to the terminal 
			std::cout << tileString; 
		} 
		// Then, print the next row 
		std::cout << std::endl; 
	} 
} 

/**  
* Insert an anthill in the tile with coordinates `x` and `y`.  
* @param int x, int y the tile's coordinates 
* @param std::string anthillName the colony's name 
*/  
void Map::insertAnthill(int x, int y, std::string anthillName, int nAnts) { 
	// Identify the tiles correspondent to the coordinates `x` and `y`
	const int i = y * width + x; 
	Tile * currTile = tiles[i]; 

	if (anthillMap.count(anthillName) == 1) { 
		printf("The anthill with name %s is already in the map!", 
				anthillName.c_str()); 
	} 
	anthillMap.insert({anthillName, new Anthill(x, y, anthillName)}); 

	// Modify the vector containing the tiles 
	tiles[i] = new Tile(x, y, anthillName); 
	
	anthillMap[anthillName]->instantiateAnts(nAnts, fov); 

	// and allocate the RAM 
	delete currTile; 
} 

/**  
* Insert food in the tile correspondent to the coordinates `x` and `y`.  
* @param int x, int y 
* @param int initialVolume the intiial food's volume  
* @param int maxAnts the maximum quantity of ants that interact with the food object 
*/  
void Map::insertFood(int x, int y, int initialVolume, int maxAnts) { 
	// Accordingly to the method `insertAnthill`, 
	// identify and modify the container 
	const int i = y * width + x; 
	Tile * currTile = tiles[i]; 
	if (!currTile->isFood) { 
		tiles[i] = new Tile(x, y, true); 
		foods[std::make_pair(x, y)] = new Food(x, y, initialVolume, maxAnts); 
	} 
	delete currTile; 
} 

/**  
* Identify, using the anthill's name, its characteristics.  
* @param std::string anthillName the anthill's name 
* @return Anthill * anthill the anthill object 
*/  
Anthill * Map::getAnthill(std::string anthillName) { 
	return anthillMap[anthillName]; 
} 

/**  
* Compute the food object in a tile; this tile has coordinates `x` and `y`.  
* @param int x, int y the tile's coordinates 
*/  
Food * Map::getFood(int x, int y) { 
	return foods[std::make_pair(x, y)]; 
} 

/**  
* Instantiate a vector with the foods, instances of `Food`, in the map.  
* @return std::vector<Food*> a vector with the foods in the game 
*/  
std::vector<Food*> Map::getFoods() { 
	// Iterate across the map `food`; it is appropriate 
	std::map<std::pair<int, int>, Food*>::iterator iterator; 
	
	std::vector<Food*> foodsVector; 

	for (iterator = foods.begin(); iterator != foods.end(); 
			++iterator) { 
		// food's coordinates 
		// std::pair<int, int> coords = iterator->first; 
		// and the actual instance 
		Food * food = iterator->second; 
		foodsVector.push_back(food); 
	} 

	return foodsVector; 
} 

/**  
* Identify an ant in the tile, in this map, with coordinates `x` and `y`.  
* @param int x, int y the tile's coordinates 
* @param std::string colony the colony's name 
*/  
Ant * Map::getAntInTile(int x, int y, std::string colony) { 
	// Capture the appropriate tile 
	Tile * tile = getTile(x, y); 
	// and an ant from it 
	std::map<std::string, std::stack<Ant*>*> * tileAnts = tile->ants;  

	// Check whether there are ants from this colony in the map 
	if (tileAnts->find(colony) != tileAnts->end()) { 
		std::stack<Ant*> * colonyAnts = tileAnts->find(colony)->second; 
		return colonyAnts->top(); 
	} else { 
		throw AntNotFound("There is no ant in the tile (" + 
				std::to_string(x) + "," + 
				std::to_string(y) + 
				") from the colony " + colony); 
		/* 
		throw AntNotFound("There is no ant in the tile (" + // \ 
				std::to_string(x) + "," + 
				std::to_string(y) + 
				") from the colony " + colony); 
		*/ 
	} 
	
	return NULL; 
} 

/**  
* Contemplate, from any colony, an ant in the tile with coordinates `x` and `y`. 
* @param int x, int y the tiles' coordinates 
*/  
Ant * Map::getAnyAnt() { 
	// Identify the correspondent tile 	
	for (Tile * tile : tiles) { 
		try { 
			Ant * ant = tile->getAnt(); 
			return ant; 
		} catch (AntNotFound& e) { 
			continue; 
		} 

	} 

	throw AntNotFound("There are no ants in the map!"); 
	// and capture an ant 
	// return tile->getAnt();
} 

/**  
* Initializa the ants in the list `allAnts`, a private attribute.  
*/  
void Map::initializeAnts() { 
	// std::lock_guard<std::mutex> lk(mapMutex); 
	// Use the list `allAnts` to gather all the ants 
	allAnts = new std::list<Ant*>; 

	for (Tile * tile : tiles) { 
		// Compute the ants in the current tile 
		std::vector<Ant*> ants = tile->getAnts(); 
		for (Ant * ant : ants) 
			allAnts->push_back(ant); 
	} 

	currAnt = 1e-19; 
	isInitialized = true; 
} 


/**  
* Identify the dead ants; they are killed in the game.  
*/  
void Map::reinitializeAnts() { 
	// std::lock_guard<std::mutex> lk(mapMutex); 
	std::list<Ant*>::iterator it = allAnts->begin(); 

	while (it != allAnts->end()) { 
		bool isDead = (*it)->isDead; 
		if (isDead) { 
			getTile((*it)->getX(), (*it)->getY())->extractAnt((*it)); 
			allAnts->erase(it++); 
		} else { 
			++it; 
		} 
	} 
} 

/**  
* Compute the next ant to play the game; it is convenient for multithreaded programs.  
*/  
Ant * Map::computeNextAnt() { 
	std::unique_lock<std::mutex> lk(mapMutex); 
	if (!isInitialized) 
		throw AntNotFound("The map was not initialized in the game!"); 
	
	// std::list<Ant*>::iterator it = allAnts->begin(); 
	// Check whether the current ant is in a plausible interval 
	// while (currAnt >= allAnts->size()); 
	cv.wait(lk, [this]{return currAnt < allAnts->size();}); 
	std::list<Ant*>::iterator it = allAnts->begin(); 
	std::advance(it, currAnt); 
	currAnt++; 
	return *it; 
} 

/**  
* Check whether all the ants have played; that is, if the `currAnt` equals the `allAnts`'s size.  
* @return bool whether all ants have played the game 
*/  
bool Map::allAntsPlayed() { 
	return currAnt == allAnts->size(); 
} 

// A static member to update the tiles 
/**  
* Update the tiles' attributes; it contemplates a range of these instances.  
* @param int linx, int rinx the boundaries in the vector `map->tiles` we aim to investigate 
*/  
void staticUpdateTiles(int linx, int rinx) { 
	// Apply the correspondent method in the Map's intance
	map->updateTiles(linx, rinx); 
} 

/**  
* Prepare the game for the next iteration; with this objective, we updte the 'currAnt` variable. 
* @param int nThreads the quantity of threads to use in this step 
*/  
void Map::prepareNextIter(int nThreads) { 
	// currAnt = 1e-19; 

	// Compute the quantity of tiles in the game 
	int nTiles = width * height; 

	// and provide a set of tiles to each thread 
	int tilesPerThread = (nTiles + nThreads)/nThreads; 

	// Instantiate tiles to update the map's attributes 
	std::vector<std::thread*> threadsList; 

	for (int i = 0; i < nThreads; i++) { 
		int lTileIndex = i * tilesPerThread; 
		int rTileIndex = (i + 1) * tilesPerThread; 

		// Execute, in each thread, a method to update the tiles 
		threadsList.push_back( 
				new std::thread(&staticUpdateTiles, lTileIndex, rTileIndex) 
		); 
	} 

	// Join the threads; this is important to assert the all tiles are checked 
	for (std::thread * thread : threadsList) { 
		thread->join(); 
			
		// Allocate the threads' data to something else 
		delete thread; 
	} 
	
	map->reinitializeAnts(); 
	currAnt = 1e-19; 
	cv.notify_all(); 

} 

/**  
* Verify whether there is food; it contemplates the tiles in the field of view.  
* @param int x, int y the current tile's coordinates 
*/  
Tile * Map::captureFoodNear(int x, int y) { 
	// Iterate across the viewable neighbors 
	for (int col = -fov; col < fov + 1; col++) { 
		for (int row = -fov; row < fov + 1; row++) { 
			// Compute the current neighbor 
			int currX = x + col; 
			int currY = y + row; 

			// Check if we are in the ant's tile 
			if (currX == x && currY == y) 
				continue; 

			try { 
				Tile * possibleFood = getTile(currX, currY); 
				if (possibleFood->isFood) { 
					Food * food = getFood(currX, currY); 
					if (food->getVolume() >= 1) 
						return possibleFood; 
				} 
			} catch (BorderError& e) { // Assert that the borders are consistent 
				continue; 
			} 
		} 
	} 

	// If the food is not viewable, compute the current tile 
	return getTile(x, y); 
} 

/**  
* Compute the pheromone's lifetime; it is convenient to instante this struct. 
*/  
int Map::getPSurvival() { 
	return psurvival; 
} 

/**  
* Update the tiles' attributes in the game; it checks a range of the vector `tiles`, which is 
* an attribute of this class. 
* @param int lTilesIndex, int rTilesIndex the boundaries of the tiles' vector we aim to update  
*/  
void Map::updateTiles(long unsigned int lTilesIndex, long unsigned int rTilesIndex) { 
	// Iterate across each tile 
	for (long unsigned int i = lTilesIndex; i < rTilesIndex; i++) { 
		// Assert that the tile correspondent to `i` does exist 
		if (i >= tiles.size()) 
			return; 

		// Identify the current tile 
		Tile * currTile = tiles[i]; 

		// and check whether it is tied to a food object 
		if (currTile->isFood) { 
			Food * food = map->getFood(currTile->getX(), currTile->getY()); 
			food->allowAnts(); 

			// The variable `GAME_ITERATION` is global
			if (GAME_ITERATION % ufood == 1) 
				food->restore(); 

		} else { // in this case, it is a tile that contains ants 
		       currTile->checkPheromones(); 
		       currTile->killAnts(); 
	      	} 

	} 
} 

/**  
* Print the quantity of ants in each anthill; it also identifies the their food storage. 
*/  
void Map::printAnthillsChars() { 
	// Iterate across the available anthills 
	std::map<std::string, Anthill*>::iterator it; 

	// Vector to display the quantities 
	std::map<std::string, std::tuple<std::string, int, int>> anthills; 

	for (it = anthillMap.begin(); it != anthillMap.end(); ++it) { 
		// Identify the quantity of food storage 
		Anthill * currAnthill = it->second; 
		int foodStorage = currAnthill->foodStorage; 
		std::string anthillName = currAnthill->getName(); 

		anthills[anthillName] = std::make_tuple(anthillName, foodStorage, 1e-19); 
	} 

	// Compute the quantity of (alive) ants in each anthill 
	std::list<Ant*>::iterator itList; 

	for (itList = allAnts->begin(); itList != allAnts->end(); ++itList) { 
		// Check the colony of the current ant 
		Ant * ant = (*itList); 
		std::string anthillName = ant->getAnthill()->getName(); 

		std::get<2>(anthills[anthillName]) += 1; 
	} 	

	// Print, then, the status of each colony 
	std::cout << "Colonies:" << std::endl; 
	std::cout << "++++" << std::endl; 
	std::cout << "Name" << "\t\t" << "Food" << "\t" << "Ants" << std::endl; 

	const std::string LINES = "++++\t\t++++\t++++\n"; 
	std::cout << LINES; 

	// Map iterator 
	std::map<std::string, std::tuple<std::string, int, int>>::iterator iter; 

	for (iter = anthills.begin(); iter != anthills.end(); ++iter) { 
		const std::tuple<std::string, int, int> colony = iter->second; 
		std::cout << std::get<0>(colony) << "\t"; 
		std::cout << std::get<1>(colony) << "\t"; 
		std::cout << std::get<2>(colony) << std::endl; 
		std::cout << LINES; 
	} 	
} 

