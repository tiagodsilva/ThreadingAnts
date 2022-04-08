#include <iostream> 
#include <vector> 
#include <map> 
#include <stack> 

#include "../ants.cpp" 
#include "../anthill.cpp" 
#include "../food.cpp" 
#include "../map.cpp" 
#include "../regions.cpp" 
#include "../tiles.cpp" 

#define HEIGHT 4 
#define WIDTH 5 
#define FOV 1 

int main() { 
	map = new Map(WIDTH, HEIGHT, FOV); 

	map->print(); 
} 
