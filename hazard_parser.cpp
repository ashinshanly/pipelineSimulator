#include "hazard_parser.h"
#include <string>
using namespace std;

bool dataHazard(string currentLine, string prevLine) {
	
	
	if (prevLine == "nop" || currentLine == "nop")
		return false;

	
	string dest_str = prevLine.substr(prevLine.find("$"), prevLine.find(",") - prevLine.find_first_of("$"));

	string instruction = currentLine.substr(0, currentLine.find(" "));

	if (instruction == "sub" || instruction == "add" || instruction == "or" || instruction == "and") {

		string reg1 = currentLine.substr(currentLine.find(",") + 1, currentLine.find_last_of(",") - (currentLine.find(",") + 1));
		string reg2 = currentLine.substr(currentLine.find_last_of("$"));

		return dest_str == reg1 || dest_str == reg2;
	}
	else if (instruction == "addi" || instruction == "laod" || instruction == "store" || instruction == "andi") {
		string reg1 = currentLine.substr(currentLine.find(",")+1, currentLine.find_last_of(",") - (currentLine.find(",") + 1));

		return dest_str == reg1;
	}
	else if (instruction == "bne" || instruction == "beq") {
		string reg1 = currentLine.substr(currentLine.find("$"), currentLine.find(",") - currentLine.find("$"));
		string reg2 = currentLine.substr(currentLine.find_last_of("$"), currentLine.find_last_of(",") - currentLine.find_last_of("$"));

		return dest_str == reg1 || dest_str == reg2;
	}
	
	return false;
}
