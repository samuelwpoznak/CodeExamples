//
//  diagram_processing.cpp
//

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include <set>
#include <string>

#include "diagram_processing.h"

//Comparators for Points

static bool yValComp(cv::Point a, cv::Point b){
    return(a.y < b.y);
}

static bool xValComp(cv::Point a, cv::Point b){
    return(a.x < b.x);
}

int Link::GetLinksSize(){

    return linkSize;
}

void Link::IncrementLinksSize(){

    ++linkSize;
}

int Link::at(){

    return this->indexInList;
}

void Link::setAt(int a){

    this->indexInList = a;
}

void PointMap::resetFlags(){

    for(int i = 0; i < this->dataStorage.size(); ++i){

        this->dataStorage[i].visitation = false;
        this->dataStorage[i].backTrack = nullptr;

    }
}

void PointMap::changeHead(int index){

    this->head = &this->dataStorage[index];

}


void setShapeTypeForPoly(DiagramShape &polygon, const primitivesStruct &dataSet){



        if(polygon.ids.size() == 3){

            polygon.type = ShapeType::TRIANGLE;

        }else if (polygon.ids.size() == 4){

            
            if(isSquare(polygon, dataSet)) polygon.type = ShapeType::SQUARE;
            else if(isRect(polygon, dataSet)) polygon.type = ShapeType::RECTANGLE;
            else polygon.type = ShapeType::SCC;
            
        }else{

            polygon.type = ShapeType::SCC;
        }

}

