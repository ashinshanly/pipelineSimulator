#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <math.h>

#include "execution.h"
#include "parser.h"
#include "hazard_parser.h"
#include "printer.h"

using namespace std;

unsigned int predictiveStallNum = 7;
unsigned int maxCycles = 16;

int main(int argc, char* argv[])
{
	int tRegs[10], sRegs[8];
	for (int i = 0; i < 10; i++) {
		tRegs[i] = 0;
		if(i < 8)
			sRegs[i] = 0;
	}
	
	bool forwarding = *argv[1] == 'F';

	print_start(forwarding);

	ifstream inputstream(argv[2]);
	vector<string> instructions;
	string temp, temp2;
	int labelcount = 0;
	int lineCount = 0;
	while(inputstream >> temp){
		if(isLabel(temp)){
			labelLine(temp, lineCount); 
			instructions.push_back(temp);
			labelcount++;
		} else {
			inputstream >> temp2;
			instructions.push_back(temp + ' ' + temp2);
			lineCount++; 
		}
	}

	vector<vector<int> > pipeline;	  
	vector<string> pipeinstructions;  
	int controlLimit = -1;										//control hazard
	unsigned int total = instructions.size() - labelcount + 4;  
	unsigned int cycle = 0;										
	unsigned int instructionIterator = 0;						
	while(cycle != total && cycle < maxCycles){
		print_line();
		if(!forwarding){
			
			print_cycle();
		}
		else
		{
			cout<<"Executing Instructions"<<"\n";
		}
		
		
		bool stallHazard = false;

		unsigned int curPipeSize = pipeline.size();

		
		vector<int> inst;
		for (unsigned int i = 0; i < curPipeSize && (controlLimit == -1 || (int)i < controlLimit) && i < maxCycles; i++) {
			//cout << pipeline[i][cycle - 1] << '\n';

			unsigned int hazard_offset = 0;

			if (pipeline[i][cycle - 1] + 1 == 2 && pipeinstructions[i] != "nop") {//only on ID check for hazards

				int realLines = 0;

				//hazard detection (only when forwarding is false)
				for (int j = i - 1; j >= 0 && !forwarding; j--) {

					if (pipeinstructions[j] != "nop") {
						realLines++;

						unsigned int difference = 3 - (i - (i-realLines));

					    bool hazardFound = dataHazard(pipeinstructions[i], pipeinstructions[j]);

						if (hazardFound && difference > hazard_offset && realLines <= 2 && pipeline[j][cycle] != 5) {
							//cout<<"Hazard!\n";
							stallHazard = true;
							
							
							hazard_offset = difference;
							if (pipeinstructions[i-1] != "nop") { 
								//cout<<"Hazard11111!\n";
								for(unsigned int addNop = 0; addNop < hazard_offset ; addNop++) { 
									pipeinstructions.insert(pipeinstructions.begin() + i, "nop");

									pipeline.insert(pipeline.begin() + i, vector<int>());

									for (unsigned int k = 0; k < maxCycles; k++)
										pipeline[i].push_back(5);

									pipeline[i][cycle - 2] = 0;
									pipeline[i][cycle - 1] = 1;
									pipeline[i][cycle] = 6;
								}
								i += hazard_offset;
								curPipeSize += hazard_offset;
								total += hazard_offset;
							}
						}
						
					}
				}
			}

			if (hazard_offset == 0) { 

				for (int j = i - 1; j >= 0 && forwarding; j--) {
					bool hazardFound = dataHazard(pipeinstructions[i], pipeinstructions[j]);

					if (hazardFound) {
							inst.push_back(i);
					}		

				}
				if (pipeinstructions[i] != "nop") {
					
					if (pipeline[i][cycle - 1] == 3) {
						bool jump = parse(pipeinstructions[i], sRegs, tRegs);
						if (jump) { 

							for (unsigned int j = i + 1; j < pipeinstructions.size(); j++)
								pipeline[j][cycle] = predictiveStallNum + (pipeline[j][cycle-1]); 

							int labelline = labelParse(pipeinstructions[i]);

							int curSize = pipeinstructions.size();
							//if (controlLimit != -1)
							//	curSize = controlLimit;

							for (unsigned int j = labelline; j < instructions.size(); j++) {
								if (!isLabel(instructions[j])) {
									pipeinstructions.push_back(instructions[j]); 
									total += 1;
									
									pipeline.push_back(vector<int>());
									for (unsigned int k = 0; k < maxCycles; k++)
										pipeline[pipeline.size() - 1].push_back(5);
								}
							}
							pipeline[curSize][cycle] = 0;
							controlLimit = curSize++;
						}
					}
					if (pipeline[i][cycle] >= (int)predictiveStallNum) {}
					else if (pipeline[i][cycle - 1] < 4 && !stallHazard)
						pipeline[i][cycle] = pipeline[i][cycle - 1] + 1;
					else if (pipeline[i][cycle - 1] < 4 && stallHazard)
						pipeline[i][cycle] = pipeline[i][cycle - 1];
					else if (pipeline[i][cycle - 1] >= (int)predictiveStallNum && pipeline[i][cycle - 1]  < 10 && !stallHazard)
						pipeline[i][cycle] = pipeline[i][cycle - 1] + 1;

					int doublestall = i >= 2 && pipeline[i - 2][cycle] == 6;
					int checkcycle = 4 -  doublestall;

					if (i >= 1 && pipeline[i - 1][cycle] == 6 && pipeline[i][cycle] >= checkcycle && !stallHazard)
						pipeline[i - 1][cycle] = 5;
					if (i >= 2 && pipeline[i - 2][cycle] == 6 && pipeline[i][cycle] >= checkcycle && !stallHazard)
						pipeline[i - 2][cycle] = 5;
				}
				else if (pipeline[i][cycle - 1] == 6)
					pipeline[i][cycle] = 6;
			}
			else if (pipeline[i][cycle - 1] + 1 == 1)
				pipeline[i][cycle] = 1;
			else
				pipeline[i][cycle] = pipeline[i][cycle - 1]; //stall
		}

	
		unsigned int lineCount = 0;
		while (lineCount < pipeline.size()) {
			if (isLabel(pipeinstructions[lineCount]))
				labelLine(temp, lineCount);
			lineCount++;
		}

		while(instructionIterator < instructions.size() && isLabel(instructions[instructionIterator])) 
			instructionIterator++;

		if (instructionIterator < instructions.size()) {
			pipeinstructions.push_back(instructions[instructionIterator]);
			pipeline.push_back(vector<int>());

			for (unsigned int i = 0; i < maxCycles; i++)
				pipeline[pipeline.size() - 1].push_back(5);
			pipeline[pipeline.size() - 1][cycle] = 0;
		}

		if (controlLimit != -1 && controlLimit < (int)pipeline.size())
			pipeline[controlLimit][cycle] = 0;

		if(forwarding){
			for (unsigned int i = 0; i < pipeline.size() && (controlLimit == -1 || (int)i <= controlLimit) && i < 16; i++) {
				auto it=find(inst.begin(),inst.end(),i);
				if(it!=inst.end()){
						cout << pipeinstructions[i]<<","<<"$L3"<<"\n";
				}
			
				else
				{
					cout << pipeinstructions[i]<<"\n";
				}
			}
			
		}
		else
		{
			print_pipeline(pipeinstructions, pipeline, controlLimit);
		}
		
		
		inst.clear();
		if (controlLimit != -1 && controlLimit < (int)pipeline.size())
			controlLimit++;

		cycle++;
		instructionIterator++;
		print_regs(sRegs, tRegs);
	}

	print_end();
}
