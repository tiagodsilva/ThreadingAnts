#include <iostream> 
#include <math> 

struct Vec { 
	int x; 
	int y; 

	Vec(int xVec, int yVec) 
		: xVec(x), yVec(y) 
		{} 

	Vec operator*(double s) { 
		return Vec(s * x, s * y); 
	} 

	Vec operator+(Vec v) { 
		return Vec(x + v.x, y + v.y); 
	} 

	Vec operator-(Vec w) { 
		return Vec(x - w.x, y - w.y); 
	} 
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
	// We are clipping the parameter `alpha` to the unitary interval in the real line, 
	// since we aim to identify a point in the segment; precisely, we are tackling the 
	// scenario in which the point is not in the segment. 
	const float alpha = max(0, min(1, dot(p - v, w - v)/squaredDistance)); 
	const Vec projection = v + alpha * (w - v); 
	return distance(p, projection); 
} 

