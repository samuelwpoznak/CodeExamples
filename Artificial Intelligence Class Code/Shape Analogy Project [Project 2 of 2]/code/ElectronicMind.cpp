#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "ElectronicMind.h"

using namespace std;

size GetSizeOfShapeView(string input){


	if(input.at(0) == 's'){
		return size::SMALL;
	}else if (input.at(0) == 'm'){
		return size::MEDIUM;
	}else if(input.at(0) == 'l'){
		return size::LARGE;
	}


}

void OrderShapePair(ShapePair &result){


	if(result.shape2->GetID().at(0) == 'A'){

		Shape* temp = result.shape2;
		result.shape2 = result.shape1;
		result.shape1 = temp;
	}else if(result.shape1->GetID().at(0) == 'K'){

		Shape* temp = result.shape1;
		result.shape1 = result.shape2;
		result.shape2 = temp;
	}

	return;

}

void OrderInterpPair(InterpretationPair &result){

	if(result.ptr2->GetID().at(0) == 'A'){

		Interpretation* temp = result.ptr2;
		result.ptr2 = result.ptr1;
		result.ptr1 = temp;
	}else if(result.ptr2->GetID().at(0) == 'K'){

		Interpretation* temp = result.ptr1;
		result.ptr1 = result.ptr2;
		result.ptr2 = temp;
	}

	return;

}

//Shapes are determined to be similar based on the following metric:
// 1. The type of shape
// 2. The size of shape
// 3. The location of shape
// In that order
// A "1" means the shapes are the exact same.
double RankSimilarity(Shape* A, Shape* B){

	double result = 1;

	if(A->GetTypeOf() == B->GetTypeOf()){

		result = result;
	}else{


		result = result - result/2;
	}

	if(A->GetSizeOf() == B->GetSizeOf()){


		result = result;
	}else{

		result = result - result/4;
	}

	if(A->GetCenterX() == B->GetCenterX()){


		result = result;
	}else{

		double dif = abs(A->GetCenterX() - B->GetCenterX());
		result = result - ((result/8)*(dif/100));
	}

	if(A->GetCenterY() == B->GetCenterY()){

		result = result;
	}else{

		double dif = abs(A->GetCenterY() - B->GetCenterY());
		result = result - ((result/8)*(dif/100));
	}

	return result;

}

//Shapes are determined to be similar based on the following metric:
// 1. The location of shape
// 2. The size of shape
// 3. The type of shape
// In that order
// A "1" means the shapes are the exact same.
double RankAnalogy(Shape* A, Shape* B){

	double result = 1;

	if(A->GetCenterX() == B->GetCenterX()){

		result = result;
	}else{


		result = result - result/2;
	}

	if(A->GetCenterY() == B->GetCenterY()){


		result = result;
	}else{

		result = result - result/2;
	}

	if(A->GetSizeOf() == B->GetSizeOf()){


		result = result;
	}else{

		result = result - result/4;
	}

	if(A->GetTypeOf() == B->GetTypeOf()){

		result = result;
	}else{

		result = result - result/8;
	}

	return result;

}

//This finds the most similar [not analagous] shape in the interpretation it is given
Shape* FindMostSimilarShapeInInterpretation(Interpretation* ptr, Shape* other){

	double highestSimilarity = 0;
	Shape* mostSimilar = ptr->AccessShapeAt(0);



	for(int i = 0; i < ptr->GetNumShapes(); ++i){

		double currentSimilarity = RankSimilarity(ptr->AccessShapeAt(i), other);
		Shape* currentShape = ptr->AccessShapeAt(i);

		if(currentSimilarity > highestSimilarity){

			highestSimilarity = currentSimilarity;
			mostSimilar = currentShape;
		}

	}

	return mostSimilar;


}

//This finds the most analagous [not similar] shape in the interpretation it is given
Shape* FindMostAnalagousShapeInInterpretation(Interpretation* ptr, Shape* other){

	double highestAnalogy = 0;
	Shape* mostAnalagous = ptr->AccessShapeAt(0);



	for(int i = 0; i < ptr->GetNumShapes(); ++i){

		double currentAnalogy = RankAnalogy(ptr->AccessShapeAt(i), other);
		Shape* currentShape = ptr->AccessShapeAt(i);

		if(currentAnalogy > highestAnalogy){

			highestAnalogy = currentAnalogy;
			mostAnalagous = currentShape;
		}

	}

	return mostAnalagous;


}

