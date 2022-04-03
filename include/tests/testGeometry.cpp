#include <iostream> 
#include "../headers/geometry.hpp" 

#define IS_TRUE(x) { if(!(x)) std::cout << __FUNCTION__ << " inadequate; line " << __LINE__ << std::endl; } 

/**  
* Verify the sum of the vectors, `Vec`.  
* @param Vec * v, Vec * w pointers to the vector 
*/  
void testSum(Vec * v, Vec * w) { 
	// Sum the vectors 
	Vec vwSum = *v + *w; 
	
	IS_TRUE(vwSum.x == v->x + w->x); 
	IS_TRUE(vwSum.y == v->y + w->y); 

	std::cout << "The operator + was checked!" << std::endl; 
} 

/**  
* Verify, now, the subtraction of the vectors.  
* @param Vec *v, Vec *w the vectors we aim to subtract. 
*/  
void testSubtraction(Vec * v, Vec * w) { 
	// Apply the subtraction 
	Vec vw = *v - *w; 
	
	IS_TRUE(vw.x == v->x - w->x); 
	IS_TRUE(vw.x == v->y - w->y); 

	std::cout << "The operator - was checked!" << std::endl; 
} 

/**  
* Verify, also, the multiplication of a vector by a real scalar.  
* @param Vec * v the vector 
* @param double s the scalar 
*/  
void testMultiplication(Vec * v, double s) { 
	// Multiply `v` by `s` 
	Vec vs = s * (*v); 
	
	IS_TRUE(vs.x == s * v->x); 
	IS_TRUE(vs.y == s * v->y); 

	vs = (*v) * s; 

	IS_TRUE(vs.x == s * v->x); 
	IS_TRUE(vs.y == s * v->y); 

	std::cout << "The operator * was checked!" << std::endl; 
} 

/**  
* Verify, on the other hand, whether the computation of the distance between a point and a 
* segment is consitent with what we should contemplate.  
* @param Vec * v, Vec * w, Vec p the vectors underlying the segment and the point, the distance 
* 	to which we are going to compute 
* @param double expectedDistance the distance we should verify 
*/  
void testDistances(Vec * v, Vec * w, Vec p, double expectedDistance) { 
	// Compute the distances 
	double distance = distanceToSegment(*v, *w, p); 

	const double tol = 1e-5; 
	IS_TRUE(distance <= expectedDistance + tol && distance >= expectedDistance - tol); 
} 

int main() { 
	// Instantiate vectors 
	Vec * v = new Vec(1, 3); 
	Vec * w = new Vec(2, 4); 

	// Check opeators 
	testSum(v, w); 
	testSubtraction(v, w); 
	testMultiplication(v, 3); 

	Vec p = (*v + *w) * .5; 
	testDistances(v, w, p, int(1e-13)); 

	p = Vec(1, 4); 
	testDistances(v, w, p, std::sqrt(2)/2); 
} 