void pairShapes(primitivesStruct &dataSet, vector< DiagramShape> &result, int hashIndex){

    string linesUsed = "/" + dataSet.shapeHash[hashIndex] + "-" + dataSet.shapeHash[hashIndex].front() + "/";

    int tester = 0;

    //push the first shape into the result;
    DiagramShape firstShape;
    for(int i = 0; i < dataSet.shapeHash[hashIndex].length()-2; ++i){

        if(dataSet.shapeHash[hashIndex].at(i) == '-') continue;

        int pointID1, pointID2;

        pointID1 = (dataSet.shapeHash[hashIndex].at(i) - '0');
        pointID2 = (dataSet.shapeHash[hashIndex].at(i+2) - '0');

        cv::Point p1, p2;

        p1 = dataSet.structuredMap.pointList[pointID1];
        p2 = dataSet.structuredMap.pointList[pointID2];

        int a[] = {p1.x, p1.y, p2.x, p2.y};
        int b[] = {p2.x, p2.y, p1.x, p1.y};

        vector<int> v(a, a + sizeof(a) / sizeof(a[0]) );
        vector<int> rv(b, b + sizeof(b) / sizeof(b[0]) );

        v.resize(4);
        rv.resize(4);
                
        auto it = find(dataSet.lines.begin(), dataSet.lines.end(), v);
        if(it == dataSet.lines.end()) it = find(dataSet.lines.begin(), dataSet.lines.end(), rv);

        int index = it - dataSet.lines.begin();
        //cout << index << endl;
        firstShape.points.push_back(p2);
        firstShape.ids.push_back("s" + to_string(index + 1));
        //cout << firstShape.ids.back() << "<- line index" << endl;
    }


    //add the full circle connection
    

    int pointID1, pointID2;

    pointID1 = dataSet.shapeHash[hashIndex].front() - '0';
    pointID2 = dataSet.shapeHash[hashIndex].back() - '0';

    cv::Point p1, p2;          

    p1 = dataSet.structuredMap.pointList[pointID1];
    p2 = dataSet.structuredMap.pointList[pointID2];

    int a[] = {p1.x, p1.y, p2.x, p2.y};
    int b[] = {p2.x, p2.y, p1.x, p1.y};

    vector<int> v(a, a + sizeof(a) / sizeof(a[0]) );
    vector<int> rv(b, b + sizeof(b) / sizeof(b[0]) );

    v.resize(4);
    rv.resize(4);

    auto it = find(dataSet.lines.begin(), dataSet.lines.end(), v);
    if(it == dataSet.lines.end()) it = find(dataSet.lines.begin(), dataSet.lines.end(), rv);

    int index = it - dataSet.lines.begin();
    //cout << "line index" << index << endl;
    firstShape.ids.push_back("s" + to_string(index + 1));
    firstShape.points.push_back(p1);
    //cout << "linesUsed: " << linesUsed << endl;

    result.push_back(firstShape);

    ///////////////////////////////




    while(true){

        //cout << "first loop: " << endl;
        //cout << dataSet.shapeHash.size() << endl;
        int endflag=false;

        for(int i = 0; i < dataSet.shapeHash.size(); ++i){

            bool shapeIsValid=true;

            for(int j = 0; j < dataSet.shapeHash[i].length()-2; ++j){

                if(dataSet.shapeHash[i].at(j) == '-') continue;

                string lineConnect = dataSet.shapeHash[i].substr(j,3);
                string reverseConnect = lineConnect;
                //cout << lineConnect << endl;
                reverse(reverseConnect.begin(), reverseConnect.end());

                size_t iter = linesUsed.find(lineConnect);
                if(iter == string::npos) iter = linesUsed.find(reverseConnect);

                if(iter != string::npos){

                    //cout << "invalid" << endl;
                    shapeIsValid=false;
                    break;

                }else{
                    //cout << "shape hash for valid: " << dataSet.shapeHash[i] << endl;
                    string lineConnect = dataSet.shapeHash[i].substr(dataSet.shapeHash[i].length()-1, 1);
                    lineConnect += "-";
                    lineConnect += dataSet.shapeHash[i].substr(0,1);
                    string reverseConnect = lineConnect;
                    //cout << lineConnect << endl;
                    reverse(reverseConnect.begin(), reverseConnect.end());

                    size_t iter = linesUsed.find(lineConnect);
                    if(iter == string::npos) iter = linesUsed.find(reverseConnect);


                    if(iter != string::npos){ 

                        shapeIsValid = false;
                        break;
                    }else{

                        shapeIsValid = true;
                    }

                    
                }

            }




            if(shapeIsValid){

                //cout << "valid" << endl;
                DiagramShape temp;
                temp.ids.clear();
                //cout << dataSet.shapeHash[i] << endl;
                linesUsed += dataSet.shapeHash[i] + "-" + dataSet.shapeHash[i].front() + "/";

                for(int k = 0; k < dataSet.shapeHash[i].length()-2; ++k){

                    if(dataSet.shapeHash[i].at(k) == '-') continue;

                    int pointID1, pointID2;

                    pointID1 = (dataSet.shapeHash[i].at(k) - '0');
                    pointID2 = (dataSet.shapeHash[i].at(k+2) - '0');

                    cv::Point p1, p2;

                    p1 = dataSet.structuredMap.pointList[pointID1];
                    p2 = dataSet.structuredMap.pointList[pointID2];

                    int a[] = {p1.x, p1.y, p2.x, p2.y};
                    int b[] = {p2.x, p2.y, p1.x, p1.y};

                    vector<int> v(a, a + sizeof(a) / sizeof(a[0]) );
                    vector<int> rv(b, b + sizeof(b) / sizeof(b[0]) );

                    v.resize(4);
                    rv.resize(4);

                    //cout << v[0] << v[1] << v[2] << v[3] << endl;
                
                    auto it = find(dataSet.lines.begin(), dataSet.lines.end(), v);
                    if(it == dataSet.lines.end()) it = find(dataSet.lines.begin(), dataSet.lines.end(), rv);

                    int index = it - dataSet.lines.begin();
                    //cout << "line index" << index << endl;
                    temp.points.push_back(p2);
                    temp.ids.push_back("s" + to_string(index + 1));

                }

                //add the full circle connection
                int pointID1, pointID2;

                pointID1 = dataSet.shapeHash[i].front() - '0';
                pointID2 = dataSet.shapeHash[i].back() - '0';

                cv::Point p1, p2;          

                p1 = dataSet.structuredMap.pointList[pointID1];
                p2 = dataSet.structuredMap.pointList[pointID2];

                int a[] = {p1.x, p1.y, p2.x, p2.y};
                int b[] = {p2.x, p2.y, p1.x, p1.y};

                vector<int> v(a, a + sizeof(a) / sizeof(a[0]) );
                vector<int> rv(b, b + sizeof(b) / sizeof(b[0]) );

                v.resize(4);
                rv.resize(4);

                auto it = find(dataSet.lines.begin(), dataSet.lines.end(), v);
                if(it == dataSet.lines.end()) it = find(dataSet.lines.begin(), dataSet.lines.end(), rv);

                int index = it - dataSet.lines.begin();
                //cout << "line index" << index << endl;
                temp.points.push_back(p1);
                temp.ids.push_back("s" + to_string(index + 1));

                //cout << "linesUsed: " << linesUsed << endl;



                result.push_back(temp);

            }


            //now we should check to see if we are done. 
            int lineCount = count(linesUsed.begin(), linesUsed.end(), '-');

            if(lineCount == dataSet.lines.size()){

                endflag = true;
                break;

            } 

        }


        if(endflag) break;


        ++tester;
    }

    for(int i = 0; i < result.size(); ++i){


        setShapeTypeForPoly(result[i], dataSet);
    }



}

DiagramShape splitIntoTwoShapes(DiagramShape &original, const primitivesStruct &dataSet){

    cv::Point tempBasePt, tempEndPt;
    DiagramShape result;


    //First we need to find the shared point. 
    for(int i = 1; i < original.ids.size(); ++i){
        tempBasePt.x = dataSet.lines[atoi(&original.ids[i].at(1))][0];
        tempBasePt.y = dataSet.lines[atoi(&original.ids[i].at(1))][1];

        for(int j = i; j < original.ids.size(); ++j){
            tempEndPt.x = dataSet.lines[atoi(&original.ids[i].at(1))][2];
            tempEndPt.y = dataSet.lines[atoi(&original.ids[i].at(1))][3];

            if(tempBasePt == tempEndPt){
                //this means that the shape has closed at this point, but continued on.
                //we need to copy these lines into the new shape, and delete from the old.
                result.ids.resize(j-i);
                copy(original.ids.begin() + i, original.ids.begin() + j, back_inserter(result.ids));
                original.ids.erase(original.ids.begin() + i, original.ids.begin() + j);
                setShapeTypeForPoly(result, dataSet);
                return result;

            }
        }

    }
}

