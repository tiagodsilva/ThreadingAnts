#include <iostream> 
#include <tile.cpp> 

class Ant { 
	private: 
		// the ant's coordinates 
		int x_pos; 
		int y_pos; 

		Anthill * antHill; // The tile correspondent to the anthill 
		Map * map; // The map in which the game occurs 
	public: 
		bool hasFood; // Whether the ant contains food 

		/**  
		* Constructor method for `Ant`.  
		* @param int x, y the ant's coordinates in the map 
		* @param Anthill colony the colony to which the colony's pertains  
		* @param Map map the map in which the game unfolds; it is, now, a convenient attribute
		*/  
		Ant(int x, int y, Anthill colony, Map * antsMap) 
			: x_pos(x), y_pos(y), antHill(colony), map(antsMap)  
			{} 
		/**  
		* Execute a movement for the current ant.  
		* @param x, y the coordinates 
		*/  
		void move(int x, int y) { 
			x_pos += x; 
			y_pos += y; 
		} 

		void eat(Food * food) { 
			// should be synchronized 
			food->volume--; 
			hasFood = true; 
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
}; 