//Should be used in ranking whether an interp of A is suitable for transforming into an interp of B
//Important quialities are:
//1: shapes types are similar
//2/3: shapes are similar in size or location
//
//most important for this ranking is that the shape types are the same.
double RankSimilarInterpretations(InterpretationPair pair){

	double result = 0;

	if(pair.ptr1->GetNumShapes() < pair.ptr2->GetNumShapes()){

		for(int i = 0; i < pair.ptr1->GetNumShapes(); ++i){

			Shape* temp = pair.ptr1->AccessShapeAt(i);
			Shape* simShape = FindMostSimilarShapeInInterpretation(pair.ptr2, temp);
			result += RankSimilarity(temp, simShape);

		}

		result = result/pair.ptr1->GetNumShapes();


	}else{

		for(int i = 0; i < pair.ptr2->GetNumShapes(); ++i){

			Shape* temp = pair.ptr2->AccessShapeAt(i);
			Shape* simShape = FindMostSimilarShapeInInterpretation(pair.ptr1, temp);
			result += RankSimilarity(temp, simShape);

		}

		result = result/pair.ptr2->GetNumShapes();



	}
	
	return result;

}

//Should be used in ranking whether an interp of C is a suitable analogy for an interp of A
double RankAnalagousInterpretations(InterpretationPair pair){

	double result = 0;

	if(pair.ptr1->GetNumShapes() < pair.ptr2->GetNumShapes()){

		for(int i = 0; i < pair.ptr1->GetNumShapes(); ++i){

			Shape* temp = pair.ptr1->AccessShapeAt(i);
			Shape* simShape = FindMostAnalagousShapeInInterpretation(pair.ptr2, temp);
			result += RankAnalogy(temp, simShape);

		}

		result = result/pair.ptr1->GetNumShapes();


	}else{

		for(int i = 0; i < pair.ptr2->GetNumShapes(); ++i){

			Shape* temp = pair.ptr2->AccessShapeAt(i);
			Shape* simShape = FindMostAnalagousShapeInInterpretation(pair.ptr1, temp);
			result += RankAnalogy(temp, simShape);

		}

		result = result/pair.ptr2->GetNumShapes();



	}
	
	return result;
}

Brain::Brain(SubconsciousMemory* refSCM){ 

	A.SetSCM(refSCM);
	B.SetSCM(refSCM);
	C.SetSCM(refSCM);
	K1.SetSCM(refSCM);
	K2.SetSCM(refSCM);
	K3.SetSCM(refSCM);
	PredictedAnswer.SetSCM(refSCM);



	A.SetID("A");
	B.SetID("B");
	C.SetID("C");
	K1.SetID("K1");
	K2.SetID("K2");
	K3.SetID("K3");
	PredictedAnswer.SetID("Prediction");
	PredictedAnswer.AddNullInterpretation();

	SCM = refSCM;

}

SubconsciousMemory::SubconsciousMemory(){

	this->AllShapeData.reserve(50);
	return;

}

void SubconsciousMemory::AddShapeToData(Shape other){

	this->AllShapeData.push_back(other);
	return;
}

int SubconsciousMemory::GetDataSize(){

	return this->AllShapeData.size();
}

Shape* SubconsciousMemory::FindShapeByID(string SID){

	for(int i = 0; i < this->AllShapeData.size(); ++i){

		if(SID == AllShapeData[i].GetID()) return &AllShapeData[i];
	}

	return nullptr;
}

void Brain::View(string inputFile){

	ifstream myFile;
	myFile.open(inputFile);
	string temp;
	int counter = 0;

	while(getline(myFile, temp)){
		if(temp.length() == 0) ++counter;
		if(counter != 0) break;

		if(temp.at(0) == 'A'){
			A.ViewInterpretation(myFile);
			counter = 0;
		}else if(temp.at(0) == 'B'){
			B.ViewInterpretation(myFile);
			counter = 0;
		}else if(temp.at(0) == 'C'){
			C.ViewInterpretation(myFile);
			counter = 0;
		}else if(temp.at(1) == '1'){
			K1.ViewInterpretation(myFile);
			counter = 0;
		}else if(temp.at(1) == '2'){
			K2.ViewInterpretation(myFile);
			counter = 0;
		}else if(temp.at(1) == '3'){
			K3.ViewInterpretation(myFile);
			counter = 0;
		}


	}

	return;
}

void Diagram::PushNewInterpretation(Interpretation &other){


	this->interpretations.push_back(other);
}

void Diagram::DeleteIncorrectInterpretations(Interpretation* correct){


	Interpretation temp(*correct);
	ClearAllInterpretations();
	this->PushNewInterpretation(temp);
	return;


}

