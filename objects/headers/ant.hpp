#include<iostream> 

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
		Ant(int x, int y, Anthill colony, Map * antsMap, int fieldOfView); 
		void move(int x, int y); 
		void die(); 
		void eat(Food * food); 
		int * getCoordinates(); 
		void releasePheromone(); 
		void moveToColony(); 
		void moveInSegment(Vec v, Vec w, std::vector<Tile> neighbors); 
		Tile hasFoodNear(); 
		bool moveToFood(); 
} 
