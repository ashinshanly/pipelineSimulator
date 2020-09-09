#include "execution.h"
#include <iostream>

void add_(int* destRegister, int* leftRegister, int* rightRegister) {   //////////
	*destRegister = *leftRegister + *rightRegister;
}

void load_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister + immediate;
}
void store_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister + immediate;
}
void sub_(int* destRegister, int* leftRegister, int* rightRegister) {   //////////
	*destRegister = *leftRegister - *rightRegister;
}

void addi_(int* destRegister, int* leftRegister, int immediate) { /////////////
	*destRegister = *leftRegister + immediate;
}

void and_(int* destRegister, int* leftRegister, int* rightRegister) {
	*destRegister = *leftRegister & *rightRegister;
}

void andi_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister & immediate;
}

void or_(int* destRegister, int* leftRegister, int* rightRegister) {
	*destRegister = *leftRegister | *rightRegister;
}

void ori_(int* destRegister, int* leftRegister, int immediate) {
	*destRegister = *leftRegister | immediate;
}

bool beq_(int* leftRegister, int* rightRegister) {   ///////////////
	return *leftRegister == *rightRegister;
}


bool bne_(int* leftRegister, int* rightRegister) {
	return *leftRegister != *rightRegister;
}
