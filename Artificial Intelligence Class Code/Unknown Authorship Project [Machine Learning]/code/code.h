#include <glob.h>
#include <libgen.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <cassert>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <stack>
#include <dirent.h>
#include <queue>

using namespace std;

// Function to remove leading, trailing, and extra space from a string.
    // Inputs a string with extra spaces.
    // Outputs a string with no extra spaces.
string remove_extra_space(string str);

    // Tokenizer.
    // Input: string
    // Output: list of lowercased words from the string
vector<string> word_tokenize(string input_string);

class trainDoc{

public:

    trainDoc(){}

    vector<string> text;

    void importAndTokenizeText(string fileName);

    bool docHasWord(string word);
};

class singleKnownAuthor{

public:

    singleKnownAuthor(){}

    vector<trainDoc> docs;

    int findN_ci(string word);

};

class features{

public: 

    map<string, double> conditionalProbabilities;
    string stopWord;
    double CCE;


    features(string word, map<string, singleKnownAuthor> &authorSet, int N);
    features(){};
    bool operator<(const features& other) const{

        return (CCE < other.CCE);
    }

};



class unknownAuthorDoc{
public:

    unknownAuthorDoc(){}
    vector<string> text;

    void importAndTokenizeText(string fileName);

    double calcProbGivenClass(string thisClass, map<string, features> &feats, int N, int Nc);

    bool docHasWord(string word);

};

class PreProcess{

public:

    map<string, features> stopWords;
    string _directory;
    map<string, singleKnownAuthor> trainingSet;
    map<string, unknownAuthorDoc> testingSet;
    vector<int> truth;
    int numDocs;

    PreProcess(){}
    void importAllFiles();

    void importStopWords();

    void importCorrectAnswers();

    

    // Function to print the confusion matrix.
    // Argument 1: "actual" is a list of integer class labels, one for each test example.
    // Argument 2: "predicted" is a list of integer class labels, one for each test example.
    // "actual" is the list of actual (ground truth) labels.
    // "predicted" is the list of labels predicted by your classifier.
    // "actual" and "predicted" MUST be in one-to-one correspondence.
    // That is, actual[i] and predicted[i] stand for testfile[i].
    void printConfMat(vector<int>actual, vector<int>predicted);

};

class BernoulliNaiveBayes{

private:

    

    string answerForOneDoc(string nameOf, unknownAuthorDoc &currentProblem, map<string, features> &SW);



public:
    PreProcess p;
    vector<string> answerIds;
    priority_queue<features> rankedFeatures;

    BernoulliNaiveBayes(PreProcess pp){
        p = pp;
    }

    //the training in this implementation is now done with the preprocess.


    // Define Test function
    void test(map<string, features> &SW);
    void testUsingTop(int num);

    void rankFeats();
    void printFeatures(int amount);

};