void Brain::FindSimilarShapesAB(){

	InterpretationPair correctInterpretations;

	correctInterpretations = this->A.Consider(&this->B);
	this->ABPairs.resize(0);

	if(correctInterpretations.ptr1->GetNumShapes() < correctInterpretations.ptr2->GetNumShapes()){


		for(int i = 0; i < correctInterpretations.ptr1->GetNumShapes(); ++i){

			Shape* shp1;
			Shape* shp2;
			shp1 = correctInterpretations.ptr1->AccessShapeAt(i);
			shp2 = FindMostSimilarShapeInInterpretation(correctInterpretations.ptr2, shp1);
			ShapePair shpPr;
			shpPr.shape1 = shp1;
			shpPr.shape2 = shp2;

			OrderShapePair(shpPr);
			ABPairs.push_back(shpPr);


		}


	}else{


		for(int i = 0; i < correctInterpretations.ptr2->GetNumShapes(); ++i){

			Shape* shp1;
			Shape* shp2;
			shp1 = correctInterpretations.ptr2->AccessShapeAt(i);
			shp2 = FindMostSimilarShapeInInterpretation(correctInterpretations.ptr1, shp1);
			ShapePair shpPr;
			shpPr.shape1 = shp1;
			shpPr.shape2 = shp2;

			OrderShapePair(shpPr);
			ABPairs.push_back(shpPr);


		}

	}

	return;


}

void Brain::FindSimilarShapesCK(){

	InterpretationPair correctInterpretations;

	correctInterpretations = this->C.Consider(this->bestAnswer);
	this->CKPairs.resize(0);

	if(correctInterpretations.ptr1->GetNumShapes() < correctInterpretations.ptr2->GetNumShapes()){


		for(int i = 0; i < correctInterpretations.ptr1->GetNumShapes(); ++i){

			Shape* shp1;
			Shape* shp2;
			shp1 = correctInterpretations.ptr1->AccessShapeAt(i);
			shp2 = FindMostSimilarShapeInInterpretation(correctInterpretations.ptr2, shp1);
			ShapePair shpPr;
			shpPr.shape1 = shp1;
			shpPr.shape2 = shp2;

			OrderShapePair(shpPr);
			CKPairs.push_back(shpPr);


		}


	}else{


		for(int i = 0; i < correctInterpretations.ptr2->GetNumShapes(); ++i){

			Shape* shp1;
			Shape* shp2;
			shp1 = correctInterpretations.ptr2->AccessShapeAt(i);
			shp2 = FindMostSimilarShapeInInterpretation(correctInterpretations.ptr1, shp1);
			ShapePair shpPr;
			shpPr.shape1 = shp1;
			shpPr.shape2 = shp2;

			OrderShapePair(shpPr);
			CKPairs.push_back(shpPr);


		}

	}

	return;


}

void Brain::FindAnalagousShapesAC(){


	InterpretationPair correctInterpretations;

	correctInterpretations = this->A.Consider(&this->C);
	this->ACPairs.resize(0);

	if(correctInterpretations.ptr1->GetNumShapes() < correctInterpretations.ptr2->GetNumShapes()){


		for(int i = 0; i < correctInterpretations.ptr1->GetNumShapes(); ++i){

			Shape* shp1;
			Shape* shp2;
			shp1 = correctInterpretations.ptr1->AccessShapeAt(i);
			shp2 = FindMostAnalagousShapeInInterpretation(correctInterpretations.ptr2, shp1);
			ShapePair shpPr;
			shpPr.shape1 = shp1;
			shpPr.shape2 = shp2;

			OrderShapePair(shpPr);
			ACPairs.push_back(shpPr);


		}


	}else{


		for(int i = 0; i < correctInterpretations.ptr2->GetNumShapes(); ++i){

			Shape* shp1;
			Shape* shp2;
			shp1 = correctInterpretations.ptr2->AccessShapeAt(i);
			shp2 = FindMostAnalagousShapeInInterpretation(correctInterpretations.ptr1, shp1);
			ShapePair shpPr;
			shpPr.shape1 = shp1;
			shpPr.shape2 = shp2;

			OrderShapePair(shpPr);
			ACPairs.push_back(shpPr);


		}

	}

	return;


}

vector <Transform> Brain::TheorizeTheChanges(){

	vector <Transform> result;
	for(int i = 0; i < this->ABPairs.size(); ++i){



		OrderShapePair(ABPairs[i]);
		Transform temp(ABPairs[i].shape1, ABPairs[i].shape2);
		result.push_back(temp);


	}

	return result;

}

void Brain::OrderTheShapePairs(){

	for(int i = 0; i < this->ABPairs.size(); ++i){

		for(int j = i+1; j < this->ACPairs.size(); ++j){

			if(ABPairs[i].shape1 == ACPairs[j].shape1){
				ShapePair temp = ACPairs[i];
				ACPairs[i] = ACPairs[j];
				ACPairs[j] = temp;

			}
		}




	}




}

