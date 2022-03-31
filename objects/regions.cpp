#include <iostream> 
#include <vector> 

class Region { 
	private: 
		std::vector<Tile> tiles; // the tiles in the current region 

		// The coordinates in which the region starts 
		//   | 
		//   | Region 
		// (x, y)+++
		int x; 
		int y; 

		// The horizontal and vertical offsets 
		int xOffset; 
		int yOffset; 

		// a mutex 
	public: 

} 

