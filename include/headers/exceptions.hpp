#ifndef EXCEPTIONS_H 
#define EXCEPTIONS_H 
#include <iostream> 
#include <exception>  

class AntNotFound: public std::exception { 
	private: 
		std::string msg; 
	public: 
		/**  
		* Constructor method for `AntNotFound`.  
		*/  
		AntNotFound(std::string raiseMsg) 
			: msg(raiseMsg) 
			{} 
			
		/**  
		* Assert that an ant was not found in the circumstances. 
		*/  
		virtual const char * what() const noexcept override { 
			return msg.c_str(); 
		} 
}; 

class BorderError: public std::exception { 
	private: 
		std::string msg; 
	public: 
		/**  
		* Constructor method for `BorderError`.  
		*/  
		BorderError(std::string raiseMsg)
			: msg(raiseMsg) 
			{} 

		/**  
		* Check whether the coordinates are contemplated in the map.  
		*/  
		virtual const char * what() const noexcept override { 
			return msg.c_str(); 
		} 
}; 
#endif 