bool Brain::ShapeExistInA_AsPairB(Shape* other){


	for(int i = 0; i < ABPairs.size(); ++i){

		if(other->GetID() == ABPairs[i].shape1->GetID()){

			return true;
		}

	}

	return false;

}

bool Brain::ShapeExistInC_AsPairK(Shape* other){


	for(int i = 0; i < CKPairs.size(); ++i){

		if(other->GetID() == CKPairs[i].shape1->GetID()){

			return true;
		}

	}

	return false;

}

void Brain::CreateNewDiagram(){

	vector <Shape*> shapesInPrediction;

	for(int i = 0; i < ACPairs.size(); ++i){

		if(ShapeExistInA_AsPairB(ACPairs[i].shape1)){


			Shape temp(*ACPairs[i].shape2);
			this->SCM->AddShapeToData(temp);
			Shape* ptr = &this->SCM->AllShapeData.back();
			shapesInPrediction.push_back(ptr);
		}
	}

	this->PredictedAnswer.AccessInterpretationAt(0)->SetShapesVec(shapesInPrediction);

	return;

}

void Brain::TransformNewDiagram(vector <Transform> &transforms){

	for(int i = 0; i < transforms.size(); ++i){

		transforms[i].Apply(this->PredictedAnswer.AccessInterpretationAt(0)->AccessShapeAt(i));
	}

	return;

}

void Brain::PredictAnswer(){


	vector <Transform> allChanges = this->TheorizeTheChanges();

	this->OrderTheShapePairs();
	this->CreateNewDiagram();
	this->TransformNewDiagram(allChanges);


}

double Brain::RankAnswer(InterpretationPair answerPair){

	double result = RankSimilarInterpretations(answerPair);

	if(answerPair.ptr1->GetNumShapes() != answerPair.ptr2->GetNumShapes()){
		result = result/2;
	}
	return result;

}

void Brain::FindBestAnswer(){

	InterpretationPair K1Pred, K2Pred, K3Pred;

	//Get Correct K1 Interp
	K1Pred = this->K1.Consider(&this->PredictedAnswer);


	//Get Correct K2 Interp
	K2Pred = this->K2.Consider(&this->PredictedAnswer);

	//Get Correct K3 Interp
	K3Pred = this->K3.Consider(&this->PredictedAnswer);

	//Now find the rankings of each diagram
	double K1Rank, K2Rank, K3Rank;

	K1Rank = this->RankAnswer(K1Pred);
	K2Rank = this->RankAnswer(K2Pred);
	K3Rank = this->RankAnswer(K3Pred);

	if(K1Rank > K2Rank && K1Rank > K3Rank){

		this->bestAnswer = &this->K1;
		return;

	}else if(K2Rank > K3Rank && K2Rank > K1Rank){

		this->bestAnswer = &this->K2;
		return;

	}else if(K3Rank > K1Rank && K3Rank > K2Rank){

		this->bestAnswer = &this->K3;
		return;

	}else{

		return;
	}
}

void Brain::NarrowADown(){


	InterpretationPair correctInterpretations;
	correctInterpretations = this->A.Consider(&this->B);

	OrderInterpPair(correctInterpretations);
	this->A.DeleteIncorrectInterpretations(correctInterpretations.ptr1);
	return;

}

void Brain::Think(){

	FindSimilarShapesAB();

	NarrowADown();
	FindAnalagousShapesAC();

	PredictAnswer();
	FindBestAnswer();

}

Diagram* Brain::Answer(){

	FindSimilarShapesCK();

	if(!this->bestAnswer){

		return nullptr;
	}else{

		if(this->bestAnswer->GetID().at(1) == '1'){

			return &this->K1;

		}else if(this->bestAnswer->GetID().at(1) == '2'){

			return &this->K2;

		}else{

			return &this->K3;
		}
	}


}

void Brain::AlphabetizeShapePairs(){


	struct less_than_key{

		bool operator() (ShapePair sp1, ShapePair sp2){

			int one, two;
			string temp1, temp2;

			one = sp1.shape1->GetID().find_last_of("_")+1;
			two = sp2.shape1->GetID().find_last_of("_")+1;

			temp1 = sp1.shape1->GetID().substr(one);
			temp2 = sp2.shape1->GetID().substr(two);

			int comp = temp1.compare(temp2);
        	return (comp > 0);
    	}

	};

	sort(this->ABPairs.begin(), this->ABPairs.end(), less_than_key());
	reverse(this->ABPairs.begin(), this->ABPairs.end());

	sort(this->CKPairs.begin(), this->CKPairs.end(), less_than_key());
	reverse(this->CKPairs.begin(), this->CKPairs.end());

	return;


}

