//
//  main.cpp
//
using namespace std;

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "diagram_shape.h"
#include "diagram_processing.h"
#include "io.h"


bool diagComp(DiagramShape a, DiagramShape b){

    if(a.ids.size() < b.ids.size()){

        return true;
    }else if(a.ids.size() > b.ids.size()){

        return false;
    }else{

        string compString1="", compString2="";

        for(int i = 0; i < a.ids.size(); ++i){

            compString1+=a.ids[i];

            compString2+=b.ids[i];
        }

        sort(compString1.begin(), compString1.end());
        sort(compString2.begin(), compString2.end());

        return compString1 < compString2;
    }


}

bool interpComp(vector<DiagramShape> a, vector<DiagramShape> b){


    if(a.size() < b.size()){

        return true;
    }else if(a.size() > b.size()){

        return false;
    }else{

        sort(a.begin(), a.end(), diagComp);
        sort(b.begin(), b.end(), diagComp);

        for(int i = 0; i < a.size(); ++i){

        	if(diagComp(a[i], b[i])){

        		return true;
        	}

        	if(diagComp(b[i], a[i])){

        		return false;
        	}

        }

        return false;

    }

}

vector<int> inputCircle(string parseData){

	vector<int> result;
	//cout << "parseData: " << parseData << endl;
	int pos;
	string tempNum;
	stringstream ss;
	ss.str(parseData);
	getline(ss, tempNum, '(');
	getline(ss, tempNum, ',');

	result.push_back(stoi(tempNum));
	getline(ss, tempNum, ',');

	result.push_back(stoi(tempNum));
	getline(ss, tempNum, ',');
	//cout << tempNum << endl;

	//get the final number, seperated by the parenthetical
	getline(ss, tempNum, ')');
	result.push_back(stoi(tempNum));

	result.resize(4);
	return(result);

}

vector<int> inputLine(string parseData){

	vector<int> result;
	//cout << "parseData: " << parseData << endl;
	int pos;
	string tempNum;
	stringstream ss;
	ss.str(parseData);
	getline(ss, tempNum, '(');
	getline(ss, tempNum, ',');

	result.push_back(stoi(tempNum));
	getline(ss, tempNum, ',');

	//get the numbers seperated by commas
	for(int i = 1; i < 3; ++i){

		result.push_back(stoi(tempNum));
		getline(ss, tempNum, ',');
		//cout << tempNum << endl;
	}

	//get the final number, seperated by the parenthetical
	getline(ss, tempNum, ')');
	result.push_back(stoi(tempNum));

	result.resize(4);
	return(result);

	
}

vector<int> inputDot(string parseData){

	vector<int> result;
	//cout << "parseData: " << parseData << endl;
	int pos;
	string tempNum;
	stringstream ss;
	ss.str(parseData);
	getline(ss, tempNum, '(');
	getline(ss, tempNum, ',');

	result.push_back(stoi(tempNum));

	//get the final number, seperated by the parenthetical
	getline(ss, tempNum, ')');
	result.push_back(stoi(tempNum));

	return(result);
	
}



int main(int argc, const char *argv[]) {
    // *** IMPLEMENT ***   
    // - take in input, find shapes, output

	//INPUT and File System Set up//
	ifstream inputFile(argv[1]);
	string outputFolder(argv[2]);
	string outputFile(argv[3]);

	//////////////////////////////

	//Set up array of primitives, as well as names of primitives
	primitivesStruct DataManager;
	//////////////////////////////

	//Start collecting and parsing data from input file
	string temp;
	while(inputFile >> temp){
	
		if(temp.at(0) == (char)'s'){

			DataManager.lineID.push_back(temp);
			getline(inputFile, temp);
			DataManager.lines.push_back(inputLine(temp));

		}else if(temp.at(0) == (char)'c'){

			DataManager.circleID.push_back(temp);
			getline(inputFile, temp);
			DataManager.circles.push_back(inputCircle(temp));

		}else if(temp.at(0) == (char)'d'){

			DataManager.dotID.push_back(temp);
			getline(inputFile, temp);
			DataManager.dots.push_back(inputDot(temp));
		}
	}

	//if that while loop works, this should populate the vectors
	//hooray. it works.

	//Now, we need to at least populate a vector with the most simple solution possible.
	//First, let us declare all of the output variables.


	vector< vector< DiagramShape > > totalInterpretations;



	//run the function I created that finds the most simple solution.

	/*findSimplePolygons(DataManager, interpretation);
	interpretCircles(DataManager, interpretation);
	interpretDots(DataManager, interpretation);
	giveIDsToShapes(interpretation);
	setCentersForAll(interpretation);

	totalInterpretations.push_back(interpretation);

	//This needs a loop here that goes through all of the different interpretations.
	//cout << totalInterpretations.size() << endl;
	findRelations(totalInterpretations);

	printOutput(totalInterpretations, outputFolder + outputFile);
	*/

	createMap(DataManager);

	for(int i = 0; i < DataManager.structuredMap.pointList.size(); ++i){


		findAllShapesFromOnePoint(DataManager, i);

	}

	for(int i = 0; i < DataManager.shapeHash.size(); ++i){

		vector< DiagramShape> interpretation;
		pairShapes(DataManager, interpretation, i);
		//cout << interpretation.size() << endl << endl;
		totalInterpretations.push_back(interpretation);

	}

	//cout << totalInterpretations.size() << endl;

	bool(*fn_pt)(vector<DiagramShape>, vector<DiagramShape>) = interpComp;
	//sort(totalInterpretations.begin(), totalInterpretations.end(), interpComp);
	set< vector<DiagramShape>, bool(*)(vector<DiagramShape>, vector<DiagramShape>)> uniqueInterps(totalInterpretations.begin(), totalInterpretations.end(), fn_pt);

	//cout << uniqueInterps.size() << endl;

	totalInterpretations.clear();

	totalInterpretations = vector< vector<DiagramShape> >(uniqueInterps.begin(), uniqueInterps.end());

	//cout << totalInterpretations.size() << endl;


	for(int i = 0; i < totalInterpretations.size(); ++i){

		interpretCircles(DataManager, totalInterpretations[i]);
		interpretDots(DataManager, totalInterpretations[i]);
		setCentersForAll(totalInterpretations[i]);
		giveIDsToShapes(totalInterpretations[i]);
		findRelationsInSingleDiagram(totalInterpretations[i]);

	}

	printOutput(totalInterpretations, outputFolder + outputFile);



    return 0;
}
