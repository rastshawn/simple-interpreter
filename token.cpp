
#include "token.h"
#include <string>
#include <iostream>
Token::Token(Type t) {
	this->type = t;
	this->raw_string = "UNDEFINED";
}

bool Token::isNumber(std::string tokenString) {
	for (unsigned i = 0; i<tokenString.length(); i++) 
		if (!std::isdigit(tokenString[i])) return false;
	return true;
}

void Token::setValue(void* value) {
	this->value = value;
}

Token::Token(){
	
}
void Token::print(){
	bool printToken = false; // for debugging only
	
	if (printToken) {
		std::cout << raw_string << ", ";
	}
	std::string typeString;
	
	switch (this->type) {
		case VARIABLE: typeString = "VARIABLE"; break; 
		case SEMICOLON: typeString = "SEMICOLON"; break;
		case PLUS: typeString = "PLUS"; break;
		case MINUS: typeString = "MINUS"; break;
		case ASTERISK: typeString = "ASTERISK"; break;
		case PRINT: typeString = "PRINT"; break;
		case SLASH: typeString = "SLASH"; break;
		case PLUS_EQUALS: typeString = "PLUS_EQUALS"; break;
		case MINUS_EQUALS: typeString = "MINUS_EQUALS"; break; 
		case ASTERISK_EQUALS: typeString = "ASTERISK_EQUALS"; break;
		case SLASH_EQUALS: typeString = "SLASH_EQUALS"; break; 
		case EQUALS: typeString = "EQUALS"; break; 
		case INT_LITERAL: typeString = "INT_LITERAL"; break; 
		case STRING_LITERAL: typeString = "STRING_LITERAL"; break;
		default: 
			std::cout << "hit base case";
			break;
	}
	if (printToken)
		std::cout << typeString << std::endl;
	
}

Token::Token(std::string tokenString) {
	// makes proper token item by deciphering string
	int tokenLength = tokenString.length();
	this->type = UNSET;
	
	this->raw_string = tokenString;
	if (tokenLength == 1) {
		switch(tokenString[0]){
			case '=': this->type = EQUALS; break;
			case ';': this->type = SEMICOLON; break;
			case '+': this->type = PLUS; break;
			case '-': this->type = MINUS; break;
			case '/': this->type = SLASH; break;
			case '*': this->type = ASTERISK; break;
			default: // variable or int literal
				if (isNumber(tokenString)){
					this->type = INT_LITERAL;
				} else {
					this->type = VARIABLE;
				}
				break;
		}
	} else if (tokenLength == 2) {
		// either +=, variable name, or int literal
		if (tokenString[1] == '=') {
			switch(tokenString[0]){
				case '+': this->type = PLUS_EQUALS; break;
				case '-': this->type = MINUS_EQUALS; break;
				case '/': this->type = SLASH_EQUALS; break;
				case '*': this->type = ASTERISK_EQUALS; break;
				default:
					// something is syntactically wrong
					std::cout << tokenString[0] << "= is not a valid token\n";
			}
		}
	} if (this->type == UNSET) {
		
		// either int literal, variable, string literal, or PRINT
		if (tokenString[0] == '"') {
			// should be string literal
			this->type = STRING_LITERAL;
		} else if (isNumber(tokenString)){
			this->type = INT_LITERAL;
		} else if (tokenString == "PRINT") {
			this->type = PRINT;
		} else {
			// must be variable
			this->type = VARIABLE;
		}
	}
	
	// set value
	if (this->type == INT_LITERAL){
		int* i = new int;
		*i = std::stoi(tokenString);
		this->setValue(i);
	} else if (this->type == STRING_LITERAL){
		char buf[256];
		unsigned i;
		for (i=1; i<tokenString.size() - 1; i++){
			buf[i-1] = tokenString[i];
		}
		buf[i-1] = '\0';
		std::string* s = new std::string(buf);
		this->setValue(s);
	}
}
