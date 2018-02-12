#ifndef SYMBOL
#define SYMBOL

#include <string>
#include "token.h"

enum VariableType {
	STRING, 
	INT,
	UNDEFINED
};

class Symbol {
	private:
	
	public:
		Token t;
		std::string name;
		void* value;
		VariableType type;
		
		Symbol(Token t);
	
};
#endif
