#include <iostream>
#include <sstream>
#include "diagram_shape.h"

//used to compare doubles
bool doubleEqual(double a, double b) {
    return fabs(a - b) < DBL_EPSILON;
}

//finds distance between two points
double euclideanDistance(cv::Point pt1, cv::Point pt2) {
    cv::Point diff = pt1 - pt2;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

//checks if it is possible to to combine this and shape, if it is return true and edit result_combination
//Can be used when finding shapes in diagram_processing.cpp
bool DiagramShape::combine(const DiagramShape &shape, DiagramShape &result_combination) const {
  // *** IMPLEMENT *** 
  return false;
}


bool DiagramShape::convertToSquare() {
  // *** IMPLEMENT ***
  this->type = SQUARE;
  return true;   
}

bool DiagramShape::convertToRectangle() {
  // *** IMPLEMENT ***
  this->type = RECTANGLE; 
  return true;
}

bool DiagramShape::convertToTriangle() {
    // *** IMPLEMENT *** //
    this->type = TRIANGLE;
    return true;
}

//You do not need to edit this function
void DiagramShape::draw(cv::Mat &img, cv::Scalar color) {
    // scale everything to 1000 x 1000 (better resolution) and plot
    if (this->type == CIRCLE){
        cv::Point center = this->center;
        center.x *= 10;
        center.y *= 10;
        cv::circle(img, center, this->radius*10, color);
    }
    else if (this->type == DOT) {
        cv::Point center = this->center;
        center.x *= 10;
        center.y *= 10;
        cv::circle(img, center, this->radius*10, color, -1);
    }
    else {
        // Draw lines between each set of points.
        for (int i = 0; i < this->points.size()-1; ++i) {
            cv::Point pt1 = this->points[i];
            pt1.x *= 10;
            pt1.y *= 10;
            cv::Point pt2 = this->points[i+1];
            pt2.x *= 10;
            pt2.y *= 10;
            cv::line(img, pt1, pt2, color);
        }

        cv::Point pt1 = this->points[0];
        pt1.x *= 10;
        pt1.y *= 10;
        cv::Point pt2 = this->points[points.size()-1];
        pt2.x *= 10;
        pt2.y *= 10;
        cv::line(img, pt1, pt2, color);

    }
}

