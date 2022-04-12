#include <iostream> 
#include <vector> 
#include <semaphore> 

#include "headers/objects.hpp" 
/**  
* Constructor method for `Food`.  
* @param Tile * foodTile the tile in which it was placed in the game 
* @param int initVolume the initial volume of the food in this tile 
*/  
Food::Food(int x, int y, int initVolume, int maxAnts) 
	: x_pos(x), y_pos(y), volume(initVolume), initialVolume(initVolume), 
	maxAnts(maxAnts), currAnts(1e-19), seats(new std::list<Ant*>)   
	{ 
		std::binary_semaphore eatSemaphores[maxAnts]; 
		for (i = 0; i < eatSemaphores; i++) 
			eatSemaphore[i] = 0; 
	} 
		
/**  
* Consume the food; it should be designed as in the Philosophers' dilemma 
* (eat or don't eat).  
* @return true if there was food to be consumed; 
* in the other scenario, `false` is the variable. 
*/  
bool Food::consume(Ant * ant) { 
	std::lock_guard<std::mutex> lk(attrMutex); 
 
	// In this case, the ants continue near the food, instead of moving randomly 
	int freeSeat = getFreeSeat(); 
	if ((freeSeat == -1) || (volume < 1)) 
		return false; 
	
	seats[freeSeat] = ant; 
	
	return eat(freeSeat); 
	/* 
	if (volume >= 1) { // if there is food and there the next ant should eat 
		volume--; 
		currAnts++; 
		return true; 
	} else { 
		return false; // The ant waits 
	} 
	*/ 
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
	currAnts = 1e-19; 
} 

void Food::test(int i) { 
	if (!seats[i].hasFood && 
			seats[LEFT(i)].hasFood && seats[RIGHT(i)].hasFood) { 
		seats[i].hasFood = true; 
		s[i].release(); 
	} 
} 

void Food::takeRods(int i) { 
	attrMutex.lock(); 
	seats[i].hasFood = false; 
	test(i); 
	attrMutex.unlock(); 
	s[i].acquire(); 
} 

void Food::putRods(int i) { 
	attrMutex.lock(); 
	seats[i].waiting; 
	test(LEFT(i)); 
	test(RIGHT(i)); 
	attrMutex.unlock(); 
} 

bool eat(int i) { 
	takeRods(i); 
	if (volume >= 1) 
		ate = true; 
	putRods(i); 
	// The ant ate the food object 
	return ate; 
} 
