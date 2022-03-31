#include <iostream> 

class Ant { 
	private: 
		int x_pos; 
		int y_pos; 
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
