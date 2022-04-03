#include <iostream> 
#include <vector> 

#include "headers/objects.hpp" 

/**  
* Constructor method for `Region`; it is designed to be a mutex keeper.  
* @param int xRegion, yRegion, xOffsetRegion, 
* 	yOffsetRegion the offsets that characterize the region 
* @param Map * map the map to which we tied the tiles 
*/  
Region::Region(int xRegion, int yRegion, 
		int xOffsetRegion, 
		int yOffsetRegion)
	: x(xRegion), y(yRegion), 
	xOffset(xOffsetRegion), yOffset(yOffsetRegion) { 
		// Identify the tiles in the region charecterized by the provided coordinates 
		for (int col = x; col < x + xOffset; col++) { 
			for (int row = y; row < y + yOffset; row++) { 
				// Check whether the tile is consistent with the 
				// map boundaries 
				Tile * tile = map->getTile(col, row); 
				if (tile) 
					tiles.push_back(tile); 
			} 
		} 
} 

// It should contain functions to control simultaneous access to shared data  
		
// mutex functions 
