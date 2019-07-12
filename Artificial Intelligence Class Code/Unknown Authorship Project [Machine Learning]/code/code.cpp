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
#include "code.h"
#include <queue>

using namespace std;

// Function to remove leading, trailing, and extra space from a string.
    // Inputs a string with extra spaces.
    // Outputs a string with no extra spaces.
string remove_extra_space(string str){
        string buf; // Have a buffer string
        stringstream ss(str); // Insert the string into a stream
        vector<string> tokens; // Create vector to hold our words
        while (ss >> buf) tokens.push_back(buf);
        const char* const delim = " ";
        ostringstream imploded;
        copy(tokens.begin(), tokens.end(), ostream_iterator<string>(imploded, delim));
        return imploded.str();
    }

    // Tokenizer.
    // Input: string
    // Output: list of lowercased words from the string
 vector<string> word_tokenize(string input_string){
        string punctuations = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
        string extra_space_removed = remove_extra_space(input_string);
        string punctuation_removed = "";
        for (unsigned i = 0; i < extra_space_removed.length(); i++) {
            char curr = extra_space_removed[i];
            if (punctuations.find(curr) == string::npos) punctuation_removed += curr;
        }
        transform(punctuation_removed.begin(), punctuation_removed.end(), punctuation_removed.begin(), ::tolower);
        string buf; // Have a buffer string
        stringstream ss(punctuation_removed); // Insert the string into a stream
        vector<string> tokens; // Create vector to hold our words
        while (ss >> buf) tokens.push_back(buf);
        return tokens;
}

