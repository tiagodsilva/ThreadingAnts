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
			const i = y * width + x; 
			return tiles[i]; 
		} 
}; 