void Brain::Communicate(string outputFile){

	ofstream myFile;
	myFile.open(outputFile);




	this->A.AccessInterpretationAt(0)->AlphabetizeShapes();

	InterpretationPair IP = this->C.Consider(this->bestAnswer);

	IP.ptr1->AlphabetizeShapes();


	AlphabetizeShapePairs();

	if(Answer()){

		myFile << "Answer: " << this->bestAnswer->GetID() << endl;
		myFile << endl;


		myFile << "T_AB" << endl;
		myFile << "[";


		int index = 0;
		for(int i = 0; i < this->A.AccessInterpretationAt(0)->GetNumShapes(); ++i){


			if(ShapeExistInA_AsPairB(this->A.AccessInterpretationAt(0)->AccessShapeAt(i))){

				myFile << "(";
				int pos = this->ABPairs[index].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->ABPairs[index].shape1->GetID().substr(pos);
				myFile << simpleID;

				myFile << ",";

				pos = this->ABPairs[index].shape2->GetID().find_last_of("_") + 1;
				simpleID = this->ABPairs[index].shape2->GetID().substr(pos);
				myFile << simpleID;

				myFile << ")";

				if(i != (this->A.AccessInterpretationAt(0)->GetNumShapes()-1)){

					myFile << ",";
				}

				index++;

			}else{

				myFile << "(";
				int pos = this->A.AccessInterpretationAt(0)->AccessShapeAt(i)->GetID().find_last_of("_") + 1;
				string simpleID = this->A.AccessInterpretationAt(0)->AccessShapeAt(i)->GetID().substr(pos);
				myFile << simpleID;

				myFile << ",0)";

				if(i != (this->A.AccessInterpretationAt(0)->GetNumShapes()-1)){

					myFile << ",";
				}



			}
		}

		myFile << "]" << endl;


		//NOW WE NEED TO COMMUNICATE THE TRANSFORMS//

		for(int i = 0; i < this->ABPairs.size(); ++i){


			Transform temp(this->ABPairs[i].shape1, this->ABPairs[i].shape2);

			myFile << "action('keep',";

			int pos = this->ABPairs[i].shape1->GetID().find_last_of("_") + 1;
			string simpleID = this->ABPairs[i].shape1->GetID().substr(pos);
			myFile << simpleID;
			myFile << ")" << endl;

			if(temp.OutPutSizeAction().length() != 0){
				myFile << "action('";
				myFile << temp.OutPutSizeAction() << "',";

				int pos = this->ABPairs[i].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->ABPairs[i].shape1->GetID().substr(pos);
				myFile << simpleID;
				myFile << ")" << endl;

			}

			if(temp.OutPutXPositionAction().length() != 0){
				myFile << "action('";
				myFile << temp.OutPutXPositionAction() << "',";

				int pos = this->ABPairs[i].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->ABPairs[i].shape1->GetID().substr(pos);
				myFile << simpleID;
				myFile << "," << temp.PosXChange;
				myFile << ")" << endl;

			}

			if(temp.OutPutYPositionAction().length() != 0){
				myFile << "action('";
				myFile << temp.OutPutYPositionAction() << "',";

				int pos = this->ABPairs[i].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->ABPairs[i].shape1->GetID().substr(pos);
				myFile << simpleID;
				myFile << "," << temp.PosYChange;
				myFile << ")" << endl;

			}



		}

		myFile << endl;

		////////////////////////////////////////////

		myFile << "T_C" << this->bestAnswer->GetID() << endl;
		myFile << "[";

		index = 0;
		for(int i = 0; i < IP.ptr2->GetNumShapes(); ++i){


			if(ShapeExistInC_AsPairK(IP.ptr2->AccessShapeAt(i))){

				myFile << "(";
				int pos = this->CKPairs[index].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->CKPairs[index].shape1->GetID().substr(pos);
				myFile << simpleID;

				myFile << ",";

				pos = this->CKPairs[index].shape2->GetID().find_last_of("_") + 1;
				simpleID = this->CKPairs[index].shape2->GetID().substr(pos);
				myFile << simpleID;

				myFile << ")";

				if(i != (IP.ptr2->GetNumShapes()-1)){

					myFile << ",";
				}

				index++;

			}else{

				myFile << "(";
				int pos = IP.ptr2->AccessShapeAt(i)->GetID().find_last_of("_") + 1;
				string simpleID = IP.ptr2->AccessShapeAt(i)->GetID().substr(pos);
				myFile << simpleID;

				myFile << ",0)";

				if(i != (IP.ptr2->GetNumShapes()-1)){

					myFile << ",";
				}



			}
		}

		myFile << "]" << endl;

		//NOW WE NEED TO COMMUNICATE THE TRANSFORMS//

		for(int i = 0; i < this->CKPairs.size(); ++i){


			Transform temp(this->CKPairs[i].shape1, this->CKPairs[i].shape2);

			myFile << "action('keep',";

			int pos = this->CKPairs[i].shape1->GetID().find_last_of("_") + 1;
			string simpleID = this->CKPairs[i].shape1->GetID().substr(pos);
			myFile << simpleID;
			myFile << ")" << endl;

			if(temp.OutPutSizeAction().length() != 0){
				myFile << "action('";
				myFile << temp.OutPutSizeAction() << "',";

				int pos = this->CKPairs[i].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->CKPairs[i].shape1->GetID().substr(pos);
				myFile << simpleID;
				myFile << ")" << endl;

			}

			if(temp.OutPutXPositionAction().length() != 0){
				myFile << "action('";
				myFile << temp.OutPutXPositionAction() << "',";

				int pos = this->CKPairs[i].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->CKPairs[i].shape1->GetID().substr(pos);
				myFile << simpleID;
				myFile << "," << temp.PosXChange;
				myFile << ")" << endl;

			}

			if(temp.OutPutYPositionAction().length() != 0){
				myFile << "action('";
				myFile << temp.OutPutYPositionAction() << "',";

				int pos = this->CKPairs[i].shape1->GetID().find_last_of("_") + 1;
				string simpleID = this->CKPairs[i].shape1->GetID().substr(pos);
				myFile << simpleID;
				myFile << "," << temp.PosYChange;
				myFile << ")" << endl;

			}



		}

	}else{

		myFile << "Ambiguous Answer Occurs" << endl;
	}
	
}


