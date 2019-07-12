#ifndef ElectronicMind_h
#define ElectronicMind_h

#include <vector>
#include <string>
#include <iostream>

using namespace std;

enum size{
	SMALL,
	MEDIUM,
	LARGE
};

enum type{
	TRIANGLE,
	RECTANGLE,
	SQUARE,
	CIRCLE,
	DOT,
	SCC
};

class Shape{

	private:
		string ShapeID;
		size SizeOf;
		type TypeOf;
		int Center[2];
		Shape* InsideOf;
		Shape* LeftOf;
		Shape* Above;
		Shape* Overlap;
		string ID;

	public:
		Shape(string ID, size Girth, type Kind, int X, int Y, string parentID);
		Shape(const Shape& b){

			this->SizeOf = b.GetSizeOf();
			this->TypeOf = b.GetTypeOf();
			this->Center[0] = b.GetCenterX();
			this->Center[1] = b.GetCenterY();
			this->ShapeID = b.GetShapeID();
			this->InsideOf = b.ShapeSurrounding();
			this->LeftOf = b.ShapeToRight();
			this->Above = b.ShapeBelow();
			this->Overlap = b.ShapeOverlapping();
			this->ID = b.GetID();
		}
		string GetID() const;
		string GetShapeID() const;
		size GetSizeOf() const;
		type GetTypeOf() const;
		int GetCenterX() const;
		int GetCenterY() const;
		Shape* ShapeToRight() const;
		Shape* ShapeSurrounding() const;
		Shape* ShapeBelow() const;
		Shape* ShapeOverlapping() const;
		void SetLeftOf(Shape* other);
		void SetAbove(Shape* other);
		void SetInside(Shape* other);
		void SetOverlap(Shape* other);
		void SetSizeOf(size setter);
		void SetCenter(int setter[2]);
		Shape operator=(const Shape& b){

			Shape result("", b.GetSizeOf(), b.GetTypeOf(), b.GetCenterX(), b.GetCenterY(), b.GetID());
			return result;
		}


};

class SubconsciousMemory{

	public:
		SubconsciousMemory();

		vector<Shape> AllShapeData;

		void AddShapeToData(Shape other);
		Shape* FindShapeByID(string SID);
		int GetDataSize();

};

class Transform{

	private:

		size SizeFrom;
		size SizeTo;


	public:
		int PosXChange;
		int PosYChange;

		Transform(Shape* from, Shape* to);
		void Apply(Shape* orig);
		string OutPutSizeAction();
		string OutPutXPositionAction();
		string OutPutYPositionAction();



};


class Interpretation{

	private:
		vector<Shape*> shapes;
		string ID;
		SubconsciousMemory* SCM;

	public:
		Interpretation(const Interpretation& other);
		Interpretation(string parentID, int index, SubconsciousMemory* refSCM);
		vector<Shape*> GetShapesVec() const;
		void SetShapesVec(vector<Shape*> &other);
		void AddShape(Shape* shape);
		Shape* AccessShapeAt(int index);
		int GetNumShapes() const;
		string GetID() const ;
		SubconsciousMemory* GetSCM() const;
		void AlphabetizeShapes();

};

struct InterpretationPair{

	Interpretation* ptr1;
	Interpretation* ptr2;
	
};

class Diagram{

	private:
		vector<Interpretation> interpretations;
		string ID;
		SubconsciousMemory* SCM;

		void PushNewInterpretation(Interpretation &other);
		void ClearAllInterpretations();

	public:
		Diagram(SubconsciousMemory* refSCM);
		Diagram();
		InterpretationPair Consider(Diagram* other);
		void ViewInterpretation(ifstream &myFile);
		void AddNullInterpretation();
		Interpretation* AccessInterpretationAt(int index);
		void SetSCM(SubconsciousMemory* refSCM);
		void SetID(string newID);
		string GetID();
		int NumInterpretations();
		void DeleteIncorrectInterpretations(Interpretation* correct);


};

struct ShapePair{

	Shape* shape1;
	Shape* shape2;

};

class Brain{

	private:
		Diagram A, B, C, K1, K2, K3;
		vector<ShapePair> ABPairs, ACPairs, CKPairs;
		SubconsciousMemory* SCM;
		Diagram PredictedAnswer;
		Diagram* bestAnswer;
		bool ShapeExistInA_AsPairB(Shape* other);
		bool ShapeExistInC_AsPairK(Shape* other);
		void AlphabetizeShapePairs();
		void CreateNewDiagram();
		void TransformNewDiagram(vector <Transform> &transforms);
		void FindSimilarShapesAB();
		void FindSimilarShapesCK();
		void FindAnalagousShapesAC();
		void PredictAnswer();
		void FindBestAnswer();
		vector <Transform> TheorizeTheChanges();
		void NarrowADown();
		double RankAnswer(InterpretationPair answerPair);
		void OrderTheShapePairs();

	public:
		Brain(SubconsciousMemory* refSCM);
		void View(string inputFile);
		void Think();
		Diagram* Answer();
		void Communicate(string outputFile);


};


#endif
