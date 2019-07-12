//
//  diagram_processing.h
//

#ifndef diagram_processing_h
#define diagram_processing_h

#include "diagram_shape.h"
#include <set>

class Link;

class Link{
public:
	cv::Point coordinates;
	bool visitation;
	vector<Link*> links;
	Link *backTrack = nullptr;
	int GetLinksSize();
	int at();
	void IncrementLinksSize();
	void setAt(int a);
private:
	int linkSize=0;
	int indexInList=0;

};

class PointMap{
public:
	vector<cv::Point> pointList;
	vector<Link> dataStorage;
	Link *head;
	void resetFlags();
	void changeHead(int index);
};

struct primitivesStruct{
	//Set up array of primitives, as well as names of primitives
	vector<	vector<int> > lines;
	vector< vector<int> > circles;
	vector< vector<int> > dots;
	vector<string> lineID;
	vector<string> circleID;
	vector<string> dotID;
	vector< vector<int> > intersectionPoints;
	PointMap structuredMap;
	vector<string> shapeHash;
	set<string> baseShapes;
	//////////////////////////////
};


//This function modifies original and returns another shape as well, with correct info and all.
DiagramShape splitIntoTwoShapes(DiagramShape &original, const primitivesStruct &dataSet);

void pairShapes(primitivesStruct &dataSet, vector< DiagramShape>  &result, int hashIndex);

bool isTwoShapes(DiagramShape &original, const primitivesStruct &dataSet);

void giveIDsToShapes(vector<DiagramShape> &results);

void findSimplePolygons(const primitivesStruct &dataSet, vector<DiagramShape> &results);

void getRidOfDuplicateShapes(vector<string> &data);

void heuristics(PointMap &data);

void findAllShapesFromOnePoint(primitivesStruct &dataSet, int pointIndex);

void interpretCircles(const primitivesStruct &dataSet, vector<DiagramShape> &results);

void interpretDots(const primitivesStruct &dataSet, vector<DiagramShape> &results);

bool isSquare(DiagramShape &inputShape, const primitivesStruct &dataSet);

bool isRect(DiagramShape &inputShape, const primitivesStruct &dataSet);

void findSimpleCurvesAndDots(const primitivesStruct &dataSet);

void setShapeTypeForPoly(DiagramShape &polygon, const primitivesStruct &dataSet);

void setCenter(DiagramShape &polygon);

void setCentersForAll(vector<DiagramShape> &results);

void createMap(primitivesStruct &dataSet);



// Main method called to solve this problem. The results should be stored in "results", where each inner vector is a found solution for the input diagram (given by the vector of primitives).
// ***IMPLEMENT***
void findShapes(vector<DiagramShape> &primitives, vector<vector<DiagramShape> > &results);

void findRelationsInSingleDiagram(vector<DiagramShape> &diagram);

//add relationships to shapes in each diagram
// ***IMPLEMENT***
void findRelations(vector<vector<DiagramShape> > &diagrams);


// Determines whether two shapes overlap. Returns true if shapes overlap, false otherwise.
bool shapeOverlap(const DiagramShape& shape1, const DiagramShape& shape2);

// Determines whether a shape is within another shape.
// Returns 0 if neither are within the other, 1 if shape1 is within shape2, 2 if shape2 is within shape1.
int shapeInside(const DiagramShape& shape1, const DiagramShape& shape2);

#endif /* diagram_processing_h */
