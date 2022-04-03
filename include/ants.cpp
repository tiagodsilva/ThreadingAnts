#include <iostream> 
#include <vector> 
// #include "tiles.cpp" 

#include "headers/ants.hpp" 
#include "headers/anthill.hpp" 
#include "../utils/geometry.cpp" 

#define MAX_INT 2147483647 

/**  
* Constructor method for `Ant`.  
* @param int x, y the ant's coordinates in the map 
* @param Anthill colony the colony to which the colony's pertains  
* @param Map map the map in which the game unfolds; it is, now, a convenient attribute
*/  
Ant::Ant(int x, int y, Anthill colony, Map * antsMap, int fieldOfView) 
	: x_pos(x), y_pos(y), antHill(colony), map(antsMap), fov(fieldOfView) 
	{} 
/**  
* Execute a movement for the current ant.  
* @param x, y the coordinates 
*/  
void Ant::move(int x, int y) { 
	// Identify the ant's current tile 
	Tile tile = map->getTile(x_pos, y_pos); 
	// and extract this ant from it 
	tile.extractAnt(this); 
	x_pos += x; 
	y_pos += y; 
	// Insert the ant in the other tile 
	Tile nextTile = map->getTile(x_pos, y_pos); 
	nextTile.insertAnt(this); 
} 
		
/**  
* Kill this ant; it dies and is extracted from the current tile.  
*/  	
void Ant::die() { 
	// Identify ant's tile 
	Tile tile = map->getTile(x_pos, y_pos); 
	tile.killAnt(this); 
} 

void Ant::eat(Food * food) { 
	// should be synchronized 
	hasFood = food.consume(); 
} 

/**  
* Compute the ants coordinates 
* @return int * the coordinates' array 
*/  
int * Ant::getCoordinates() { 
	int coords[] = {x_pos, y_pos}; 
	return coords; 
} 
	
/**  
* Increment the pheromone's intensity in the current ant's tile; 
* a map would be maybe convenient 
*/  
void Ant::releasePheromone() { 
	const Tile currTile = map->getTile(x_pos, y_pos); 
	currTile.incrementPheromone(); 
} 

void Ant::moveToColony() { 
	// Identify the ant's colony 
	const Tile colonyTile = antHill->tile; 
	// and its coordinates 
	const int xColony = Tile->x; 
	const int yColony = Tile->y; 
	// Compute, then, the ant's tile's neighbors 
	std::vector<Tile> neighbors = map->neighbors(x_pos, y_pos); 
			
	// Line through the ant's coordinates and the colony's 
	Vec antVec = Vec(x_pos, y_pos); 
	Vec colonyVec = Vec(xColony, yColony); 
			
	moveSegment(antVec, colonyVec, neighbors); 
} 

/**  
* Shift a ant across a segment, which is characterized by a pair of vectors.  
* @param Vec v, w the vectors subjacent to the segment 
* @param std::vector<Tile> neighbors the list of neighbors of the ant 
*/  
void Ant::moveInSegment(Vec v, Vec w, std::vector<Tile> neighbors) { 
	// Move across the segment that underlies the convex combinations of the 
	// vectors `v` and `w` 
	float currDist = MAX_INT; 
	Vec moveDirection; 
	for (Tile neighbor : neighbors) { 
		int x = neighbor.x; 
		int y = neighbor.y; 

		Vec neighborVec = Vec(x, y); 
		float distanceToLine = distanceToSegment(v, w, neighborVec); 

		if (distanceToLine < currDist) { 
			currDist = distanceToLine; 
			moveDirection = neighborVec; 
		} 
	} 
	
	// Identify the movement direction 
	int xDir = moveDirection.x - x; 
	int yDir = moveDirection.y - y; 
	move(xDir, yDir); 
} 

/**  
* Check whether there is a food object contemplated in the field of view.  
*/  
Tile Ant::hasFoodNear() { 
	// Instantiate the neighbors in the field of view 
	std::vector<Tile> neighbors = map->neighbors(x_pos, y_pos); 
			
	for (Tile neighbor : neighbors) { 
		if (neighbor.hasFood) 
			return neighbor; 
	} 
	return NULL; 		
} 

/**  
* Assert the existence of food in the field of view and, in this case, move to it.  
*/  
bool Ant::moveToFood() { 
	// Verify if there is food near 
	if (Tile neighbo = hasFoodNear()) { 
		int x = neighbor.x; 
		int y = neighbor.y; 
		Vec antVec = Vec(x_pos, y_pos); 
		Vec foodVec = Vec(x, y); 

		// Ant's neighbors 
		std::vector<Tile> neighbors = map->neighbors(x_pos, y_pos); 
				
		// Identify and apply the food's direction 
		moveInSegment(antVec, foodVec, neighbors); 
		return true; 
	} 
			
	// Notify that no movement was executed; this is convenient to move randomly 
	return false; 
} 

