#include "io.h"
#include "diagram_shape.h"
#include "diagram_processing.h"

//Fills up primitives vector with data from given file name
void readInput(string fileName, vector<DiagramShape> &primitives){
  // *** IMPLEMENT ***
}


class Colors{
private:
    vector<cv::Scalar> colors;
    
    int color_index;
    
public:
    
    //had to create constructor for the vector init to work on my compiler.
    Colors(){ 
        color_index = -1;
        colors.push_back(cv::Scalar(176, 23, 31));
        colors.push_back(cv::Scalar(125, 38, 205));
        colors.push_back(cv::Scalar(100, 149, 237));
        colors.push_back(cv::Scalar(67, 205, 128));
        colors.push_back(cv::Scalar(238, 238, 0));
        colors.push_back(cv::Scalar(255, 128, 0));
        colors.push_back(cv::Scalar(142, 142, 56));
    }

    cv::Scalar getColor(){
        
        if(color_index == colors.size()){
            color_index = 0;
        }
        else{
            color_index++;
        }
        
        return colors[color_index];
        
    }
};

struct diagrams_comparator
{
    inline bool operator() (const vector<DiagramShape>& struct1, const vector<DiagramShape>& struct2)
    {
        return (struct1.size() < struct2.size());
    }
};

struct less_than_shape
{
    inline bool operator() (const DiagramShape& struct1, const DiagramShape& struct2)
    {
        //return true;
        if(struct1.type < struct2.type) return true;
        else if(struct1.type > struct2.type){ return false;}
        else if(struct1.ids[0] < struct1.ids[0]){ return true;}
        else{ return false; }
    }
};

void printOutput(vector < vector<DiagramShape> > diagrams, string output_name){
    
    //cout << output_name << endl;
    //sorts vector from smallest size to largest size
    //additonal sorting may need to be done when putting solutions into the pdf
    sort(diagrams.begin(), diagrams.end(), diagrams_comparator());
    //cout << diagrams.size() << endl;
    
    //loop through all given diagrams to print them out
    for(int i = 0; i < diagrams.size(); i++){
        ofstream os;
        os.open(output_name+ "_" + to_string(i) + ".txt");
        
        //creates a 1000 by 1000 matrix for the diagram to be displayed
        cv::Mat image(1000, 1000, CV_8UC3, cv::Scalar(255, 255, 255));
        
        string relations = "";
        
        Colors diagram_colors;
        
        //sort shapes
        sort(diagrams[i].begin(), diagrams[i].end(), less_than_shape());
        
        string shape_notation = "";
        string relations_str = "";
        
        //for each shape in the diagram
        for(auto k: diagrams[i]){
            
            k.draw(image, diagram_colors.getColor());
            os << k.id << " = ";
            switch(k.type){
                case CIRCLE:
                    os << "circle(";
                    shape_notation = shape_notation + "shape(" + k.id +", circle)\n";
                    break;
                case DOT:
                    os << "dot(";
                    shape_notation = shape_notation + "shape(" + k.id +", dot)\n";
                    break;
                case SQUARE:
                    os << "square(";
                    shape_notation = shape_notation + "shape(" + k.id +", square)\n";
                    break;
                case RECTANGLE:
                    os << "rectangle(";
                    shape_notation = shape_notation + "shape(" + k.id +", rectangle)\n";
                    break;
                case TRIANGLE:
                    os << "triangle(";
                    shape_notation = shape_notation + "shape(" + k.id +", triangle)\n";
                    break;
                case SCC:
                    os << "scc(";
                    shape_notation = shape_notation + "shape(" + k.id +", scc)\n";
                    break;
            }
            for(int w= 0; w < k.ids.size(); w++){
                if(w !=0){
                    os << ", ";
                }
                os << k.ids[w] ;
            }
            os << ")" <<endl;
            
            //print relations
            for(auto rel : k.relations){
                switch(rel.relation_type){
                    case LEFT_OF:
                        relations_str = relations_str + "left_of(" + k.id + ", " + rel.otherShape_id + ")\n";
                        break;
                    case ABOVE:
                         relations_str = relations_str + "above(" + k.id + ", " + rel.otherShape_id + ")\n";
                        break;
                    case INSIDE:
                         relations_str = relations_str + "inside_of(" + k.id + ", " + rel.otherShape_id + ")\n";
                        break;
                    case OVERLAP:
                         relations_str = relations_str + "overlap(" + k.id + ", " + rel.otherShape_id + ")\n";
                        break;
                    case TOUCH:
                         relations_str = relations_str + "touch(" + k.id + ", " + rel.otherShape_id + ")\n";
                        break;
                    default:
                        break;
                }
            }
            
        }//end of shape
        
        os << shape_notation;
        os << relations_str;
        
        os << endl;
        os.close();
        
        cv::imwrite(output_name + "_" + to_string(i) + ".jpg", image );
    }//end of diagram
}