features::features(string word, map<string, singleKnownAuthor> &authorSet, int N){

        map<string, singleKnownAuthor>::iterator it;
        stopWord = word;
        CCE = 0;

        for(it = authorSet.begin(); it != authorSet.end(); ++it){

            pair<string, double> probabilty;
            probabilty.first = it->first;


            int Nc = it->second.docs.size();

            int nci = it->second.findN_ci(word);


            probabilty.second = ( (double)nci + (double)1 )  /  ( (double)Nc + (double)2 );

            conditionalProbabilities.insert(probabilty);
            CCE = CCE + (probabilty.second * log2(probabilty.second) * ((double)Nc/(double)N));

        }

        CCE = CCE*(-1);

        map<string, double>::iterator it2;



    }

    void trainDoc::importAndTokenizeText(string fileName){

        ifstream myFile;
        myFile.open(fileName);
        string word;

        while(getline(myFile, word)){

            vector<string> temp = word_tokenize(word);
            text.insert(text.end(), temp.begin(), temp.end());

        }

        myFile.clear();
        myFile.seekg(0, myFile.beg);
        myFile.close();
    }

    bool trainDoc::docHasWord(string word){

        for(int i = 0; i < text.size(); ++i){

            if(text[i] == word){

                return true;
            }


        }

        return false;
    }

    int singleKnownAuthor::findN_ci(string word){

        int result = 0;


        for(int i = 0; i < docs.size(); ++i){

            if(docs[i].docHasWord(word)){

                ++result;
            }


        }

        return result;
    }

    void unknownAuthorDoc::importAndTokenizeText(string fileName){

        ifstream myFile;
        myFile.open(fileName);
        string word;

        while(getline(myFile, word)){

            vector<string> temp = word_tokenize(word);
            text.insert(text.end(), temp.begin(), temp.end());

        }

        myFile.clear();
        myFile.seekg(0, myFile.beg);
        myFile.close();

    }

    bool unknownAuthorDoc::docHasWord(string word){

        for(int i = 0; i < text.size(); ++i){

            if(text[i] == word){

                return true;
            }

        }

        return false;


    }

    double unknownAuthorDoc::calcProbGivenClass(string thisClass, map<string, features> &feats, int N, int Nc){

        double prior = (double)Nc / (double)N;
        prior = log2(prior);

        double cond = 0;

        map<string, features>::iterator it;
        for(it = feats.begin(); it != feats.end(); ++it){

            if(docHasWord(it->first)){

                double temp = it->second.conditionalProbabilities.find(thisClass)->second;

                cond = cond + log2(temp);
            }else{

                double temp = 1 - it->second.conditionalProbabilities.find(thisClass)->second;
                cond = cond + log2(temp);

            }
        }

        return prior + cond;


    }

    void PreProcess::importAllFiles(){

        DIR *pdir = NULL;
        struct dirent *pent = NULL;

        pdir = opendir(_directory.c_str());

        numDocs = 0;

        while( (pent = readdir(pdir)) ){

            string theName(pent->d_name);

            if(theName == "." || theName == "..") continue;

            if(theName.find("sample") != string::npos){


                pair<string, unknownAuthorDoc> temp;
                temp.first = theName;
                unknownAuthorDoc sec;
                sec.importAndTokenizeText(_directory + "/" + theName);
                temp.second = sec;

                testingSet.insert(temp);

            }else{
                //this means it is a training file

                ++numDocs;
                int pos = theName.find("-");
                string classId = theName.substr(0, pos);

                map<string, singleKnownAuthor>::iterator it = trainingSet.find(classId);

                if(it != trainingSet.end()){

                    trainDoc tempTrain;
                    tempTrain.importAndTokenizeText(_directory + "/" + theName);

                    it->second.docs.push_back(tempTrain);

                }else{

                    trainDoc tempTrain;
                    tempTrain.importAndTokenizeText(_directory + "/" + theName);

                    pair<string, singleKnownAuthor> newAuthor;
   
                    newAuthor.first = classId;
                    newAuthor.second.docs.push_back(tempTrain);

                    trainingSet.insert(newAuthor);

                }


            }
        }

        closedir(pdir);
        return;

    }

    void PreProcess::importStopWords(){

        ifstream myFile;
        myFile.open("./stopwords.txt");
        string word;

        while(myFile >> word){

            bool flag = false;

            map<string, unknownAuthorDoc>::iterator it;

                pair<string, features> featureData;
                featureData.first = word;

                features wordData(word, trainingSet, numDocs);
                featureData.second = wordData;

                stopWords.insert(featureData);


        }

        myFile.clear();
        myFile.seekg(0, myFile.beg);
        myFile.close();

    }



    // Function to print the confusion matrix.
    // Argument 1: "actual" is a list of integer class labels, one for each test example.
    // Argument 2: "predicted" is a list of integer class labels, one for each test example.
    // "actual" is the list of actual (ground truth) labels.
    // "predicted" is the list of labels predicted by your classifier.
    // "actual" and "predicted" MUST be in one-to-one correspondence.
    // That is, actual[i] and predicted[i] stand for testfile[i].
    void PreProcess::printConfMat(vector<int>actual, vector<int>predicted){
        vector<int> all_labels;
        assert(actual.size() == predicted.size());
        for (vector<int>::iterator i = actual.begin(); i != actual.end(); i++)
            all_labels.push_back((*i));
        for (vector<int>::iterator i = predicted.begin(); i != predicted.end(); i++)
            all_labels.push_back((*i));
        sort( all_labels.begin(), all_labels.end() );
        all_labels.erase( unique( all_labels.begin(), all_labels.end() ), all_labels.end() );
        map<pair<int,int>, unsigned> confmat;  // Confusion Matrix
        int itt = 0;
        for (vector<int>::iterator i = actual.begin(); i != actual.end(); i++){
            int a = (*i);
            pair<int, int> pp = make_pair(a, predicted[itt]);
            if (confmat.find(pp) == confmat.end()) confmat[pp] = 1;
            else confmat[pp] += 1;
            itt++;
        }
        cout << "\n\n";
        cout << "0 ";  // Actual labels column (aka first column)
        vector<int> tmp_labels;
        for (vector<int>::iterator i = all_labels.begin(); i != all_labels.end(); i++){
            int label2 = (*i);
            cout << label2 << " ";
            tmp_labels.push_back(label2);
        }
        cout << "\n";
        for (vector<int>::iterator i = all_labels.begin(); i != all_labels.end(); i++){
            int label = (*i);
            cout << label << " ";
            for (vector<int>::iterator i2 = tmp_labels.begin(); i2 != tmp_labels.end(); i2++){
                int label2 = (*i2);
                pair<int, int> pp = make_pair(label, label2);
                if (confmat.find(pp) == confmat.end()) cout << "0 ";
                else cout << confmat[pp] << " ";
            }
            cout << "\n";
        }
    }

    void PreProcess::importCorrectAnswers(){

        ifstream myFile;
        myFile.open("test_ground_truth.txt");

        string problem = _directory.substr(_directory.length()-1);
        string line;

        while(getline(myFile, line)){

            if(line.find(problem + "sample") != string::npos){

                int ans = stoi(line.substr(line.length()-2));
                truth.push_back(ans);



            }
        }

        return;





    }


    string BernoulliNaiveBayes::answerForOneDoc(string nameOf, unknownAuthorDoc &currentProblem, map<string, features> &SW){

        string candidate = p.trainingSet.begin()->first;
        singleKnownAuthor skaTEMP = p.trainingSet.begin()->second;

        double currentBest = currentProblem.calcProbGivenClass(candidate, SW, p.numDocs, skaTEMP.docs.size());


        map<string, singleKnownAuthor>::iterator it;

        string winner = candidate;

        for(it = p.trainingSet.begin(); it != p.trainingSet.end(); ++it){

            candidate = it->first;
            skaTEMP = it->second;


            double tempProb = currentProblem.calcProbGivenClass(candidate, SW, p.numDocs, skaTEMP.docs.size());

            if(tempProb > currentBest){


                currentBest = tempProb;
                winner = candidate;

            }

        }

        return winner;


    }
    //the training in this implementation is now done with the preprocess.


    // Define Test function
    void BernoulliNaiveBayes::test(map<string, features> &SW){
      
         map<string, unknownAuthorDoc>::iterator it;

         for(it = p.testingSet.begin(); it != p.testingSet.end(); ++it){

            answerIds.push_back(answerForOneDoc(it->first, it->second, SW));

        }




    }

    void BernoulliNaiveBayes::rankFeats(){

        map<string, features>::iterator it;

        for(it = p.stopWords.begin(); it != p.stopWords.end(); ++it){

            rankedFeatures.push(it->second);

        }


    }

    void BernoulliNaiveBayes::printFeatures(int amount){

        priority_queue<features> tempQ = rankedFeatures;

        for(int i = 0; i < amount; ++i){

            features temp = tempQ.top();
            tempQ.pop();

            cout << "feature " << i+1 << ": " << temp.stopWord << " --- CCE: " << temp.CCE << endl;

        }
    }

    void BernoulliNaiveBayes::testUsingTop(int num){

        map<string, features> top;
        priority_queue<features> tempQ = rankedFeatures;
        answerIds.clear();

        for(int i = 0; i < num; ++i){

            pair<string, features> temp;
            temp.first = tempQ.top().stopWord;
            temp.second = tempQ.top();

            tempQ.pop();
            top.insert(temp);
        }

        test(top);
    }

