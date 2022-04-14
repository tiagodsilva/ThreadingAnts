#ifndef CLASSES_H 
#define CLASSES_H 
#include <iostream> 
#include <vector> 
#include <map> 
#include <stack>  
#include <utility> 
#include <list> 
#include <thread> 
#include <mutex> 
// #include <semaphore> 
#include <semaphore.h> 
#include <condition_variable> 

#include "geometry.hpp" 

class Anthill; 
class Map; 
class Ant; 
class Tile; 
class Region; 
class Food; 

int GAME_ITERATION = 0;  

enum { 
	HUNGRY = 1, // The ant is aiming for food in this seat 
	EATING = 2, // The ant already has food in this seat 
	FREE = 3, // The seat is free; the ants are in moving 
};  

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
}; 

class Anthill { 
	private: 
		// the colony's name and coordinates, sequentially 
		std::string name; 
		int x_pos; 
		int y_pos; 
		
		// Whether the ants were inserted in the game 
		bool isInitialized = false; 
		// Mutex to control the access to the food storage 
		std::mutex foodMutex; 
	public: 
		int initialAnts; 
		int foodStorage; 

		// Methods 
		Anthill(int x, int y, std::string colonyName); 
		void instantiateAnts(int numAnts); 
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
		// Mutex to control the access to the food's attributes 
		std::mutex attrMutex; 
		std::mutex seatMutex;

		// The maximum quantity of ants that would eat this food in a 
		// stage 
		int maxAnts; 
		int currAnts; // identify the quantity of ants eating this object 

		// Compute the indexes in a round table 
		int LEFT(int i); 
		int RIGHT(int i); 
		
		// Check whether a seat is free in the table 
		void getFreeSeat(); 
	public: 
		int * seats; // Seats for the philosopher problem 
		sem_t * eatSemaphores; // Semaphores for the philosopher problem 
		// methods 
		Food(int x, int y, int initVolume, int maxAnts); 
		bool consume(); 
		void restore(); 
		int getVolume(); 
		void allowAnts(); 

		// Methods for the philosophical ants 
		void test(int i); 
		void wait(int i); 
		void takeRods(int i); 

		void eat(int i); 
		void putRods(int i); // The ant is released from the list 

		// Choose, randomly, whether to fight, and possibly die 
		bool fight(); 
}; 

class Ant {
	private:
		// The ant's coordinates
		int x_pos;
		int y_pos;

		Anthill * antHill;
	public:
		bool hasFood;
		bool isDead; 

		// Explicit ants methods
		Ant(int x, int y, Anthill * colony);
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

		// Choose, randomly, whether to fight, and possibly die 
		bool fight(); 
};

class Tile {
	private:
		// Tile's coordinates
		int x;
		int y;

		void traverseStack(std::stack<Ant*>* st, std::vector<Ant*>* vt); 
		std::list<Pheromone*> * pheromones; // the pheromones in this tile 
		// A mutex to control multithreading modification of this tile's attributes 
		std::mutex tileMutex; 
		// A mutex to control the access to the phermones 
		std::mutex pherMutex; 
		// A mutex to control the death of the ants 
		std::mutex deathMutex; 
	public:
		// Array with the ants in this tile for each colony
		std::map<std::string, std::stack<Ant*>*> * ants;
		// and with the deaths 
		std::map<std::string, int> deaths; 

		// Ramifications of this tile 
		bool isAnthill; 
		std::string anthillName; 
		bool isFood; 

		int pheromone; // the pheromone's intensity in this tile 

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

		void checkPheromones(); 
		
		void incrementDeaths(std::string anthillName); 
		void killAnts(); 
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
		// the pheromone's lifetime 
		int psurvival; 

		// Mutex to control the map's update 
		std::mutex mapMutex; 
		std::mutex ioMutex; // Mutex to control the access to the IO methods in 
				// this map 
		std::condition_variable cv; 
		// It is convenient, in a multithread program, to track the 
		// ants in a list 
		bool isInitialized; 
		long unsigned int currAnt; // A pointer to the current ant 
		std::list<Ant*> * allAnts; 

		// The rate with which we  restore the foods in this map 
		int ufood; 
	public:
		// Whether we should implement fight; it is unstable 
		bool fight; 
		Map(int mapWidth, int mapHeight, int fov, int psurvival, int ufood, 
				bool fight);
		Map(); 
		Tile * getTile(int x, int y);
		std::vector<Tile*> neighbors(int x, int y);

		// Initialization methods 
		void insertAnthill(int x, int y, std::string anthillName, 
				int nAnts); 
		void insertFood(int x, int y, int initialVolume, int maxAnts); 

		// Compute attributes 
		Anthill * getAnthill(std::string anthillName); 
		Food * getFood(int x, int y); 
		std::vector<Food*> getFoods();  
		void print();
		Ant * getAntInTile(int x, int y, std::string colony); 
		Ant * getAnyAnt(); 

		void updateTiles(long unsigned int lTilesIndex, 
				long unsigned int rTilesIndex); // Updates the tiles' attributes 
		// Appropriate for multithreaded programs 
		void initializeAnts(); 
		Ant * computeNextAnt(); 
		bool allAntsPlayed(); 
		void prepareNextIter(int nThreads); 

		Tile * captureFoodNear(int x, int y); 
		int getPSurvival(); 

		void reinitializeAnts(); 

		void printAnthillsChars(); 
};

Map * map; 
#endif 