Diagram::Diagram(){

	SCM = nullptr;
}

Diagram::Diagram(SubconsciousMemory* refSCM){

	SCM = refSCM;
}

void Diagram::SetSCM(SubconsciousMemory* refSCM){

	SCM = refSCM;
}

void Diagram::SetID(string newID){

	this->ID = newID;
	return;
}

string Diagram::GetID(){

	return this->ID;
}

void Diagram::ClearAllInterpretations(){

	this->interpretations.clear();
	return;

}

InterpretationPair Diagram::Consider(Diagram* other){


	double highestLikelyhood = 0;
	InterpretationPair highestPair;
	highestPair.ptr1 = &this->interpretations[0];
	highestPair.ptr2 = &other->interpretations[0];


	for(int i = 0; i < this->interpretations.size(); ++i){

		for(int j = 0; j < other->interpretations.size(); ++j){


			InterpretationPair currentPair;
			currentPair.ptr1 = &this->interpretations[i];
			currentPair.ptr2 = &other->interpretations[j];

			double currentLikelyhood = RankSimilarInterpretations(currentPair);

			if(currentLikelyhood > highestLikelyhood){

				highestLikelyhood = currentLikelyhood;
				highestPair = currentPair;
			}



		}

	}

	OrderInterpPair(highestPair);
	return highestPair;

}

