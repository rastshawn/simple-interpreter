#ifndef TOKEN
#define TOKEN
#include <string>

enum Type {
	VARIABLE, SEMICOLON, PLUS, MINUS, ASTERISK, PRINT,
	SLASH, PLUS_EQUALS, MINUS_EQUALS, ASTERISK_EQUALS,
	SLASH_EQUALS, EQUALS, INT_LITERAL, STRING_LITERAL,
	UNSET
};

class Token {
	private:
		bool isNumber(std::string tokenString);
	public: 
		Type type;
		std::string raw_string;
		void* value;
		Token(Type t);
		Token(std::string tokenString);
		Token();
		void print();
		void setValue(void* value);
		
		
	
};
#endif
