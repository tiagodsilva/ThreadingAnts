#include <iostream> 
#include <tile.cpp> 

class Ant { 
	private: 
		// the ant's coordinates 
		int x_pos; 
		int y_pos; 

		Tile * antHill; // The tile correspondent to the anthill 
	public: 
		bool hasFood; // Whether the ant contains food 
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
}; 