bool isTwoShapes(DiagramShape &original, const primitivesStruct &dataSet){

    vector<cv::Point> corners;
    if(original.ids.size() > 5){

        for(int i = 0; i < original.ids.size(); ++i){
            corners.push_back(cv::Point(dataSet.lines[i][0], dataSet.lines[i][1]));
            corners.push_back(cv::Point(dataSet.lines[i][2], dataSet.lines[i][3]));
        }

        for(int j = 0; j < corners.size(); ++j){

            if(count(corners.begin(), corners.end(), corners[j]) > 2){

                return true;
            }


        }
    }

    return false;
}

void giveIDsToShapes(vector<DiagramShape> &results){

    int dotCount = 1, triCount = 1, rectCount = 1, sqCount = 1, circCount = 1, sccCount = 1;

    for(int i = 0; i < results.size(); ++i){

        switch(results[i].type){

            case ShapeType::TRIANGLE:
                results[i].id = "t" + to_string(triCount);
                triCount++;
                break;
            case ShapeType::SQUARE:
                results[i].id = "sq" + to_string(sqCount);
                sqCount++;
                break;
            case ShapeType::SCC:
                results[i].id = "scc" + to_string(sccCount);
                sccCount++;
                break;           
            case ShapeType::DOT:
                results[i].id = "d" + to_string(dotCount);
                dotCount++;
                break;
            case ShapeType::CIRCLE:
                results[i].id = "c" + to_string(circCount);
                circCount++;
                break;
            case ShapeType::RECTANGLE:
                results[i].id = "r" + to_string(rectCount);
                rectCount++;
                break;

        }

    }


}

void createMap(primitivesStruct &dataSet){

    dataSet.structuredMap.pointList.push_back(cv::Point(dataSet.lines[0][0], dataSet.lines[0][1]));
    dataSet.structuredMap.pointList.push_back(cv::Point(dataSet.lines[0][2], dataSet.lines[0][3]));

    //cout<<"first loop incoming" << endl;

    //iterate through line list to get all points
    for(int i = 1; i < dataSet.lines.size(); ++i){

        //-1 = 01,
        //1 = 23,
        bool firstInList= 0, secondInList=0;

        for(int j = 0; j < dataSet.structuredMap.pointList.size(); ++j){

            if(dataSet.structuredMap.pointList[j] == cv::Point(dataSet.lines[i][0], dataSet.lines[i][1])){

                firstInList=true;
            }

            if(dataSet.structuredMap.pointList[j] == cv::Point(dataSet.lines[i][2], dataSet.lines[i][3])){

                secondInList=true;
            }

        }

        if(!firstInList) dataSet.structuredMap.pointList.push_back(cv::Point(dataSet.lines[i][0], dataSet.lines[i][1]));
        if(!secondInList) dataSet.structuredMap.pointList.push_back(cv::Point(dataSet.lines[i][2], dataSet.lines[i][3]));

    }
    //we now have an unordered list of all points. 

    //now we need to create the dataSpace for the links
    dataSet.structuredMap.dataStorage.resize(dataSet.structuredMap.pointList.size());


    //cout << "second loop incoming" << endl;
    //now iterate through the point list to create the beginning data for the linked list
    for(int i = 0; i < dataSet.structuredMap.pointList.size(); ++i){

        dataSet.structuredMap.dataStorage[i].coordinates = dataSet.structuredMap.pointList[i];
        dataSet.structuredMap.dataStorage[i].visitation = false;
        dataSet.structuredMap.dataStorage[i].links.resize(dataSet.structuredMap.pointList.size()-1);
        dataSet.structuredMap.dataStorage[i].setAt(i);
    }


    //cout << "third" << endl;

    //now, connect the links on the datastructure.
    for(int i = 0; i < dataSet.lines.size(); ++i){

        for(int j = 0; j < dataSet.structuredMap.pointList.size(); ++j){

            if(dataSet.structuredMap.pointList[j] == cv::Point(dataSet.lines[i][0], dataSet.lines[i][1])){

                auto it = find(dataSet.structuredMap.pointList.begin(), dataSet.structuredMap.pointList.end(), cv::Point(dataSet.lines[i][2], dataSet.lines[i][3]));
                int index = it - dataSet.structuredMap.pointList.begin();

                dataSet.structuredMap.dataStorage[j].IncrementLinksSize();
                dataSet.structuredMap.dataStorage[j].links[dataSet.structuredMap.dataStorage[j].GetLinksSize() - 1] = &dataSet.structuredMap.dataStorage[index];

                dataSet.structuredMap.dataStorage[index].IncrementLinksSize();
                dataSet.structuredMap.dataStorage[index].links[dataSet.structuredMap.dataStorage[index].GetLinksSize() - 1] = &dataSet.structuredMap.dataStorage[j];
                break;
            }

            if(dataSet.structuredMap.pointList[j] == cv::Point(dataSet.lines[i][2], dataSet.lines[i][3])){

                auto it = find(dataSet.structuredMap.pointList.begin(), dataSet.structuredMap.pointList.end(), cv::Point(dataSet.lines[i][0], dataSet.lines[i][1]));
                int index = it - dataSet.structuredMap.pointList.begin();

                dataSet.structuredMap.dataStorage[j].IncrementLinksSize();
                dataSet.structuredMap.dataStorage[j].links[dataSet.structuredMap.dataStorage[j].GetLinksSize() - 1] = &dataSet.structuredMap.dataStorage[index];

                dataSet.structuredMap.dataStorage[index].IncrementLinksSize();
                dataSet.structuredMap.dataStorage[index].links[dataSet.structuredMap.dataStorage[index].GetLinksSize() - 1] = &dataSet.structuredMap.dataStorage[j];
                break;
            }

        }
    }

    //cout << "four" << endl;
    //now iterate through and resize the linksSize
    for(int i = 0; i < dataSet.structuredMap.dataStorage.size(); ++i){

        dataSet.structuredMap.dataStorage[i].links.resize(dataSet.structuredMap.dataStorage[i].GetLinksSize());

    }

    //set the list head
    dataSet.structuredMap.head = &dataSet.structuredMap.dataStorage[0];

    //cout << dataSet.structuredMap.pointList.size() << endl;
    //cout << dataSet.structuredMap.dataStorage.size() << endl;
    //cout << dataSet.structuredMap.dataStorage[0].GetLinksSize() << endl;
    //cout << dataSet.structuredMap.pointList << endl;

}