int main(int argc, char** argv) {
    
    PreProcess allData;
    allData._directory = argv[1];

    //hacky way to fix an issue prior to release//
    allData._directory.pop_back();
    // //

    allData.importAllFiles();

    allData.importStopWords();
    allData.importCorrectAnswers();


    BernoulliNaiveBayes BNB(allData);
    BNB.test(BNB.p.stopWords);

    vector<int> ourAnswers;
    vector<int> topAnswers;
    cout << endl << endl;
    cout << "Hi! Welcome to Project3 output!" << endl << endl;
    cout << "..............................." << endl << endl;

    for(int i = 0; i < BNB.answerIds.size(); ++i){

        ourAnswers.push_back(stoi(BNB.answerIds[i].substr(BNB.answerIds[i].length()-2)));

    }

    int numCorrect = 0;
    topAnswers = ourAnswers;


    for(int k = 0; k < topAnswers.size(); ++k){

            if(BNB.p.truth[k] == topAnswers[k]){

                ++numCorrect;
            }

    }

    
    cout << "Test accuracy for all data: " << (double)numCorrect/(double)topAnswers.size() << endl << endl;
    cout << ". . . . . . . . . . . . . ." << endl << endl;

    

    cout << "Confusion Matrix for the current problem: " << endl;
    cout << "-----------------------------------------";

    allData.printConfMat(BNB.p.truth, ourAnswers);
    cout << endl << ". . . . . . . . . . . . . ." << endl << endl;

    cout << "The top 20 features of the data are: \n";
    cout << "------------------------------------\n";

    BNB.rankFeats();
    BNB.printFeatures(20);

    cout << endl << ". . . . . . . . . . . . . ." << endl << endl;
    cout << "I will now output the point-data for the feature curve." << endl;
    cout << "Please copy and paste this data into ‘http://www.shodor.org/interactivate/activities/SimplePlot/'." << endl << endl;

    for(int j = 10; j < BNB.p.stopWords.size(); j+=10){

        BNB.testUsingTop(j);
        cout << "(" << j << ",";
        int numCorrect = 0;
        ourAnswers.clear();

        for(int i = 0; i < BNB.answerIds.size(); ++i){

            ourAnswers.push_back(stoi(BNB.answerIds[i].substr(BNB.answerIds[i].length()-2)));

        }

        for(int k = 0; k < BNB.answerIds.size(); ++k){

            if(BNB.p.truth[k] == ourAnswers[k]){

                ++numCorrect;
            }

        }

        cout << (double)numCorrect/(double)ourAnswers.size() << ")\n";

    }

    numCorrect = 0;

    for(int k = 0; k < BNB.answerIds.size(); ++k){

            if(BNB.p.truth[k] == topAnswers[k]){

                ++numCorrect;
            }

    }

    cout << "(" << BNB.p.stopWords.size() << "," << (double)numCorrect/(double)topAnswers.size() << ")" << endl;
    cout << "...............................";
    cout << "\n\n\nProgram Finished! Have a nice day!\n\n";









}



