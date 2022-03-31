#include <iostream> 
#include <tile.cpp> 

class Ant { 
	private: 
		// the ant's coordinates 
		int x_pos; 
		int y_pos; 

		Tile * antHill; // The tile correspondent to the anthill 
	public: 
		/**  
		* Execute a movement for the current ant.  
		* @param x, y the coordinates 
		*/  
		void move(int x, int y) { 
			x_pos += x; 
			y_pos += y; 
		} 

}; 
