#include <iostream> 
#include <math> 

struct Vec { 
	int x; 
	int y; 
} 

float lengthSquared(Vec v, Vec w) { 
	return (v.x - w.x) * (v.x - w.x) + (v.y - w.y) * (v.y - w.y); 
} 

float distance(Vec v, Vec w) { 
	return sqrt(lengthSquared(v, w)); 
} 

float dotProduct(Vec v, Vec w) { 
	return v.x * w.x + v.y * w.y; 
} 


