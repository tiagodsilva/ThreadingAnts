#ifndef CLASSES_H 
#define CLASSES_H 
#include <iostream> 
#include <vector> 
#include <map> 
#include <stack>  
#include <utility> 
#include <list> 

#include "geometry.hpp" 

class Anthill; 
class Map; 
class Ant; 
class Tile; 
class Region; 
class Food; 

struct Pheromone { 
	int lifetime; 
	int init; 
	/**  
	* Constructor method for `Pheromone`; it is designed to instantiate 
	* have a lifetime. 
	* @param int lifetime the phermone's lifetime   
	*/  
	Pheromone(int lifetime, int initialIteration) 
		: lifetime(lifetime), init(initialIteration) 
		{} 

	/**  
	* Check whether this pheromone should die; that is, 
	* if it is beyond its own lifetime.  
	* @param int iteration the current iteration in the game 
	*/  
	bool kill(int iteration) { 
		return iteration > init + lifetime; 
	} 
} 

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
		int getVolume(); 
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
		void moveRandomly(); 
		void stage(); 
};

class Tile {
	private:
		// Tile's coordinates
		int x;
		int y;

		void traverseStack(std::stack<Ant*>* st, std::vector<Ant*>* vt); 
	public:
		// Array with the ants in this tile for each colony
		std::map<std::string, std::stack<Ant*>*> * ants;
		// Ramifications of this tile 
		bool isAnthill; 
		std::string anthillName; 
		bool isFood; 
		int pheromone; // the intensity of pheromone in these coordinates

		Tile(int x_tile, int y_tile, bool containsFood);
		Tile(int x_tile, int y_tile, std::string anthillName); 
		Tile(); 
		void insertAnt(Ant * ant);
		void killAnt(Ant * ant);
		Ant * extractAnt(Ant * ant);
		std::map<std::string, int> numAnts();
		void incrementPheromone();
		std::string print();
		int getX(); 
		int getY(); 
		Ant * getAnt(); 
		std::vector<Ant*> getAnts(); 
};

class Map {
	private:
		// the map's dimensions
		int width;
		int height;

		// the tiles in the map
		std::vector<Tile*> tiles; 

		// a map from the anthill's name to the anthill 
		std::map<std::string, Anthill*> anthillMap; 
		
		// and a list with the foods' coordinates 
		std::map<std::pair<int, int>, Food*> foods; 

		// the ant's field of view 
		int fov; 
	public:
		Map(int mapWidth, int mapHeight, int fov);
		Map(); 
		Tile * getTile(int x, int y);
		std::vector<Tile*> neighbors(int x, int y);
		void insertAnthill(int x, int y, std::string anthillName, 
				int nAnts); 
		void insertFood(int x, int y, int initialVolume); 
		Anthill * getAnthill(std::string anthillName); 
		Food * getFood(int x, int y); 
		std::vector<Food*> getFoods();  
		void print();
		Ant * getAntInTile(int x, int y, std::string colony); 
		Ant * getAnyAnt(); 
		void restoreFoods(); 
		std::pair<std::vector<Ant*>::iterator, 
			std::vector<Ant*>::iterator> getAllAnts(); 
		Tile * captureFoodNear(int x, int y); 
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
