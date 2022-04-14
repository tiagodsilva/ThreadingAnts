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
	std::random_device r; 
	std::default_random_engine generator(r()); 
	std::uniform_real_distribution<double> distribution(0.0, 1.0); 	

	return distribution(generator); 
} 

/**  
* Compute the sum of the elements of the array `arr`, which is a container for integers.  
* @param int * arr the array whose elements we are going to sum 
* @param int size the array's size 
*/  
template <typename T> 
T sumArray(T * arr, int size) { 
	// Initialize the sum 
	T sum = 0; 
	for (int i = 0; i < size; i++) 
		sum += arr[i]; 
	return sum; 
} 

/**  
* Sample a instance from a weighted array; those weights correspond to the probabilities.  
* @param int * weights the weighted array 
* @param int arrSize the size of the array `weights` 
*/  
template <typename T> 
int weightedRandom(T * weights, int arrSize) { 
	// Generate a uniformly distributed random 
	double randomNumber = genUniform(); 
	
	// Instantiate an array with the probabilities, instead of the weights, which are 
	// integers 
	double probabilities[arrSize + 1]; 
	
	// The array's cumulative sum 
	T cumSum = 0; 
	T sumWeights = sumArray<T>(weights, arrSize); 
	for (int i = 0; i < arrSize; i++) { 
		cumSum += weights[i]; 
		// Cast integers to float 
		probabilities[i] = static_cast<double>(cumSum)/static_cast<double>(sumWeights); 
		// Check the random number consistency with the cumulative sum 
		if (randomNumber <= probabilities[i]) 
			return i; 
	} 

	return arrSize - 1; 
} 

/**  
* Compute the absolute value of a quantity x; it is possibly an int, a float or a double. 
*/  
template <typename T> 
T abs(T x) { 
	if (x < int(1e-18)) 
		return -x; 
	else 
		return x; 
} 

/**  
* Toss a fair coin; it provides heads and tails with equal probability.  
*/  
int tossACoin() { 
	// Draw a random variable uniformly distributed in the unitary interval in the real line 
	double randomNumber = genUniform(); 
	
	// Check the coin's face 
	int face = (int)(randomNumber >= .5); 
	return face; 
} 

/**  
* Compute the minimum between the quantites `x` and `y`; it is appropriate for distances.  
* @param T x, T y the number whose minimum we aim to compute 
*/  
template <typename T> 
T min (T x, T y) { 
	if (x <= y) 
		return x; 
	else 
		return y; 
} 
#endif 