//this function takes the lines of the primitive list and creates the list of simple polygons
void findSimplePolygons(const primitivesStruct &dataSet, vector<DiagramShape> &results){

    vector<int> baseJoint(2);
    vector<int> endJoint(2);
    vector<int> tempJoint(2);
    cv::Point tempPoint;
    deque< vector<int> > proxyData(dataSet.lines.begin(), dataSet.lines.end());
    deque<string> proxyNames(dataSet.lineID.begin(), dataSet.lineID.end());
    DiagramShape tempShape;
    vector<DiagramShape> splitShapes(0);
    //cout << dataSet.lines[3][0] << dataSet.lines[3][1] << dataSet.lines[3][2] << dataSet.lines[3][3] << endl;



    while(proxyData.size() != 0){
        //cout << proxyData.size() << endl;
        baseJoint[0] = proxyData[0][0];
        baseJoint[1] = proxyData[0][1];
        endJoint[0] = proxyData[0][2];
        endJoint[1] = proxyData[0][3];
        proxyData.pop_front();
        tempShape.ids.push_back(proxyNames[0]);
        //tempPoint.x = baseJoint[0];
        //tempPoint.x = baseJoint[1];
        //tempShape.points.push_back(tempPoint);
        tempPoint.x = endJoint[0];
        tempPoint.y = endJoint[1];
        tempShape.points.push_back(tempPoint);
        proxyNames.pop_front();
        tempJoint = endJoint;

        while(tempJoint != baseJoint){
            int index = 0;
            //cout << "temp does not equal end" << endl;
            while(true){
                //cout << "check next joint" << endl;
                //cout << proxyNames[index] << endl;
                //cout << proxyData[index][0] << " " << proxyData[index][1] << endl;
                if(tempJoint[0] == proxyData[index][0] && tempJoint[1] == proxyData[index][1]){

                        tempShape.ids.push_back(proxyNames[index]);
                        proxyNames.erase(proxyNames.begin()+index);
                        tempJoint[0] = proxyData[index][2];
                        tempJoint[1] = proxyData[index][3];
                        tempPoint.x = tempJoint[0];
                        tempPoint.y = tempJoint[1];
                        tempShape.points.push_back(tempPoint);
                        proxyData.erase(proxyData.begin()+index);
                        //cout << "found close point" << endl;
                        break;


                }else if(tempJoint[0] == proxyData[index][2] && tempJoint[1] == proxyData[index][3]){

                        tempShape.ids.push_back(proxyNames[index]);
                        proxyNames.erase(proxyNames.begin()+index);
                        tempJoint[0] = proxyData[index][0];
                        tempJoint[1] = proxyData[index][1];
                        tempPoint.x = tempJoint[0];
                        tempPoint.y = tempJoint[1];
                        tempShape.points.push_back(tempPoint);
                        proxyData.erase(proxyData.begin()+index);
                        //cout << "found close point" << endl;
                        break;


                }
                
                ++index;
            }
        }

        //edge case handler.
        //this edge case occurs when 2 seperate shapes are processed as one due to a shared corner
        //only occurs with shapes containing more than 5 sides.

        //this edge handling is fairly in depth and broad. I'm sure it's doing more than i neccessarily need to do.
        
        while(isTwoShapes(tempShape, dataSet)){

            splitShapes.push_back(splitIntoTwoShapes(tempShape, dataSet));

        }

        for(int i = 0; i < splitShapes.size(); ++i){

            while(isTwoShapes(splitShapes[i], dataSet)){

                splitShapes.push_back(splitIntoTwoShapes(tempShape, dataSet));
            }
            results.push_back(splitShapes[i]);
        }
        

        setShapeTypeForPoly(tempShape, dataSet);
        //cout << "points" << tempShape.points.size() << endl;
        results.push_back(tempShape);
        //clear tempShape and reset it
        tempShape.ids.clear();
        tempShape.points.clear();

    }
}

