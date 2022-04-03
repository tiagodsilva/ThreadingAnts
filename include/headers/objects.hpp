#ifndef CLASSES_H 
#define CLASSES_H 
#include <iostream> 
#include <vector> 
#include <map> 
#include <stack>  

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
		void instantiateAnts(int numAnts, int fov); 
		void incrementFood(); 

		int getX(); 
		int getY(); 

		std::string getName(); 
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
		Food(int x, int y, int initVolume); 
		bool consume(); 
		void restore(); 
}; 

class Ant {
	private:
		// The ant's coordinates
		int x_pos;
		int y_pos;

		Anthill * antHill;
		int fov; // field of view
	public:
		bool hasFood;

		// Explicit ants methods
		Ant(int x, int y, Anthill * colony, int fieldOfView);
		void move(int x, int y);
		void die();
		void eat(Food * food);
		int getX(); 
		int getY(); 
		void releasePheromone();
		void moveToColony();
		void moveInSegment(Vec v, Vec w, std::vector<Tile*> neighbors);
		Tile * hasFoodNear();
		bool moveToFood();
		Anthill * getAnthill(); 
};

class Tile {
	private:
		// Tile's coordinates
		int x;
		int y;

		// Array with the ants in this tile for each colony
		std::map<std::string, std::stack<Ant*>*> ants;
	
	public:
		// Ramifications of this tile 
		bool isAnthill; 
		bool isFood; 
		int pheromone; // the intensity of pheromone in these coordinates

		Tile(int x_tile, int y_tile, 
				bool containsAnthill, bool containsFood);
		Tile(); 
		void insertAnt(Ant * ant);
		void killAnt(Ant * ant);
		Ant * extractAnt(Ant * ant);
		std::map<std::string, int> numAnts();
		void incrementPheromone();
		std::string print();
		int getX(); 
		int getY(); 
};

class Map {
	private:
		// the map's dimensions
		int width;
		int height;

		// the tiles in the map
		std::vector<Tile*> tiles; 
	public:
		Map(int mapWidth, int mapHeight);
		Map(); 
		Tile * getTile(int x, int y);
		std::vector<Tile*> neighbors(int x, int y,
				int xLeftOffset = -1,
				int yBottomOffset = -1,
				int xRightOffset = 1,
				int yTopOffset = 1);
		void print();

};

Map * map; 

class Region { 
	private: 
		std::vector<Tile*> tiles; // the tiles in this region 
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

		// it needs a mutex 
	public: 
		Region(int xRegion, int yRegion, 
			int xOffsetRegion, int yOffsetRegion); 
}; 
#endif 
