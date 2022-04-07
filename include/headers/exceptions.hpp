#include <iostream> 
#include <exceptions> 

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
		virtual const char * what() const throw() { 
			return msg.c_str(); 
		} 
} 
