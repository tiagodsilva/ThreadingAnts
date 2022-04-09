#ifndef __PARSER__ 
#define __PARSER__ 
#include <iostream> 
#include <vector> 
#include <algorithm> 

// From https://stackoverflow.com/questions/865668
class InputParser { 
	private: 
		// A vector containing the tokens in the command line 
		std::vector<std::string> * tokens; 

	public: 
		/**  
		* Constructor method for `InputParser`; it is convenient to update 
		* the parametrizable attributes in the game 
		* @param int &argc the quantity of options in the parsed line  
		* @param char **argv the actual strings in the command line 
		*/  
		InputParser(int &argc, char **argv)
	       	: tokens(new std::vector<std::string>)	{ 
			for (int i = 1; i < argc; i++) 
				tokens->push_back(std::string(argv[i])); 
		}  
		
		/**  
		* Parse the option `option`; it could be, for instance, `nAnts` for 
		* the quantity of ants in the game  
		* @param const std::string &option the option we aim to parse 
		* @param const std::string defValue the default value for the option 
		* @return std::string value the value correspondent to the parsed option 
		*/  
		const std::string parse(const std::string &option, 
				const std::string defValue) const { 
			std::vector<std::string>::const_iterator itr; 
			// From "algorithm"  
			itr = std::find(tokens->begin(), tokens->end(), option); 

			if (itr != tokens->end() && ++itr != tokens->end()) { 
				return *itr; 
			} 
			
			// Assert that there parsing option, `option`, is inappropriate; 
			// in this case, use the default 
			return defValue; 
		} 

		/**  
		* Check whether the option `option` was provided; it is convenient 
		* to display help messages, for instances.  
		* @param const std::string &option the option whose existence we aim to assert 
		*/  
		bool optExists(const std::string &option) const { 
			return std::find(tokens->begin(), tokens->end(), option) 
				!= tokens->end(); 
		} 
}; 
#endif 
