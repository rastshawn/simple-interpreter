#include "symbol.h"

Symbol::Symbol(Token t) {
	this->t = t;
	this->name = t.raw_string;
	this->type = UNDEFINED;
}