void Diagram::ViewInterpretation(ifstream &myFile){

	string temp;
	Interpretation empty(this->ID, this->interpretations.size(), this->SCM);

	while(getline(myFile, temp)){

		stringstream line;
		line.str(temp);

		if(temp.length() == 0){
			break;

		}else if(temp.at(0) == 't'){
			type tempType = type::TRIANGLE;
			string tempID = temp.substr(0,2);
			getline(line, temp, '\'');
			getline(line, temp, ',');
			size tempSize = GetSizeOfShapeView(temp);
			getline(line, temp, ',');
			int x = stoi(temp);
			getline(line, temp, ')');
			int y = stoi(temp);

			Shape newShape(tempID, tempSize, tempType, x, y, empty.GetID());
			this->SCM->AddShapeToData(newShape);
			empty.AddShape(&this->SCM->AllShapeData.back());

		}else if(temp.at(0) == 'r'){
			type tempType = type::RECTANGLE;
			string tempID = temp.substr(0,2);
			getline(line, temp, '\'');
			getline(line, temp, ',');
			size tempSize = GetSizeOfShapeView(temp);
			getline(line, temp, ',');
			int x = stoi(temp);
			getline(line, temp, ')');
			int y = stoi(temp);

			Shape newShape(tempID, tempSize, tempType, x, y, empty.GetID());
			this->SCM->AddShapeToData(newShape);
			empty.AddShape(&this->SCM->AllShapeData.back());

		}else if(temp.at(0) == 's' && temp.at(1) != 'c'){
			type tempType = type::SQUARE;
			string tempID = temp.substr(0,2);
			getline(line, temp, '\'');
			getline(line, temp, ',');
			size tempSize = GetSizeOfShapeView(temp);
			getline(line, temp, ',');
			int x = stoi(temp);
			getline(line, temp, ')');
			int y = stoi(temp);

			Shape newShape(tempID, tempSize, tempType, x, y, empty.GetID());
			this->SCM->AddShapeToData(newShape);
			empty.AddShape(&this->SCM->AllShapeData.back());

		}else if(temp.at(0) == 's' && temp.at(1) == 'c'){
			type tempType = type::SCC;
			string tempID = temp.substr(0,4);
			getline(line, temp, '\'');
			getline(line, temp, ',');
			size tempSize = GetSizeOfShapeView(temp);
			getline(line, temp, ',');
			int x = stoi(temp);
			getline(line, temp, ')');
			int y = stoi(temp);

			Shape newShape(tempID, tempSize, tempType, x, y, empty.GetID());
			this->SCM->AddShapeToData(newShape);
			empty.AddShape(&this->SCM->AllShapeData.back());

		}else if(temp.at(0) == 'd'){
			type tempType = type::DOT;
			string tempID = temp.substr(0,2);
			getline(line, temp, '\'');
			getline(line, temp, ',');
			size tempSize = GetSizeOfShapeView(temp);
			getline(line, temp, ',');
			int x = stoi(temp);
			getline(line, temp, ')');
			int y = stoi(temp);

			Shape newShape(tempID, tempSize, tempType, x, y, empty.GetID());
			this->SCM->AddShapeToData(newShape);
			empty.AddShape(&this->SCM->AllShapeData.back());

		}else if(temp.at(0) == 'c'){
			type tempType = type::CIRCLE;
			string tempID = temp.substr(0,2);
			getline(line, temp, '\'');
			getline(line, temp, ',');
			size tempSize = GetSizeOfShapeView(temp);
			getline(line, temp, ',');
			int x = stoi(temp);
			getline(line, temp, ')');
			int y = stoi(temp);

			Shape newShape(tempID, tempSize, tempType, x, y, empty.GetID());
			this->SCM->AddShapeToData(newShape);
			empty.AddShape(&this->SCM->AllShapeData.back());

		}else if(temp.at(0) == 'i'){
			getline(line, temp, '(');
			getline(line, temp, ',');
			string ShapeID1 = empty.GetID() + "_" + temp;
			getline(line, temp, ')');
			string ShapeID2 = empty.GetID() + "_" + temp.substr(1);

			Shape* ptr1 = this->SCM->FindShapeByID(ShapeID1);
			Shape* ptr2 = this->SCM->FindShapeByID(ShapeID2);

			//CONNECT SHAPES
			ptr1->SetInside(ptr2);

		}else if(temp.at(0) == 'l'){
			getline(line, temp, '(');
			getline(line, temp, ',');
			string ShapeID1 = empty.GetID() + "_" + temp;
			getline(line, temp, ')');
			string ShapeID2 = empty.GetID() + "_" + temp.substr(1);

			Shape* ptr1 = this->SCM->FindShapeByID(ShapeID1);
			Shape* ptr2 = this->SCM->FindShapeByID(ShapeID2);

			//CONNECT SHAPES
			ptr1->SetLeftOf(ptr2);

		}else if(temp.at(0) == 'a'){
			getline(line, temp, '(');
			getline(line, temp, ',');
			string ShapeID1 = empty.GetID() + "_" + temp;
			getline(line, temp, ')');
			string ShapeID2 = empty.GetID() + "_" + temp.substr(1);

			Shape* ptr1 = this->SCM->FindShapeByID(ShapeID1);
			Shape* ptr2 = this->SCM->FindShapeByID(ShapeID2);

			//CONNECT SHAPES
			ptr1->SetAbove(ptr2);

		}else if(temp.at(0) == 'o'){
			getline(line, temp, '(');
			getline(line, temp, ',');
			string ShapeID1 = empty.GetID() + "_" + temp;
			getline(line, temp, ')');
			string ShapeID2 = empty.GetID() + "_" + temp.substr(1);

			Shape* ptr1 = this->SCM->FindShapeByID(ShapeID1);
			Shape* ptr2 = this->SCM->FindShapeByID(ShapeID2);

			//CONNECT SHAPES
			ptr1->SetOverlap(ptr2);
			ptr2->SetOverlap(ptr1);

		}

	}

	this->interpretations.push_back(empty);
	return;

}

void Diagram::AddNullInterpretation(){

	Interpretation temp(this->ID, 0, this->SCM);
	this->interpretations.push_back(temp);
	return;
}

int Diagram::NumInterpretations(){
	return this->interpretations.size();
}

Interpretation* Diagram::AccessInterpretationAt(int index){

	return &this->interpretations[index];
}

