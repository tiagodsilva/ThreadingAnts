#include <iostream> 
#include <vector> 
#include <map> 
#include <stack> 

#include <chrono> 
#include <thread> 

#include<string> 

#include "../ants.cpp" 
#include "../anthill.cpp" 
#include "../food.cpp" 
#include "../map.cpp" 
#include "../regions.cpp" 
#include "../tiles.cpp" 

#define HEIGHT 4 
#define WIDTH 5 
#define FOV 1 

const std::string LINES = "++++++++++++++++++++++++++++++++"; 

int main() { 
	map = new Map(WIDTH, HEIGHT, FOV); 
	
	while (true) { 
		std::this_thread::sleep_for(std::chrono::milliseconds(299)); 
		map->print(); 
		std::cout << LINES << std::endl; 
	} 
	// map->print(); 
} 
