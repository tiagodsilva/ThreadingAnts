#include <iostream> 
#include <vector> 

class Tile; 

class Region { 
	private: 
		std::vector<Tile> tiles; // the tiles in this region 
		// Coordinates in which region starts 
		//  + 
		//  + Region 
		//  + 
		// (x, y) + + +   
		int x; 
		int y; 

		// The horizontal and vertical offsets 
		int xOffset; 
		int yOffset; 

		Map * map; // the game in which the game unfolds 
		// it needs a mutex 
	public: 
		Region(int xRegion, int yRegion, 
				int xOffsetRegion, yOffsetRegion, 
				Map * mapRegion); 
}; 
