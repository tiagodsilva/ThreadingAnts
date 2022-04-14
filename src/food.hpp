#include <iostream> 
#include <vector> 
#include <thread> 
// #include <semaphore> 
#include <semaphore.h> 

#include "objects.hpp" 
/**  
* Constructor method for `Food`.  
* @param Tile * foodTile the tile in which it was placed in the game 
* @param int initVolume the initial volume of the food in this tile 
*/  
Food::Food(int x, int y, int initVolume, int maxAnts) 
	: x_pos(x), y_pos(y), volume(initVolume), initialVolume(initVolume), 
	maxAnts(maxAnts), currAnts(0) 
	{ 
		eatSemaphores = new sem_t[maxAnts]; 
		seats = new int[maxAnts]; 

		for (int i = 0; i < maxAnts; i++) 
			seats[i] = FREE; 

		for (int i = 0; i < maxAnts; i++) 
			sem_init(&eatSemaphores[i], 0, 0); 
	} 
		
/**  
* Consume the food; it should be designed as in the Philosophers' dilemma 
* (eat or don't eat).  
* @return true if there was food to be consumed; 
* in the other scenario, `false` is the variable. 
*/  
bool Food::consume() { 
	// std::lock_guard<std::mutex> lk(seatMutex); 
 
	// In this case, the ants continue near the food, instead of moving randomly 
	getFreeSeat(); 
	if (currAnts > maxAnts || (volume < 1)) 
		return false;   
	
	int thisSeat = currAnts; 	
	eat(thisSeat); // Eat the food consistently with the rods embracing 
	return true; // The food was eaten 
} 

/**  
* Restore the initial volume, as asserted in the parameter `initialVolume`.  
*/  
void Food::restore() { 
	volume = initialVolume; 
} 

/**  
* Compute the food's volume, in arbitrary units. 
*/  
int Food::getVolume() { 
	return volume; 
} 

/**  
* Allow ants to eat the food; it is executed at each game iteration.  
*/  
void Food::allowAnts() { 
	currAnts = 0; 
} 

/**  
* Assert that there are rods available for the ant at a seat; it is an 
* instance of the dining philosophers scenario.  
* @param int i the seat of the ant that is going to eat the food object 
*/  
void Food::test(int i) { 
	if (seats[i] == HUNGRY && 
		seats[LEFT(i)] != EATING && 
		seats[RIGHT(i)] != EATING) { 
		seats[i] = EATING; 
		sem_post(&eatSemaphores[i]); 
	} 
} 

/**  
* Take the rods; the ants use them to eat the food object.  
* @param int i the current seat of the ant 
*/  
void Food::takeRods(int i) { 
	attrMutex.lock(); 
	seats[i] = HUNGRY; 
	test(i); 
	attrMutex.unlock(); 
	sem_wait(&eatSemaphores[i]); 
} 

/**  
* Put the rods in the food; it is appropriate for the philosophical methods.  
* @param int i the current seat of the ant 
*/  
void Food::putRods(int i) { 
	attrMutex.lock(); 
	seats[i] = FREE; // The food ate the object 
	test(LEFT(i)); 
	test(RIGHT(i)); 
	attrMutex.unlock(); 
} 

/**  
* Eat the food; it checks if the volume is positive.  
* @param int i the current seat of the ant. 
*/  
void Food::eat(int i) { 
	takeRods(i); 
	if (volume >= 1) { 
		volume--; 
	} 
	putRods(i); 
	// The ant ate the food object 
	// return ate; 
} 

/**  
* Compute the ant at the left; they are in a table.  
* @param int i the current ant 
*/  
int Food::LEFT(int i) { 
	return (i + maxAnts - 1) % maxAnts; 
} 

/**  
* Compute the ant at the right; they are in a round table.  
* @param int i the current ant 
*/  
int Food::RIGHT(int i) { 
	return (i + 1) % maxAnts; 
} 

/**  
* Identify free seats in the table; they would be used by the ants.  
*/  
void Food::getFreeSeat() { 
	std::lock_guard<std::mutex> lk(seatMutex); 
	// Check that, in this iteration, the quantity of ants that tried to interact with 
	// the food object 
	if (currAnts > maxAnts) 
		return; 
	
	// As we increment the attribute `currAnts`, we assert that the 
	// ant is already in the table; therefore, we should not modify 
	// the `seats` variable 
	currAnts++; 
} 