//this function makes changes to the pointmap so that 
//no duplicate shapes are found
//and the algorithm prunes incorrect solutions.
void heuristics(PointMap &data){

    //first heuristic is to force 


}

void getRidOfDuplicateShapes(vector<string> &data){

    for(int i = 0; i < data.size(); ++i){

        sort(data[i].begin(), data[i].end());
    }

    set<string> s(data.begin(), data.end());
    data.clear();
    data.assign(s.begin(), s.end());
}


//this function will create a shape beginning at primaryPoint that is garunteed to be unique.
//if a dead end is found, it will apend the path it took += "DEAD" to the shapeHash so it is not repeated.

//this function returns a pointer to the end point if a shape has been created.
//If nothing has been created, it returns a nullptr.

Link* recursivelyDeepen(Link *linkPtr, Link *primaryPoint, primitivesStruct &dataSet, string searchID){

    linkPtr->visitation = true;
    Link *result=nullptr;
    bool previouslyFound = false;

    //iterate through all of this points connetions.
    for(int i = 0; i < linkPtr->GetLinksSize(); ++i){


        //cout << searchID << endl;
        //cout << "connection: " << linkPtr->at() << ", " << linkPtr->links[i]->at() << endl;
        //if a connection has not been visited
        if(!linkPtr->links[i]->visitation){

            //if this is a previously discovered dead end... continue.
            auto it = find(dataSet.shapeHash.begin(), dataSet.shapeHash.end(), searchID + to_string(linkPtr->links[i]->at()) + "-DEAD");
            if(it != dataSet.shapeHash.end() && dataSet.shapeHash.size() != 0){

                //cout << "prev disc dead end" << endl;
                linkPtr->links[i]->visitation = true;
                continue;
            } 

            //if not...
            // update the searchHash. and backtrack pointer
            linkPtr->links[i]->backTrack = linkPtr;

            result = recursivelyDeepen(linkPtr->links[i], primaryPoint, dataSet, searchID + to_string(linkPtr->links[i]->at()) + "-");
            if(result == nullptr) linkPtr->links[i]->visitation = false;
            
        }else{

            //if this visited connection is the "head"
            if(linkPtr->links[i]->at() == primaryPoint->at() && count(searchID.begin(), searchID.end(), '-') > 2){
                //cout << "found head "<< endl;
                searchID.pop_back();
                

                //find if we have discovered this solution in the past.
                auto it2 = find(dataSet.shapeHash.begin(), dataSet.shapeHash.end(), searchID);
                string baseShapeID = searchID;
                sort(baseShapeID.begin(), baseShapeID.end());
                auto it3 = find(dataSet.baseShapes.begin(), dataSet.baseShapes.end(), baseShapeID);


                //if we have not, set result. But also... add to the "Dead List"
                if(dataSet.shapeHash.size() == 0){

                    primaryPoint->backTrack = linkPtr;
                    result = linkPtr;
                    //dataSet.shapeHash.push_back(searchID + "-DEAD");

                }else if(it2 != dataSet.shapeHash.end() || it3 != dataSet.baseShapes.end()){
                    //cout << "previously found solution" << searchID <<  endl;
                    searchID += "-";
                    continue;
                    
                    //result = linkPtr;
                    //dataSet.shapeHash.push_back(searchID + "-DEAD");
                }else if(it2 == dataSet.shapeHash.end()){
                    
                    //cout << "new solution " << searchID << endl;
                    result = linkPtr;
                    primaryPoint->backTrack = linkPtr;
                }
                //if we have discovered the solution, keep iterating.

            }

        }

        if(result != nullptr){
            //cout << "break" << endl;
            //cout << result->at() << endl;
            //cout << searchID << endl;
            break;
        }


    }

    //if all connections have been visited, this path is dead. Return a nullptr.
    //update the searchHash with this new deadEnd.

    /*if(result == nullptr){


        searchID += "DEAD";
        dataSet.shapeHash.push_back(searchID);
    }*/


    return result;
}

