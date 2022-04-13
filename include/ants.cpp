#include <iostream> 
#include <vector> 
// #include "tiles.cpp" 

#include "headers/objects.hpp" 
#include "headers/geometry.hpp" 

#define MAX_INT 2147483647 

/**  
* Constructor method for `Ant`.  
* @param int x, y the ant's coordinates in the map 
* @param Anthill colony the colony to which the colony's pertains  
* @param Map map the map in which the game unfolds; it is, now, a convenient attribute
*/  
Ant::Ant(int x, int y, Anthill * colony, int fieldOfView) 
	: x_pos(x), y_pos(y), antHill(colony), fov(fieldOfView) 
	{} 
/**  
* Execute a movement for the current ant.  
* @param x, y the coordinates 
*/  
void Ant::move(int x, int y) { 
	// Identify the ant's current tile 
	Tile * tile = map->getTile(x_pos, y_pos); 
	// and extract this ant from it 
	tile->extractAnt(this); 
	x_pos += x; 
	y_pos += y; 
	
	// Insert the ant in the other tile 
	Tile * nextTile = map->getTile(x_pos, y_pos); 
	nextTile->insertAnt(this); 
} 
		
/**  
* Kill this ant; it dies and is extracted from the current tile.  
*/  	
void Ant::die() { 
	// Identify ant's tile 
	Tile * tile = map->getTile(x_pos, y_pos); 
	tile->killAnt(this); 
} 

void Ant::eat(Food * food) { 
	// should be synchronized 
	hasFood = food->consume(); 
} 

/**  
* Compute the ants coordinates 
* @return int x the horizontal coordinate 
*/  
int Ant::getX() { 
	return x_pos; 
} 

/**  
* Compute the ants coordinates. 
* @return int y the vertical coordinate  
*/  
int Ant::getY() { 
	return y_pos; 
} 

/**  
* Increment the pheromone's intensity in the current ant's tile; 
* a map would be maybe convenient 
*/  
void Ant::releasePheromone() { 
	Tile * currTile = map->getTile(x_pos, y_pos); 
	currTile->incrementPheromone(); 
} 

void Ant::moveToColony() { 
	// Identify the ant's colony 
	// and its coordinates 
	const int xColony = antHill->getX(); 
	const int yColony = antHill->getY(); 
	// Compute, then, the ant's tile's neighbors 
	std::vector<Tile*> neighbors = map->neighbors(x_pos, y_pos); 
			
	// Line through the ant's coordinates and the colony's 
	Vec antVec = Vec(x_pos, y_pos); 
	Vec colonyVec = Vec(xColony, yColony); 
			
	moveInSegment(antVec, colonyVec, neighbors); 

	// If the current tile is, now, equal to the colony's tile and the ant 
	// has food, then deposit it 
	if (x_pos == xColony && y_pos == yColony && hasFood) { 
		antHill->incrementFood(); 
		hasFood = false; 
	} 
} 

/**  
* Shift a ant across a segment, which is characterized by a pair of vectors.  
* @param Vec v, w the vectors subjacent to the segment 
* @param std::vector<Tile> neighbors the list of neighbors of the ant 
*/  
void Ant::moveInSegment(Vec v, Vec w, std::vector<Tile*> neighbors) { 
	// Move across the segment that underlies the convex combinations of the 
	// vectors `v` and `w` 
	float currDist = MAX_INT; 
	Vec moveDirection = Vec(int(1e-13), int(1e-13)); 
	for (Tile * neighbor : neighbors) { 
		int x = neighbor->getX(); 
		int y = neighbor->getY(); 

		Vec neighborVec = Vec(x, y); 
		float distanceToLine = distanceToSegment(v, w, neighborVec); 

		if (distanceToLine < currDist) { 
			currDist = distanceToLine; 
			moveDirection = neighborVec; 
		} 
	} 
	
	// Identify the movement direction 
	int xDir = moveDirection.x - x_pos; 
	int yDir = moveDirection.y - y_pos; 
	move(xDir, yDir); 
} 

/**  
* Check whether there is a food object contemplated in the field of view.  
*/  
Tile * Ant::hasFoodNear() { 
	// Instantiate the neighbors in the field of view 
	Tile * neighbor = map->captureFoodNear(x_pos, y_pos); 
	// Provide a pointer to this tile 
	return neighbor; 		
} 

