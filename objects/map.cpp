#include <iostream> 
#include <vector> 
#include <string> 

class Map { 
	private: 
		// The map's dimensions 
		int width; 
		int height; 

		// The tiles in the map 
		Tile * tiles; 
	public: 
		/**  
		* Constructor method for Map.  
		* @param mapWidth, mapHeight the map's dimensions 
		*/  
		Map(int mapWidth, int mapHeight) 
			: width(mapWidth), height(mapHeight) { 
				// Instantiate an array of tiles 
				int area = mapWidth * mapHeight; 
				tiles = new Tile[area]; 
				// and insert tiles in it 
				for (int i = 0; i < area; i++) { 
					// This is an array with a row major format; 
					// that is, tiles contains `mapWidth` arrays with length equal 
					// to `mapHeight`, 
					// |+++...+++|+++...+++|...|+++...+++| 
					//  mapWidth  mapWidth      mapWidth 
					// (each `mapWidth` tile corresponds to a row).  
					const int row = i/mapWidth; 
					const int column = i - mapWidth * row; 
					tiles[i] = new Tile(column, row); 
				} 
		} 
		
		/**  
		* Compute the tile from the coordinates x and y.  
		* @param int x, y the tile's coordinates 
		*/  
		Tile getTile(int x, int y) { 
			// Compute the tile's instance from coordinates x and y 
			if (x >= width || y >= height) 
				return NULL; 

			const i = y * width + x; 
			return tiles[i]; 
		} 

		std::vector<Tile> neighbors(int x, int y) { 
			// Vector to consolidate the neighborhood tiles 
			std::vector<Tile> neighborsTiles; 
			// Identify whether the tile is in a corner 
			int ys[] = {y - 1, y, y + 1}; 
			int xs[] = {x - 1, x, x + 1};  

			// Iterate across the tile's neighbors 
			for (int i = -1; i < 2; i++) { 
				// Compute the actual neighbor 
				for (int j = -1; j < 2; j++) { 
					int currX = xs[i]; 
					int currY = ys[j]; 

					if (currX == x && currY == y) 
						continue; 
					
					Tile currTile = getTile(int x, int y); 
					if (!currTile) 
						continue; 
					
					neighborsTiles.push(currTile); 
				} 
			} 

			return neighborsTiles; 
		} 
}; 