Link* recursivelyDeepenHelper(Link *primaryPoint, primitivesStruct &dataSet){

    //cout << "primary point, helper" << primaryPoint->at() << endl;
    return recursivelyDeepen(primaryPoint, primaryPoint, dataSet, to_string(primaryPoint->at()) + "-");



}


void findAllShapesFromOnePoint(primitivesStruct &dataSet, int pointIndex){
    //this function will use an iterative deepening method to find every unique shape that
    //stems from a single line
    //all of these shapes have unique ids inside of shapeHash.

    dataSet.structuredMap.changeHead(pointIndex);

    //since it stems from a single line, we will not iterate through every 
    //point that the head is connected to. Instead, we will opt for the first one.

    while(true){

        dataSet.structuredMap.resetFlags();
        dataSet.structuredMap.head->visitation = true;

        heuristics(dataSet.structuredMap);

        Link *pointer = recursivelyDeepenHelper(dataSet.structuredMap.head, dataSet);


        bool finished = false;

        //if a shape has been created
        if(pointer){
            
            //cout << "end link: " << pointer->at() << endl;
            //cout << "2nd end link: " << pointer->backTrack->at() << endl;
            Link *traverser = pointer;
            string newID;

            while(traverser != dataSet.structuredMap.head){

                newID += "-" + to_string(traverser->at());
                traverser = traverser->backTrack;

            }

            newID += "-" + to_string(pointIndex);
            //cout << "new shape reverse?" << newID << endl;
            reverse(newID.begin(), newID.end());
            newID.pop_back();
            //cout << "new shape: " << newID << endl;

            dataSet.shapeHash.push_back(newID);
            sort(newID.begin(), newID.end());
            dataSet.baseShapes.insert(newID);
        }else{
            break;
        }



    }

 

    return;

}



void interpretCircles(const primitivesStruct &dataSet, vector<DiagramShape> &results){

    

    for(int i = 0; i < dataSet.circles.size(); ++i){
        DiagramShape tempShape;
        tempShape.type = ShapeType::CIRCLE;
        tempShape.ids.push_back(dataSet.circleID[i]);
        tempShape.center.x = dataSet.circles[i][0];
        tempShape.center.y = dataSet.circles[i][1];
        tempShape.radius = dataSet.circles[i][2];

        results.push_back(tempShape);

    }

}

void interpretDots(const primitivesStruct &dataSet, vector<DiagramShape> &results){


    

    for(int i = 0; i < dataSet.dots.size(); ++i){
        DiagramShape tempShape;
        tempShape.type = ShapeType::DOT;
        tempShape.ids.push_back(dataSet.dotID[i]);
        tempShape.center.x = dataSet.dots[i][0];
        tempShape.center.y = dataSet.dots[i][1];
        tempShape.radius = 1;

        results.push_back(tempShape);

    }


}

bool isRect(DiagramShape &inputShape, const primitivesStruct &dataSet){

    int x1, x2, x3, x4, y1, y2, y3, y4;

    x1 = inputShape.points[0].x; 
    x2 = inputShape.points[1].x; 
    x3 = inputShape.points[2].x; 
    x4 = inputShape.points[3].x; 

    y1 = inputShape.points[0].y; 
    y2 = inputShape.points[1].y; 
    y3 = inputShape.points[2].y; 
    y4 = inputShape.points[3].y; 



  int cx,cy;
  int dd1,dd2,dd3,dd4;

  cx=(x1+x2+x3+x4)/4;
  cy=(y1+y2+y3+y4)/4;

  dd1=pow((cx-x1), 2) +pow((cy-y1), 2);
  dd2=pow((cx-x2), 2)+pow((cy-y2), 2);
  dd3=pow((cx-x3), 2)+pow((cy-y3), 2);
  dd4=pow((cx-x4), 2)+pow((cy-y4), 2);
  return dd1==dd2 && dd1==dd3 && dd1==dd4;
    

}

