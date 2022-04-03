#ifndef CLASSES_H 
#define CLASSES_H 
#include <iostream> 
#include <vector> 
#include <map> 
#include <queue> 

#include "geometry.hpp" 

class Anthill; 
class Map; 
class Ant; 
class Tile; 
class Region; 
class Food; 

class Anthill { 
	private: 
		// the colony's name and coordinates, sequentially 
		std::string name; 
		int x_pos; 
		int y_pos; 
		
		// Whether the ants were inserted in the game 
		bool isInitialized = false; 
	public: 
		int initialAnts; 
		int foodStorage; 

		// Methods 
		Anthill(int x, int y, std::string colonyName); 
		void instantiateAnts(int numAnts); 
		void incrementFood(); 
}; 

class Food { 
	private: 
		int x_pos;
	       	int y_pos;  // the tile in which the food is in the game 
		int volume; // the amount of volume 

		int initialVolume; // the initial volume of food in this tile; 
		// it is appropriate to restore the volume 
	public: 
		// methods 
		Food(Tile * foodTile, int initVolume); 
		bool consume(); 
		void restore(); 
}; 


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


class Ant {
	private:
		// The ant's coordinates
		int x_pos;
		int y_pos;

		Anthill * antHill;
		Map * map;
		int fov; // field of view
	public:
		bool hasFood;

		// Explicit ants methods
		Ant(int x, int y, Anthill * colony, Map * antsMap, int fieldOfView);
		void move(int x, int y);
		void die();
		void eat(Food * food);
		int getX(); 
		int getY(); 
		void releasePheromone();
		void moveToColony();
		void moveInSegment(Vec v, Vec w, std::vector<Tile> neighbors);
		Tile hasFoodNear();
		bool moveToFood();
};

class Tile {
	private:
		// Tile's coordinates
		int x;
		int y;

		// Array with the ants in this tile for each colony
		std::map<std::string, std::queue<Ant*>> ants;

		// Ramifications of this tile
		bool isAnthill;
		bool isFood;

	public:
		int pheromone; // the intensity of pheromone in these coordinates

		Tile(int x_tile, int y_tile, bool containsAnthill, bool containsFood);
		void insertAnt(Ant * ant);
		void killAnt(Ant * ant);
		Ant * extractAnt(Ant * ant);
		std::map<std::string, int> numAnts();
		void incrementPheromone();
		std::string print();
};

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
				int xOffsetRegion, int yOffsetRegion, 
				Map * mapRegion); 
}; 
#endif 
