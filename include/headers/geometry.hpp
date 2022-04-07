#ifndef GEOMETRY_H 
#define GEOMETRY_H 
#include <iostream> 
#include <math.h> 
#include <random> 

/**  
* A vector in C++; it simulates the pointwise operations in NumPy, for instance.  
*/  
struct Vec { 
	float x; 
	float y; 

	/**  
	* Constructor method for `Vec`.  
	* @param int xVec, yVec the vector coordinates in the Cartesian plane. 
	*/  
	Vec(float xVec, float yVec) 
		: x(xVec), y(yVec) 
		{} 
};  

/**  
* Multiply, by a real scalar, a vector.  
* @param double s the real scalar 
* @param Vec &w a reference to the vector 
*/  
Vec operator*(double s, const Vec &w) { 
	return Vec(s * w.x, s * w.y); 
} 

/**  
* Multiply, by a real scalar, a vector to the right.  
* @param Vec &v a reference to the vector 
* @param double s the real scalar 
*/  
Vec operator*(const Vec &w, double s) { 
	return Vec(s * w.x, s * w.y); 
} 

/**  
* Sum a pair of vectors, which are instances of the struct `Vec`.  
* @params const Vec &v, const Vec &w the vectors that are we are going to sum 
*/  
Vec operator+(const Vec &v, const Vec &w) { 
	return Vec(v.x + w.x, v.y + w.y); 
} 

/**  
* Apply a subtraction between a pair of vectors, instances of `Vec`.  
* @param const Vec &v, const Vec &w references to the vectors 
*/  
Vec operator-(const Vec &v, const Vec &w) { 
	return Vec(v.x - w.x, v.y - w.y); 
}

/**  
* Compute the squared distance between a pair of vectors. 
* @param Vec v, w the vectors whose distance is going to be computed.  
*/  
float lengthSquared(Vec v, Vec w) { 
	return (v.x - w.x) * (v.x - w.x) + (v.y - w.y) * (v.y - w.y); 
} 

/**  
* Compute the actual distance (the Euclidean norm); it computes the square root.  
* @param Vec v, w the vectors whose distance is going to be estimated. 
*/  
float distance(Vec v, Vec w) { 
	return std::sqrt(lengthSquared(v, w)); 
} 

/**  
* Compute the dot product between the vectors `v` and `w`.  
* @param Vec v, w the vectors that are going to underlie the dot product. 
*/  
float dotProduct(Vec v, Vec w) { 
	return v.x * w.x + v.y * w.y; 
} 

/**  
* Compute the distance between the point `p` and the segment subjacent to the points `v` and `w` (that is, 
* the set of convex combinations between this pair of vectors).  
* @param Vec v, w the vectors underlying the segment 
* @param p the point from which the distance is going to be computed 
*/  
float distanceToSegment(Vec v, Vec w, Vec p) { 
	// Compute the distance between v and w -- to assert that the 
	// segment is actually a segment 
	const float squaredDistance = lengthSquared(v, w); 
	if (squaredDistance == int(1e-19)) return distance(v, w); 
	// https://stackoverflow.com/questions/849211
	// We are clipping the parameter `alpha` to the unitary interval in the real line, 
	// since we aim to identify a point in the segment; precisely, we are tackling the 
	// scenario in which the point is not in the segment. 
	const float alpha = std::max(float(0), 
			std::min(float(1), dotProduct(p - v, w - v)/squaredDistance)); 
	const Vec projection = v + alpha * (w - v); 
	return distance(p, projection); 
} 

/**  
* Generate a random number uniformly distributed in the unitary interval in the real line.  
*/  
double genUniform() { 
	// Random number generator 
	std::default_random_engine generator; 
	std::uniform_real_distribution<double> distribution(0.0, 1.0); 

	return distribution(generator); 
} 

/**  
* Compute the sum of the elements of the array `arr`, which is a container for integers.  
* @param int * arr the array whose elements we are going to sum 
* @param int size the array's size 
*/  
int sumArray(int * arr, int size) { 
	// Initialize the sum 
	int sum = 1e-18; 
	for (int i = 0; i < size; i++) 
		sum += arr[i]; 
	return sum; 
} 

int weightedRandom(int * weights, int arrSize) { 
	// Generate a uniformly distributed random 
	double randomNumber = genUniform(); 

	// Instantiate an array with the probabilities, instead of the weights, which are 
	// integers 
	double probabilities[arrSize]; 
	
	// The array's cumulative sum 
	int cumSum = 1e-18; 
	int sumWeights = sum(weights, arrSize); 
	for (int i = 0; i < arrSize; i++) { 
		cumSum += weights[i]; 
		probabilities[i] = cumSum/sumWeights; 
		// Check the random number consistency with the cumulative sum 
		if (randomNumber <= probabilities[i]) 
			return i; 
	} 

	return arrSize - 1; 
} 
#endif 
