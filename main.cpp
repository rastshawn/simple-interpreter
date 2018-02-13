/*
 * main.cpp
 * 
 * Copyright 2018 Shawn Rast <rastshawn@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "token.h"
#include "symbol.h"

//#include "token.cpp"

using std::string;
using std::ifstream;
using std::vector;

void readFile(string filename);
void parseLine(string line);
vector<Token> tokenize(string line);
void runLine(vector<Token> tokens, int numTimes);
Symbol getSymbol(Token t);
void setSymbol(string name, void* value, VariableType t); 
void runEquation(vector<Token> tokens);

void error();

int line_counter = 0;

int main(int argc, char **argv)
{
	if (argc > 1)
		readFile(string(argv[1]));
	else 
		readFile("prog/prog1.zpm");

	return 0;
}

void readFile(string filename) {
	string line;
	ifstream source(filename);
	while (std::getline(source, line)){
		line_counter++;
		parseLine(line);
	}	
	
	source.close();
	
}

void parseLine(string line) {
	//std::cout << line << std::endl;
	//std::cout << line << std::endl;
	vector<Token> tokens = tokenize(line);
	runLine(tokens, 1);

}

vector<Token> tokenize(string line) {
	vector<Token> tokens;
	char currentToken [256];
	int counter = 0;
	for (unsigned i = 0; i<line.size(); i++){
		currentToken[counter++] = line[i];
		
	
		if(line[i] == ' ' || i == line.size()-1) {
			if (line[i] == ';') 
				currentToken[counter] = '\0';
			else
				currentToken[counter-1] = '\0';
			counter = 0;
			string tokenString(currentToken);
			Token t(tokenString);
			tokens.push_back(t);	
				
		} 
		
	}
	
	
	return tokens;
}

void runLine(vector<Token> tokens, int numTimes){
	bool printTokens = false; // for debugging
	if (tokens.size() == 0) return;
	int numSeen = 0;
	for (int i = 0; i<numTimes; i++){
		if (printTokens) {
			for (unsigned j = 0; j<tokens.size(); j++){
				tokens[j].print();
			}
			std::cout << std::endl;
		}
		
		switch(tokens[0].type) {
			case VARIABLE:
				runEquation(tokens);
				numSeen = 4;
				break;
			case PRINT:
				numSeen = 3;
				switch(tokens[1].type) {
					case VARIABLE: 
					{
						Symbol s = getSymbol(tokens[1]);
						if (s.type == UNDEFINED) error();
						
						std::cout << s.name << " : ";
						if (s.type == INT) {
							int val = *(int*)s.value;
							std::cout << val;
							
						} else if (s.type == STRING){
							string val = *(string*)s.value;
							std::cout << val;
						}
						
						std::cout << std::endl;
						
					} 
					break;
					case INT_LITERAL:
					{
						int val = *(int*)tokens[1].value;
						std::cout << val << std::endl;
						
						
					}
					break;
					case STRING_LITERAL:
					{
						string val = *(string*)tokens[1].value;
						std::cout << val << std::endl;
					}
					break;
					default: 
					{
						std::cout << "line 157 default err" << std::endl;
						error();
					}
				}
				
				break;
			case FOR:
			{
				if (tokens[tokens.size()-1].type != ENDFOR) error();
				if (tokens[1].type != INT_LITERAL) error();
				// make new subset of tokens, re-call line method
				vector<Token> newTokens;
				for (unsigned i = 2; i<tokens.size()-1; i++) {
					newTokens.push_back(tokens[i]);
				}
				runLine(newTokens, *(int*)tokens[1].value);
			
				
			}
			default:
			
				break;
		}
		
		
		// the first part only handles each individual statement. 
		// it tracks how many tokens it's seen, and then reruns with the remaining tokens
		// this only applies if the first token is NOT a for, as for takes care of that on its own
		
		if (tokens[0].type != FOR){
			vector<Token> newTokens;
			for (unsigned i = numSeen; i<tokens.size(); i++){
				newTokens.push_back(tokens[i]);
			}
			runLine(newTokens, 1);
		}
	}
}
void runEquation(vector<Token> tokens) {
	// first argument must be a variable
	// middle arg must be an op
	// last argument can be either a variable or a literal
	
	Symbol first = getSymbol(tokens[0]);
	void* value;
	VariableType valueType;
	switch(tokens[2].type) {
		case VARIABLE:
		{
			Symbol second = getSymbol(tokens[2]);
			valueType = second.type;
			value = second.value;
			break;
		}
		case INT_LITERAL:
			valueType = INT;
			value = tokens[2].value;
			break;
		case STRING_LITERAL:
			valueType = STRING;
			value = tokens[2].value;
			break;
		default:
			std::cout << "line 207 default err" << std::endl;
			error();
			break;
	}
	
	if (valueType == UNDEFINED) {
		std::cout << "line 213 value type undefined" << std::endl;
		error();
	}
	if (first.type != UNDEFINED && first.type != valueType && tokens[1].type != EQUALS) error();
	switch(tokens[1].type) {
		case EQUALS:
			setSymbol(first.name, value, valueType);
			break;
		case PLUS_EQUALS: 
		{
			if (first.type == STRING) {
				char buf[512];
				std::string firstString(*(std::string*)first.value);
				unsigned i;
				for (i = 0; i<firstString.size(); i++) {
					buf[i] = firstString[i];
				}
				std::string secondString(*(std::string*)value);
				for (unsigned j=0; j<secondString.size(); j++){
					buf[i++] = secondString[j];
				}
				buf[i] = '\0';
				//delete first.value;
				std::string* newVal = new std::string(buf);
				setSymbol(first.name, newVal, valueType);
				
			} else {
				int newVal = *(int*)first.value;
				
				newVal += *(int*)value;
				setSymbol(first.name, &newVal, valueType);
			}
			
		} break;
		case MINUS_EQUALS:
		{
			if (first.type == STRING) error();
			int newVal = *(int*)first.value;
			newVal -= *(int*)value;
			setSymbol(first.name, &newVal, valueType);
			
		} break;
		case ASTERISK_EQUALS:
		{
			if (first.type == STRING) error();
			int newVal = *(int*)first.value;
			newVal *= *(int*)value;
			setSymbol(first.name, &newVal, valueType);
			
		} break;
		default: 
		{
			std::cout << "line 262 default err" << std::endl;
			error();
			break;
		}
	}

}


vector<Symbol> symbols;
Symbol getSymbol(Token t) {
	bool found = false;
	int index = -1;
	for (unsigned i = 0; i<symbols.size(); i++){
		if (symbols[i].t.raw_string == t.raw_string) {
			found = true;
			index = i;
			break;
		}
	}
	
	if (!found) {
		Symbol newSymbol(t);
		symbols.push_back(t);
		index = symbols.size() - 1;
	}
	return symbols[index];
};

void setSymbol(string name, void* value, VariableType t){
	// pass all the things by value
	
	void* newValue;
	if (t == INT) {
		
		int* i = new int;
		*i = *(int*)value;
		newValue = i;
	} else if (t == STRING) {
		string* s = new string(*(string*)value);
		newValue = s;
	}
	bool found = false;
	int index = -1;
	for (unsigned i = 0; i<symbols.size(); i++){
		if (symbols[i].name == name) {
			found = true;
			index = i;
			break;
		}
	}
	
	if (!found) {
		error();
	} else {
		
		if (symbols[index].type == INT){
			delete (int*)symbols[index].value;
		} else if (symbols[index].type == STRING){
			delete (string*)symbols[index].value;
		}
		
		symbols[index].value = newValue;
		symbols[index].type = t;
	}
};

void error() {
	// somehow stop instruction
	std::cout << "RUNTIME ERROR: LINE " << line_counter << std::endl;
	std::exit(1);
	
}