bool isSquare(DiagramShape &inputShape, const primitivesStruct &dataSet){

    vector<cv::Point> proxyPoints(inputShape.points.begin(), inputShape.points.end());

    cv::Point maxX, minX, maxY, minY;
    vector<cv::Point>::iterator result;

    //find xmax and xmin
    result = max_element(proxyPoints.begin(), proxyPoints.end(), xValComp);
    maxX = *result;
    proxyPoints.erase(result);

    result = min_element(proxyPoints.begin(), proxyPoints.end(), xValComp);
    minX = *result;
    proxyPoints.erase(result);

    //find ymax and ymin
    result = max_element(proxyPoints.begin(), proxyPoints.end(), yValComp);
    maxY = *result;
    proxyPoints.erase(result);

    result = min_element(proxyPoints.begin(), proxyPoints.end(), yValComp);
    minY = *result;
    proxyPoints.erase(result);

    int d2 = pow(euclideanDistance(maxX, minX),2);  // from p1 to p2
    int d3 = pow(euclideanDistance(maxX, maxY),2);  // from p1 to p3
    int d4 = pow(euclideanDistance(maxX, minY),2);  // from p1 to p4
 
    // If lengths if (p1, p2) and (p1, p3) are same, then
    // following conditions must met to form a square.
    // 1) Square of length of (p1, p4) is same as twice
    //    the square of (p1, p2)
    // 2) p4 is at same distance from p2 and p3

    bool sqFlag = false;
    if (d2 == d3 && 2*d2 == d4)
    {
        int d = pow(euclideanDistance(minX, minY),2);
        return (d == pow(euclideanDistance(maxX, minY),2) && d == d2);
    }
 
    // The below two cases are similar to above case
    if (d3 == d4 && 2*d3 == d2)
    {
        int d = pow(euclideanDistance(minX, maxY),2);
        return (d == pow(euclideanDistance(minX, minY),2) && d == d3);
    }
    if (d2 == d4 && 2*d2 == d3)
    {
        int d = pow(euclideanDistance(minX, maxY),2);
        return (d == pow(euclideanDistance(maxY, minY),2) && d == d2);
    }
 
    return sqFlag;

}

void findSimpleCurvesAndDots(const primitivesStruct &dataSet, vector<DiagramShape> &results){

    DiagramShape tempShape;
    cv::Point tempPoint;
    for(int i = 0; i < dataSet.circles.size(); ++i){
        tempShape.ids.push_back(dataSet.circleID[i]);
        tempShape.radius = dataSet.circles[i][2];
        tempPoint.x = dataSet.circles[i][0];
        tempPoint.y = dataSet.circles[i][1];
        tempShape.center = tempPoint;
        tempShape.type = ShapeType::CIRCLE;
        results.push_back(tempShape);

    }

    tempShape.ids.clear();
    tempShape.radius = 10;

    for(int j = 0; j < dataSet.dots.size(); ++j){

        tempShape.ids.push_back(dataSet.dotID[j]);
        tempShape.radius = 10;
        tempPoint.x = dataSet.dots[j][0];
        tempPoint.y = dataSet.dots[j][1];
        tempShape.center = tempPoint;
        tempShape.type = ShapeType::DOT;
        results.push_back(tempShape);
    }

}

void setCenter(DiagramShape &polygon){
    cv::Point maxX, minX, maxY, minY;
    vector<cv::Point>::iterator result;

    //find xmax and xmin
    result = max_element(polygon.points.begin(), polygon.points.end(), xValComp);
    maxX = *result;

    result = min_element(polygon.points.begin(), polygon.points.end(), xValComp);
    minX = *result;

    //find ymax and ymin
    result = max_element(polygon.points.begin(), polygon.points.end(), yValComp);
    maxY = *result;

    result = min_element(polygon.points.begin(), polygon.points.end(), yValComp);
    minY = *result;

    //Now, just average these values and you will get a reasonable estimate of the "center."

    polygon.center.x = (maxX.x + minX.x)/2;
    polygon.center.y = (maxY.y + minY.y)/2;


}

void setCentersForAll(vector<DiagramShape> &results){

    for(int i = 0; i < results.size(); ++i){

        if(results[i].ids.size() > 1){

            setCenter(results[i]);
        }
      

    }
}

void findShapes(vector<DiagramShape> &primitives, vector<vector<DiagramShape> > &results) {
  // *** IMPLEMENT ***
}

void findRelationsInSingleDiagram(vector<DiagramShape> &diagram){

    Relation tempRelate;

    for(int i = 0; i < diagram.size()-1; ++i){

        for(int j = i+1; j < diagram.size(); ++j){
            //cout << i << j << endl;
            //set inside of for 2 shapes
            int eq = shapeInside(diagram[i], diagram[j]);           

            if(eq == 1){
                tempRelate.otherShape_id = diagram[j].id;
                tempRelate.relation_type = RelationType::INSIDE;
                diagram[i].relations.push_back(tempRelate);

            }else if (eq == 2){
                tempRelate.otherShape_id = diagram[i].id;
                tempRelate.relation_type = RelationType::INSIDE;
                diagram[j].relations.push_back(tempRelate);

            }else{

                bool beq = shapeOverlap(diagram[i], diagram[j]);;

                //set overlap/touch for the two shapes
                if(beq){
                    
                    tempRelate.otherShape_id = diagram[j].id;
                    tempRelate.relation_type = RelationType::OVERLAP;
                    diagram[i].relations.push_back(tempRelate);

                    tempRelate.otherShape_id = diagram[i].id;
                    tempRelate.relation_type = RelationType::OVERLAP;
                    diagram[j].relations.push_back(tempRelate);
                }

            } 

            //Set right of/left of for the 2 shapes
            if(diagram[i].center.x > diagram[j].center.x){
                tempRelate.otherShape_id = diagram[j].id;
                tempRelate.relation_type = RelationType::RIGHT_OF;
                diagram[i].relations.push_back(tempRelate);

                tempRelate.otherShape_id = diagram[i].id;
                tempRelate.relation_type = RelationType::LEFT_OF;
                diagram[j].relations.push_back(tempRelate);

            }else if(diagram[i].center.x < diagram[j].center.x){
                tempRelate.otherShape_id = diagram[j].id;
                tempRelate.relation_type = RelationType::LEFT_OF;
                diagram[i].relations.push_back(tempRelate);

                tempRelate.otherShape_id = diagram[i].id;
                tempRelate.relation_type = RelationType::RIGHT_OF;
                diagram[j].relations.push_back(tempRelate);
            }

            //set above and below for the two shapes
            if(diagram[i].center.y > diagram[j].center.y){
                tempRelate.otherShape_id = diagram[j].id;
                tempRelate.relation_type = RelationType::BELOW;
                diagram[i].relations.push_back(tempRelate);

                tempRelate.otherShape_id = diagram[i].id;
                tempRelate.relation_type = RelationType::ABOVE;
                diagram[j].relations.push_back(tempRelate);

            }else if(diagram[i].center.y < diagram[j].center.y){
                tempRelate.otherShape_id = diagram[j].id;
                tempRelate.relation_type = RelationType::ABOVE;
                diagram[i].relations.push_back(tempRelate);

                tempRelate.otherShape_id = diagram[i].id;
                tempRelate.relation_type = RelationType::BELOW;
                diagram[j].relations.push_back(tempRelate);
            }

        }


    }
}

