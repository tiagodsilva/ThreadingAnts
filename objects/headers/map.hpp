#include <iostream> 
#include <vector> 
#include <string> 

class Map { 
	private: 
		// the map's dimensions 
		int width; 
		int height; 

		// the tiles in the map 
		Tile * tiles; 
	public: 
		Map(int mapWidth, int mapHeight); 
		Tile getTile(int x, int y); 
		std::vector<Tile> neighbors(int x, int y, 
				int xLeftOffset = -1, 
				int yBottomOffset = -1, 
				int xRightOffset = 1, 
				int yTopOffset = 1); 
		void print(); 

}; 