/**  
* Assert the existence of food in the field of view and, in this case, move to it.  
*/  
bool Ant::moveToFood() { 
	// Verify if there is food near 
	Tile * neighbor = hasFoodNear(); 

	int xNeighbor = neighbor->getX(); 
	int yNeighbor = neighbor->getY(); 

	// If the taxicab distance between the ant and the food is unitary, then 
	// it is edible 
	int distanceToFood = abs<int>(xNeighbor - x_pos) + abs<int>(yNeighbor - y_pos); 
	if (distanceToFood == 1) { 
		// Assert the positiveness of the food's volume 
		Food * food = map->getFood(xNeighbor, yNeighbor); 
		int volume = food->getVolume(); 
		eat(food); 
		// If this function points positively to true, then the random 
		// movement is postponed; in this scenario, we want that either 
		// (1) the ant ate the food and, so, it awaits for the next iteration
		// (which implies hasFood = true); 
		// (2) the ant did not eat the food, as the seats were 
		// already occupied, and, so, it also awaits for the next iteration; 
		// (3) or the ant did not eat, as the food's volume equals null, so 
		// the ant should move randomly. 
		// In (1), hasFood = true; in (2), hasFood = false, whereas 
		// (presumably) food->getVolume() >= 1; and, in (3), hasFood = false 
		// and food->getVolume() < 1. Equivalently, this function should 
		// compute hasFood || food->getVolume() >= 1. 
		return (hasFood || volume >= 1); 
	} 

	Tile * currTile = map->getTile(x_pos, y_pos); 
	if (neighbor != currTile) { 
		int x = neighbor->getX(); 
		int y = neighbor->getY(); 
		Vec antVec = Vec(x_pos, y_pos); 
		Vec foodVec = Vec(x, y); 

		// Ant's neighbors 
		std::vector<Tile*> neighbors = map->neighbors(x_pos, y_pos); 
				
		// Identify and apply the food's direction 
		moveInSegment(antVec, foodVec, neighbors); 
		return true; 
	} 
		
	// Notify that no movement was executed; this is convenient to move randomly 
	return false; 
} 

/**  
* Identify the ant's anthill.  
*/  
Anthill * Ant::getAnthill() { 
	return antHill; 
} 

/**  
* Move randomly across the map; a square is choosen with probability proportional to the 
* quantity of pheromones in it. 
*/  
void Ant::moveRandomly() { 
	// Capture the ant's neighbors 
	std::vector<Tile*> thisNeighbors = map->neighbors(x_pos, y_pos); 
	int nNeighbors = thisNeighbors.size(); 
 
	// Instantiate an array with the tile's neighbors pheromones 
	int pheromones[nNeighbors]; 

	for (int i = 0; i < nNeighbors; i++) { 
		pheromones[i] = thisNeighbors[i]->pheromone + 1; 
	} 
	
	int randomTile = weightedRandom<int>(pheromones, nNeighbors); 
	
	// Compute the tile's coordinates 
	Tile * currTile = thisNeighbors[randomTile];
	move(currTile->getX() - x_pos, currTile->getY() - y_pos); 
} 

/**  
* Execute a game stage for this ant; it either moves to a specific object or randomly.  
*/  
void Ant::stage() { 
	// A triplet of actions is available 
	// in each game stage: move randomly, 
	// eat or move to a specific object 
	
	// In this scenario, the ant moves to the colony 
	if (hasFood) { 
		releasePheromone(); 
		moveToColony(); 
		// moveRandomly(); 
		return; 
	} 
	

	bool foodInFOV = moveToFood(); 

	// Check whether the ant should fight; it is an attribute 
	bool protectFood = shouldFight; 

	if (protectFood) { // In this case, the food's volume is positive, 
			// and the ant categorically fight 
		bool fought = fight(); 
		return; 
	} 

	// Toss a coin to decide whether to fight or to flee 
	int coin = tossACoin(); 

	if (coin == 1) 
		bool fought = fight(); 

	if (!fought) // If it did not fight, there are no enemies for war 
		moveRandomly(); 
} 

/**  
* Execute a fight; it will possibly culminate in death.  
*/  
bool Ant::fight() { 
	// Toss a coin to decide whether to fight 

	if (!shouldFight) // should not fight 
		return false; 
	
	std::map<std::string, int> enemies = map->getTile(x_pos, y_pos)->numAnts(); 
	
	int nColonies = enemies.size(); 
	
	// std::cout << nColonies << std::endl; 
	if (nColonies == 1) // There are no enemies in this tile 
		return false; 

	std::map<std::string, int>::iterator it = enemies.begin(); 

	// Compute the quantity of enemies 
	double nEnemies[nColonies]; 
	
	std::string thisAnthill = antHill->getName(); 

	for (int i = 0; i < nColonies; i++) { 
		nEnemies[i] = 1/(it->second + 1); // The probability of death is inversely proportional to 
							// the quantity of enemies 
		if (it != enemies.end()) 
		       ++it; 	
	} 
	
		// Execute a fight 
	int dead = weightedRandom<double>(nEnemies, nColonies);  
	
	// Compute the anthill's name correspondent to the dead ant 
	it = enemies.begin(); 
	std::advance(it, dead); 
	std::string anthillName = it->first; 

	if (anthillName == thisAnthill) { 
		// die 
	} else { 
		// kill 
	} 
	return true; 

} 
