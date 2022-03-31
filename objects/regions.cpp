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
		
		Map * map; // the map in which the game is happening 
		// a mutex 
	public: 
		/**  
		* Constructor method for `Region`; it is designed to be a mutex keeper.  
		* @param int xRegion, yRegion, xOffsetRegion, yOffsetRegion the offsets that characterize the region 
		* @param Map * map the map to which we tied the tiles 
		*/  
		Region(int xRegion, int yRegion, 
				int xOffsetRegion, int yOffsetRegion, Map * mapRegion) 
			: x(xRegion), y(yRegion), xOffset(xOffsetRegion), yOffset(yOffsetRegion), 
			mapRegion(map) { 
				// Identify the tiles in the region charecterized by the provided coordinates 
				for (int col = x; col < x + xOffset; col++) { 
					for (int row = y; row < y + Offset; row++) { 
						// Check whether the tile is consistent with the 
						// map boundaries 
						if (Tile tile = map->getTile(col, row)) 
						       tiles.push(tile); 	
					 } 
				} 
		} 

		// It should contain functions to control simultaneous access to shared data  
		
		// mutex functions 
} 