void findRelations(vector<vector<DiagramShape> > &diagrams){
  // *** IMPLEMENT ***

    for(int i = 0; i < diagrams.size(); ++i){

        findRelationsInSingleDiagram(diagrams[i]);

    }
}

// This is used only for the shape overlap/inside functions.
void drawFilledShape(const DiagramShape& shape, cv::Mat& image, cv::Scalar color) {
    if (shape.type == CIRCLE || shape.type == DOT) {
        cv::Point center = shape.center;
        // Scale to 1000x1000 grid from 100x100 grid.
        center.x *= 10;
        center.y *= 10;
        cv::circle(image, center, shape.radius*10, color, -1);
    }
    else {
        vector<cv::Point> contour = shape.points;
        // Remove the last point, which is a repeat of the first point.
        contour.pop_back();
        // Scale to 1000x1000 grid from 100x100 grid.
        for (int i = 0; i < contour.size(); ++i) {
            contour[i] *= 10;
        }
        
        
        // create a pointer to the data as an array of points (via a conversion to
        // a Mat() object)
        
        const cv::Point *pts = (const cv::Point*) cv::Mat(contour).data;
        int npts = cv::Mat(contour).rows;
        
        cv::fillPoly(image, &pts, &npts, 1, color);
    }
}

bool shapeOverlap(const DiagramShape& shape1, const DiagramShape& shape2) {
    // Test overlap by doing per-pixel comparison of the two shapes.
    cv::Mat image1 = cv::Mat::zeros(cv::Size(1000, 1000), CV_8UC1);
    cv::Mat image2 = cv::Mat::zeros(cv::Size(1000, 1000), CV_8UC1);
    
    drawFilledShape(shape1, image1, cv::Scalar(255));
    drawFilledShape(shape2, image2, cv::Scalar(255));
    
    cv::Mat overlap;
    cv::bitwise_and(image1, image2, overlap);
    //cv::imshow("overlap test", overlap);
    
    //cv::waitKey();
    if (cv::countNonZero(overlap) > 0) {
        return true;
    }
    else {
        return false;
    }
}

int shapeInside(const DiagramShape& shape1, const DiagramShape& shape2) {
    // Compare the overlapping area and each shape. If a shape is entirely within the overlap, then that shape is within the other shape.
    cv::Mat image1 = cv::Mat::zeros(cv::Size(1000, 1000), CV_8UC1);
    cv::Mat image2 = cv::Mat::zeros(cv::Size(1000, 1000), CV_8UC1);
    
    drawFilledShape(shape1, image1, cv::Scalar(255));
    drawFilledShape(shape2, image2, cv::Scalar(255));
    
    cv::Mat overlap;
    cv::bitwise_and(image1, image2, overlap);
    
    // Check if shape 1 is entirely within the overlap. If so, then the overlap between shape 1 and the overlap should be the entirety of shape 1.
    cv::Mat shape1_overlap;
    cv::bitwise_and(image1, overlap, shape1_overlap);
    bool eq1 = cv::countNonZero(shape1_overlap != image1) == 0;
    
    // Check if shape 2 is entirely within the overlap.
    cv::Mat shape2_overlap;
    cv::bitwise_and(image2, overlap, shape2_overlap);
    bool eq2 = cv::countNonZero(shape2_overlap != image2) == 0;

    
    if (eq1) {
        return 1;
    }
    else if (eq2) {
        return 2;
    }
    else {
        return 0;
    }
}

