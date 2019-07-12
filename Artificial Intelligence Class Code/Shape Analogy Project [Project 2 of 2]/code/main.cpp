
#include <iostream>
#include <string>
#include <vector>
#include <vector>

#include "ElectronicMind.h"

using namespace std;


int main(int argc, const char *argv[]) {

	//INPUT and File System Set up//
	string Base_name = argv[1];
	string Input_directory = argv[2];
	string Output_directory = argv[3];

	SubconsciousMemory electricSheep;

	Brain replicant(&electricSheep);
	replicant.View(Input_directory + "/" + Base_name + ".txt");
	replicant.Think();
	replicant.Answer();
	replicant.Communicate(Output_directory + "/" + Base_name + "_solution.txt");
}