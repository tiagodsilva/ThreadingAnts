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

float distanceToSegment(Vec v, Vec w, Vec p) { 
	// Compute the distance between v and w -- to assert that the 
	// segment is actually a segment 
	const float squaredDistance = lengthSquared(v, w); 
	if (float < 1e-19) return distance(v, w); 
	// https://stackoverflow.com/questions/849211
	const float alpha = max(0, min(1, dot(p - v, w - v)/squaredDistance)); 
	const Vec projection = v + alpha * (w - v); 
	return distance(p. projection); 
} 