Interpretation::Interpretation(const Interpretation& other){

	this->shapes = (other.GetShapesVec());
	this->ID = other.GetID();
	this->SCM = other.GetSCM();
	return;

}

Interpretation::Interpretation(string parentID, int index, SubconsciousMemory* refSCM){

	this->ID = parentID + "_" + to_string(index + 1);
	SCM = refSCM;
	return;
}

SubconsciousMemory* Interpretation::GetSCM() const{

	return this->SCM;
}

int Interpretation::GetNumShapes() const{

	return this->shapes.size();
}

vector<Shape*> Interpretation::GetShapesVec() const{

	return this->shapes;
}

void Interpretation::SetShapesVec(vector<Shape*> &other){

	this->shapes.clear();
	for(int i = 0; i < other.size(); ++i){

		this->shapes.push_back(other[i]);
	}
	return;
}

void Interpretation::AddShape(Shape* shape){

	this->shapes.push_back(shape);
	return;
}

string Interpretation::GetID() const{

	return this->ID;
}

Shape* Interpretation::AccessShapeAt(int index){

	return this->shapes[index];
}

void Interpretation::AlphabetizeShapes(){

	struct less_than_key{

		bool operator() (Shape* ptr1, Shape* ptr2){

			int one, two;
			string temp1, temp2;

			one = ptr1->GetID().find_last_of("_")+1;
			two = ptr2->GetID().find_last_of("_")+1;

			temp1 = ptr1->GetID().substr(one);
			temp2 = ptr2->GetID().substr(two);

			int comp = temp1.compare(temp2);
        	return (comp > 0);
    	}

	};

	sort(this->shapes.begin(), this->shapes.end(), less_than_key());
	reverse(this->shapes.begin(), this->shapes.end());
	return;
}

Shape::Shape(string ID, size Girth, type Kind, int X, int Y, string parentID){

	this->ShapeID = parentID + "_" + ID;
	this->SizeOf = Girth;
	this->TypeOf = Kind;
	this->Center[0] = X;
	this->Center[1] = Y;
	this->LeftOf = nullptr;
	this->Above = nullptr;
	this->InsideOf = nullptr;
	this->Overlap = nullptr;
}

string Shape::GetShapeID() const{

	return this->ID;
}

string Shape::GetID() const{


	return this->ShapeID;
}

size Shape::GetSizeOf() const{

	return this->SizeOf;
}

type Shape::GetTypeOf() const{

	return this->TypeOf;
}

int Shape::GetCenterY() const{

	return Center[1];
}

int Shape::GetCenterX() const{

	return Center[0];
}

Shape* Shape::ShapeToRight() const{

	return this->LeftOf;
}

Shape* Shape::ShapeSurrounding() const{

	return this->InsideOf;
}

Shape* Shape::ShapeBelow() const{

	return this->Above;
}

Shape* Shape::ShapeOverlapping() const{

	return this->Overlap;
}

void Shape::SetLeftOf(Shape* other){

	this->LeftOf = other;
	return;
}
void Shape::SetAbove(Shape* other){

	this->Above = other;
	return;
}
void Shape::SetInside(Shape* other){

	this->InsideOf = other;
}
void Shape::SetOverlap(Shape* other){

	this->Overlap = other;
}

void Shape::SetSizeOf(size setter){
	this->SizeOf = setter;
}
void Shape::SetCenter(int setter[2]){
	this->Center[0] = setter[0];
	this->Center[1] = setter[1];
	return;
}

Transform::Transform(Shape* from, Shape* to){

	this->SizeFrom = from->GetSizeOf();
	this->SizeTo = to->GetSizeOf();
	this->PosXChange = to->GetCenterX() - from->GetCenterX();
	this->PosYChange = to->GetCenterY() - from->GetCenterY();
	return;
}
void Transform::Apply(Shape* orig){


	orig->SetSizeOf(this->SizeTo);
	int temp[2] = {this->PosXChange + orig->GetCenterX(), this->PosYChange + orig->GetCenterY()};
	orig->SetCenter(temp);
	return;

}

string Transform::OutPutSizeAction(){

	if(this->SizeFrom > this->SizeTo){

		return "shrink";
	}else if(this->SizeFrom < this->SizeTo){

		return "enlarge";
	}else{

		return "";
	}

}

string Transform::OutPutXPositionAction(){

	if(this->PosXChange > 0){

		return "moveRight";
	}else if(this->PosXChange < 0){

		return "moveLeft";
	}else{

		return "";
	}

}

string Transform::OutPutYPositionAction(){

	if(this->PosYChange > 0){

		return "moveDown";
	}else if(this->PosYChange < 0){

		return "moveUp";
	}else{

		return "";
	}
}







