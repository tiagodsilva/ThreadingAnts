#include <iostream> 
#include <tile.cpp> 

#include "../utils/geometry.cpp" 

#define MAX_INT 2147483647 

class Ant { 
	private: 
		// the ant's coordinates 
		int x_pos; 
		int y_pos; 

		Anthill * antHill; // The tile correspondent to the anthill 
		Map * map; // The map in which the game occurs 
		int fov; // the ant's field of view 
	public: 
		bool hasFood; // Whether the ant contains food 

		/**  
		* Constructor method for `Ant`.  
		* @param int x, y the ant's coordinates in the map 
		* @param Anthill colony the colony to which the colony's pertains  
		* @param Map map the map in which the game unfolds; it is, now, a convenient attribute
		*/  
		Ant(int x, int y, Anthill colony, Map * antsMap, int fieldOfView) 
			: x_pos(x), y_pos(y), antHill(colony), map(antsMap), fov(fieldOfView) 
			{} 
		/**  
		* Execute a movement for the current ant.  
		* @param x, y the coordinates 
		*/  
		void move(int x, int y) { 
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
		void die() { 
			// Identify ant's tile 
			Tile tile = map->getTile(x_pos, y_pos); 
			tile.killAnt(this); 
		} 

		void eat(Food * food) { 
			// should be synchronized 
			hasFood = food.consume(); 
		} 

		/**  
		* Compute the ants coordinates 
		* @return int * the coordinates' array 
		*/  
		int * getCoordinates() { 
			int coords[] = {x_pos, y_pos}; 
			return coords; 
		} 
	
		/**  
		* Increment the pheromone's intensity in the current ant's tile; 
		* a map would be maybe convenient 
		*/  
		void releasePheromone() { 
			const Tile currTile = map->getTile(x_pos, y_pos); 
			currTile.incrementPheromone(); 
		} 

		void moveToColony() { 
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
			
			// Iterate across the neighbors and compute the more adequate 
			// to move the ant 
			float currDist = MAX_INT; 
			Vec moveDirection; 
			for (Tile neighbor : neighbors) { 
				int x = neighbor.x; 
				int y = neighbor.y; 
				Vec neighborVec = Vec(x, y); 
				float distanceToLine = distanceToSegment(antVec, colonyVec, neighborVec); 
				
				// Check whether the distance is more appropriate 
				if (distanceToLine < currDist) { 
					currDist = distanceToLine; 
					moveDirection = neighborVec; 
				} 
			} 

			// Move the ant 
			int xDir = moveDirection.x - x;
			int yDir = moveDirection.y - y; 
			move(xDir, yDir); 
			} 
		}
}; 
