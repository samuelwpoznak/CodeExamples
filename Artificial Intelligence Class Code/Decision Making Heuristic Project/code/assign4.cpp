#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include "assign4.h"


using namespace std;

int main(){
   cout << "=======PROBLEM 1========" << endl;
   cout << "Calculating thresholds for problem 1...." << endl;
   BinarySearch b;
   b.search(-1.7, -.001); 
   //cout << b.thresholds.size() << endl;
   cout << endl << endl << "============PROBLEM 2============" << endl << "MONTY CARLO" << endl;
   MonteCarlo m = MonteCarlo(-.04, 3, 0);
   vector<double> tenRuns = m.run(10);
   vector<double> hunRuns = m.run(100);
   vector<double> thoRuns = m.run(1000);

   double tenAvg = 0, hunAvg = 0, thoAvg = 0, tenStd = 0, hunStd = 0, thoStd = 0;


   cout << "RESULTS FOR 10 RUNTHRUS:" << endl << "--------------" << endl;
   ///////////////////////////////////////////////////////////////////////
   ////////////////////ten mean + std dev ////////////////////////////////
   for(int i = 0; i < tenRuns.size(); ++i){

      cout << tenRuns[i] << endl;
      tenAvg+=tenRuns[i];


   }

   tenAvg=tenAvg/(double)tenRuns.size();

   for(int i = 0; i < tenRuns.size(); ++i){

      tenStd += pow(tenRuns[i] - tenAvg, 2);


   }

   tenStd = sqrt(tenStd/tenRuns.size());

   cout << endl << "MEAN: " << endl;
   cout << tenAvg << "+/-" << tenStd << endl << endl;

   cout << "RESULTS FOR 100 RUNTHRUS:" << endl << "--------------" << endl;
   ///////////////////////////////////////////////////////////////////////
   ////////////////////hundred mean + std dev ////////////////////////////////

   for(int i = 0; i < hunRuns.size(); ++i){
      cout << hunRuns[i] << endl;
      hunAvg+=hunRuns[i];


   }

   hunAvg=hunAvg/(double)hunRuns.size();

   for(int i = 0; i < hunRuns.size(); ++i){

      hunStd += pow(hunRuns[i] - hunAvg, 2);


   }

   hunStd = sqrt(hunStd/hunRuns.size());
   cout << endl << "MEAN: " << endl;
   cout << hunAvg << "+/-" << hunStd << endl;

   cout << "RESULTS FOR 1000 RUNTHRUS:" << endl << "--------------" << endl;
   ///////////////////////////////////////////////////////////////////////
   ////////////////////thousand mean + std dev ////////////////////////////////
   for(int i = 0; i < thoRuns.size(); ++i){

      thoAvg+=thoRuns[i];


   }

   thoAvg=thoAvg/(double)thoRuns.size();

   for(int i = 0; i < thoRuns.size(); ++i){
      cout << thoRuns[i] << endl;
      thoStd += pow(thoRuns[i] - thoAvg, 2);


   }

   thoStd = sqrt(thoStd/thoRuns.size());
   cout << endl << "MEAN: " << endl;
   cout << thoAvg << "+/-" << thoStd << endl << endl << endl;
   /////////////////////////////////////////////////////////////////////////




   cout << "=======PROBLEM 3========" << endl;
   cout << "Calculating thresholds for problem 3...." << endl;
   BinarySearch3 bb;
   bb.search(0., .999999);

   cout << "Assignment Finished. Have a good day!" << endl;


    return 0;
}

bool Policy::operator==(const Policy &other){

        for(int i = 0; i < 4; ++i){

            for(int j = 0; j < 3; ++j){

                if(map[i][j] != SQUARE::OPEN) continue;
                //cout << actions[i][j] << " " << other.actions[i][j] << endl;
                if(actions[i][j] != other.actions[i][j]){

                    //cout << actions[i][j] << " " << other.actions[i][j] << endl;
                    return false;
                }

            }

        }

        return true;

};

Policy& Policy::operator=(const Policy &rhs){

        

        for(int i = 0; i < 4; ++i){

            for(int j = 0; j < 3; ++j){

               this->map[i][j] = rhs.map[i][j];
               this->utilities[i][j] = rhs.utilities[i][j];
               this->actions[i][j] = rhs.actions[i][j];
            }

        }

        this->R_S = rhs.R_S;
        return *this;

};
bool Policy::Equals(Policy other){

        for(int i = 0; i < 4; ++i){

            for(int j = 0; j < 3; ++j){
                if(map[i][j] != SQUARE::OPEN) continue;
                //cout << actions[i][j] << " " << other.actions[i][j] << endl;
                if(actions[i][j] != other.actions[i][j]){

                    //cout << actions[i][j] << " " << other.actions[i][j] << endl;
                    return false;
                }

            }

        }

        return true;

};

Policy::Policy(double RofS){
   //cout << "F1" << endl;

    for(int i = 0; i < 4; ++i){

        for(int j = 0; j < 3; ++j){
            map[i][j] = SQUARE::OPEN;
            utilities[i][j] = ((double)(rand() % 10))/((double)10);
            //cout << utilities[i][j] << endl;
        }
    }

    map[1][1] = SQUARE::WALL;
    utilities[1][1] = 0;
    map[3][2] = SQUARE::POS;
    utilities[3][2] = 1;
    map[3][1] = SQUARE::NEG;
    utilities[3][1] = -1;
    this->R_S = RofS;
    ValueIteration();
    FindActions();

}


double Policy::FindUtilityOfState(int x, int y){
    //cout << "F2" << endl;
    //cout << x << " " << y << endl;
    double result;
    if(map[x][y] == SQUARE::POS) result = 1;
    else if(map[x][y] == SQUARE::NEG) result = -1;
    else result = R_S + FindMaxSum(x, y);
    //cout << result << endl;
    utilities[x][y] = result;
    return result;
}

double Policy::FindMaxSum(int x, int y){
   //cout << "F3" << endl;
    double bestNum;

    //cout << "up" << endl;
    double upNum = 0;
    if(y != 2 && map[x][y+1] != SQUARE::WALL) upNum += .8*utilities[x][y+1];
    else upNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) upNum += .1*utilities[x-1][y];
    else upNum += .1*utilities[x][y];
    if(x != 3 && map[x+1][y] != SQUARE::WALL) upNum += .1*utilities[x+1][y];
    else upNum += .1*utilities[x][y];

   // cout << upNum << endl;
    bestNum = upNum;

    //cout << "left" << endl;
    double leftNum = 0;
    if(x != 0 && map[x-1][y] != SQUARE::WALL) leftNum += .8*utilities[x-1][y];
    else leftNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) leftNum += .1*utilities[x][y-1];
    else leftNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) leftNum += .1*utilities[x][y+1];
    else leftNum += .1*utilities[x][y];
    //cout << leftNum << endl;
    if(leftNum > bestNum){
        bestNum = leftNum;

    }
    //cout << "down" << endl;
    double downNum = 0;
    if(y != 0 && map[x][y-1] != SQUARE::WALL) downNum += .8*utilities[x][y-1];
    else downNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) downNum += .1*utilities[x-1][y];
    else downNum += .1*utilities[x][y];
    if(x != 3 && map[x+1][y] != SQUARE::WALL) downNum += .1*utilities[x+1][y];
    else downNum += .1*utilities[x][y];
    //cout << downNum << endl;
    if(downNum > bestNum){
        bestNum = downNum;

    }
    //cout << "right" << endl;
    double rightNum = 0;
    if(x != 3 && map[x+1][y] != SQUARE::WALL) rightNum += .8*utilities[x+1][y];
    else rightNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) rightNum += .1*utilities[x][y-1];
    else rightNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) rightNum += .1*utilities[x][y+1];
    else rightNum += .1*utilities[x][y];
    //cout << rightNum << endl;
    if(rightNum > bestNum){
        bestNum = rightNum;

    }

    //cout << bestNum << endl;

    //cout << best << endl;
    return bestNum;
}

void Policy::ValueIteration(){
   //cout << "F4" << endl;
    double delta = 0;
   
    int i = 0;
   
    do{
       //cout << "do" << endl;

        delta = 0;
        for(int x = 0; x < 4; ++x){
            //cout << "xloop" << endl;
            for(int y = 0; y < 3; ++y){
                //cout << "yloop" << endl;
                if(map[x][y] != SQUARE::WALL){
                    //cout << x << " " << y << endl;
                    double Util = utilities[x][y];
                    double UtilPrime = FindUtilityOfState(x, y);
                    //cout << Util - UtilPrime << endl;
                    if(abs(UtilPrime - Util) > delta){
                        
                        delta = abs(UtilPrime - Util);
                        //cout << delta << endl;
                    } 
                }
            }
        }


    }while(delta > .0000001);

    return;

}

void Policy::PrintActions(){
   
   cout << "(" << R_S << ")" << endl;
   for(int j = 2; j >= 0; --j){

      for(int i = 0; i < 4; ++i){
   
         if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::UP){

            cout << "^";
         }else if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::LEFT){

            cout << "<";
         }else if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::DOWN){

            cout << "v";
         }else if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::RIGHT){

            cout << ">";
         }else if (map[i][j] == SQUARE::WALL){

            cout << "X";
         }else if(map[i][j] == SQUARE::POS){

            cout << "+1";

         }else if(map[i][j] == SQUARE::NEG){

            cout << "-1";

         }
            
   

      }
      cout << endl;

   }

   for(int j = 2; j >= 0; --j){

      for(int i = 0; i < 4; ++i){
   
         //cout << utilities[i][j] << " ";
            
   

      }
      cout << endl;

   }



}

ACTION Policy::FindBestAction(int x, int y){

    //cout << "F3" << endl;
    double bestNum = 0;
    ACTION best;

    //cout << "up" << endl;
    double upNum = 0;
    if(y != 2 && map[x][y+1] != SQUARE::WALL) upNum += .8*utilities[x][y+1];
    else upNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) upNum += .1*utilities[x-1][y];
    else upNum += .1*utilities[x][y];
    if(x != 3 && map[x+1][y] != SQUARE::WALL) upNum += .1*utilities[x+1][y];
    else upNum += .1*utilities[x][y];

   //cout << upNum << endl;

    bestNum = upNum;
    best = ACTION::UP;

   //cout << "left" << endl;
    double leftNum = 0;
    if(x != 0 && map[x-1][y] != SQUARE::WALL) leftNum += .8*utilities[x-1][y];
    else leftNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) leftNum += .1*utilities[x][y-1];
    else leftNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) leftNum += .1*utilities[x][y+1];
    else leftNum += .1*utilities[x][y];
    //cout << leftNum << endl;
    if(leftNum > bestNum){
        bestNum = leftNum;
        best = ACTION::LEFT;

    }
    //cout << "down" << endl;
    double downNum = 0;
    if(y != 0 && map[x][y-1] != SQUARE::WALL) downNum += .8*utilities[x][y-1];
    else downNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) downNum += .1*utilities[x-1][y];
    else downNum += .1*utilities[x][y];
    if(x != 3 && map[x+1][y] != SQUARE::WALL) downNum += .1*utilities[x+1][y];
    else downNum += .1*utilities[x][y];
    //cout << downNum << endl;
    if(downNum > bestNum){
        bestNum = downNum;
        best = ACTION::DOWN;

    }
    //cout << "right" << endl;
    double rightNum = 0;
    if(x != 3 && map[x+1][y] != SQUARE::WALL) rightNum += .8*utilities[x+1][y];
    else rightNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) rightNum += .1*utilities[x][y-1];
    else rightNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) rightNum += .1*utilities[x][y+1];
    else rightNum += .1*utilities[x][y];
   // cout << rightNum << endl;
    if(rightNum > bestNum){
        bestNum = rightNum;
        best = ACTION::RIGHT;

    }

    //cout << bestNum << endl;

    //cout << best << endl;
    return best;



}

void Policy::FindActions(){

   for(int i = 0; i < 4; ++i){

      for(int j = 0; j < 3; ++j){

         if(map[i][j] == SQUARE::OPEN) actions[i][j] = FindBestAction(i, j);


      }
   }




}

void BinarySearch::search(double low, double high){

    if(thresholds.size() == 8) return;

    double med = (high+low)/2.0;

    Policy H = Policy(high);
    Policy L = Policy(low);
    Policy M = Policy(med);

    //H.PrintActions();
    //cout << endl << endl;

    //L.PrintActions();
    //cout << endl << endl;

   if(!(L == M)){

      search(low, med);

   }else{

      Policy T = Policy(med + .0001);
      if(!(M == T)){
         thresholds.push_back(med);
         cout << "New Threshold Found!" << endl;
         cout << "R(s) Val:" << med << endl;

         cout << "R(s) Policy:" << endl;
         M.PrintActions(); 
         //cout << endl << endl;
         cout << "Next-Sequence Policy:" << endl;
         T.PrintActions();
         cout << "----------------------------------------" << endl << endl;
         //thresholds.push_back(med);

         search(med+.0001, -.001);

      }else{

        search(med, high);


      }



   }


}

MonteCarlo::MonteCarlo(double r, int startX, int startY){

   curX = startX;
   curY = startY;
   reward = r;
   p = Policy(reward);


}

vector<double> MonteCarlo::run(int runs){


   int startX = curX, startY = curY;
   vector<double> result;

   for(int i = 0; i < runs; ++i){

      result.push_back(rewardFromSingleRun());

      curX = startX;
      curY = startY;
   }

   return result;



}

double MonteCarlo::rewardFromSingleRun(){

   double result = 0;


   while(p.map[curX][curY] == SQUARE::OPEN){

      double pseudoRandVar = ((double)(rand() % 10))/((double)10);
      //cout << pseudoRandVar << endl;
      ACTION move = p.actions[curX][curY];

      if(pseudoRandVar == .8){

         --move;

      }else if (pseudoRandVar == .9){

         ++move;

      }
      


      if(move == ACTION::UP){

         if(curY != 2 && p.map[curX][curY+1] != SQUARE::WALL){

            curY++;
         }

      }else if(move == ACTION::DOWN){

         if(curY != 0 && p.map[curX][curY-1] != SQUARE::WALL){

            curY--;

         }


      }else if(move == ACTION::LEFT){

         if(curX != 0 && p.map[curX-1][curY] != SQUARE::WALL){

            curX--;
         }

      }else if(move == ACTION::RIGHT){

         if(curX != 3 && p.map[curX+1][curY] != SQUARE::WALL){
            curX++;
         }

      }else{



         //cout << "error" << endl;
         //cout << (int)move << endl;

      }
      //cout << "move" << endl;
      //cout << (int)move << endl;
      result += p.R_S;

   }

   if(p.map[curX][curY] == SQUARE::POS) result += 1.0;
   if(p.map[curX][curY] == SQUARE::NEG) result -= 1.0;


   return result;



}

bool Problem3::operator==(const Problem3 &other){

        for(int i = 0; i < 3; ++i){

            for(int j = 0; j < 3; ++j){

                if(map[i][j] != SQUARE::OPEN) continue;
                //cout << actions[i][j] << " " << other.actions[i][j] << endl;
                if(actions[i][j] != other.actions[i][j]){

                    
                    if(i == 0 && (j == 0 || j == 2)) continue;
                    else return false;
                }

            }

        }

        return true;

};

Problem3& Problem3::operator=(const Problem3 &rhs){

        

        for(int i = 0; i < 3; ++i){

            for(int j = 0; j < 3; ++j){

               this->map[i][j] = rhs.map[i][j];
               this->utilities[i][j] = rhs.utilities[i][j];
               this->actions[i][j] = rhs.actions[i][j];
               this->rewards[i][j] = rhs.rewards[i][j];
            }

        }

        this->gamma = rhs.gamma;
        return *this;

};

double Problem3::FindUtilityOfState(int x, int y){
    //cout << "F2" << endl;
    //cout << x << " " << y << endl;
    double result;
    if(map[x][y] == SQUARE::POS) result = 10;
    else result = rewards[x][y] + gamma*FindMaxSum(x, y);
    //cout << result << endl;
    utilities[x][y] = result;
    return result;
}

double Problem3::FindMaxSum(int x, int y){
   //cout << "F3" << endl;
    double bestNum;

    //cout << "up" << endl;
    double upNum = 0;
    if(y != 2 && map[x][y+1] != SQUARE::WALL) upNum += .8*utilities[x][y+1];
    else upNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) upNum += .1*utilities[x-1][y];
    else upNum += .1*utilities[x][y];
    if(x != 2 && map[x+1][y] != SQUARE::WALL) upNum += .1*utilities[x+1][y];
    else upNum += .1*utilities[x][y];

   // cout << upNum << endl;
    bestNum = upNum;

    //cout << "left" << endl;
    double leftNum = 0;
    if(x != 0 && map[x-1][y] != SQUARE::WALL) leftNum += .8*utilities[x-1][y];
    else leftNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) leftNum += .1*utilities[x][y-1];
    else leftNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) leftNum += .1*utilities[x][y+1];
    else leftNum += .1*utilities[x][y];
    //cout << leftNum << endl;
    if(leftNum > bestNum){
        bestNum = leftNum;

    }
    //cout << "down" << endl;
    double downNum = 0;
    if(y != 0 && map[x][y-1] != SQUARE::WALL) downNum += .8*utilities[x][y-1];
    else downNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) downNum += .1*utilities[x-1][y];
    else downNum += .1*utilities[x][y];
    if(x != 2 && map[x+1][y] != SQUARE::WALL) downNum += .1*utilities[x+1][y];
    else downNum += .1*utilities[x][y];
    //cout << downNum << endl;
    if(downNum > bestNum){
        bestNum = downNum;

    }
    //cout << "right" << endl;
    double rightNum = 0;
    if(x != 2 && map[x+1][y] != SQUARE::WALL) rightNum += .8*utilities[x+1][y];
    else rightNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) rightNum += .1*utilities[x][y-1];
    else rightNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) rightNum += .1*utilities[x][y+1];
    else rightNum += .1*utilities[x][y];
    //cout << rightNum << endl;
    if(rightNum > bestNum){
        bestNum = rightNum;

    }

    //cout << bestNum << endl;

    //cout << best << endl;
    return bestNum;
}

void Problem3::ValueIteration(){
   //cout << "F4" << endl;
    double delta = 0;
   
    int i = 0;
    
   
    do{
       //cout << "do" << endl;

        delta = 0;
        for(int x = 0; x < 3; ++x){
            //cout << "xloop" << endl;
            for(int y = 0; y < 3; ++y){
                //cout << "yloop" << endl;
                if(map[x][y] != SQUARE::WALL){
                    //cout << x << " " << y << endl;
                    double Util = utilities[x][y];
                    double UtilPrime = FindUtilityOfState(x, y);
                    //cout << Util - UtilPrime << endl;
                    if(abs(UtilPrime - Util) > delta){
                        
                        delta = abs(UtilPrime - Util);
                        //cout << delta << endl;
                    } 
                }
            }
        }


    }while(delta > .00001*((1-gamma)/gamma));

    return;

}

void Problem3::PrintActions(){

   cout << "(" << gamma << ")" << endl;
   for(int j = 2; j >= 0; --j){

      for(int i = 0; i < 3; ++i){
   
         if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::UP){

            cout << "^";
         }else if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::LEFT){

            cout << "<";
         }else if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::DOWN){

            cout << "v";
         }else if(map[i][j] == SQUARE::OPEN && actions[i][j] == ACTION::RIGHT){

            cout << ">";
         }else if (map[i][j] == SQUARE::WALL){

            cout << "X";
         }else if(map[i][j] == SQUARE::POS){

            cout << "+10";

         }else if(map[i][j] == SQUARE::NEG){

            cout << "-1";

         }
            
   

      }
      cout << endl;

   }

   for(int j = 2; j >= 0; --j){

      for(int i = 0; i < 3; ++i){
   
         //cout << utilities[i][j] << " ";
            
   

      }
      cout << endl;

   }



}

ACTION Problem3::FindBestAction(int x, int y){

    //cout << "F3" << endl;
    double bestNum = 0;
    ACTION best;

    //cout << "up" << endl;
    double upNum = 0;
    if(y != 2 && map[x][y+1] != SQUARE::WALL) upNum += .8*utilities[x][y+1];
    else upNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) upNum += .1*utilities[x-1][y];
    else upNum += .1*utilities[x][y];
    if(x != 2 && map[x+1][y] != SQUARE::WALL) upNum += .1*utilities[x+1][y];
    else upNum += .1*utilities[x][y];

   //cout << upNum << endl;

    bestNum = upNum;
    best = ACTION::UP;

   //cout << "left" << endl;
    double leftNum = 0;
    if(x != 0 && map[x-1][y] != SQUARE::WALL) leftNum += .8*utilities[x-1][y];
    else leftNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) leftNum += .1*utilities[x][y-1];
    else leftNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) leftNum += .1*utilities[x][y+1];
    else leftNum += .1*utilities[x][y];
    //cout << leftNum << endl;
    if(leftNum > bestNum){
        bestNum = leftNum;
        best = ACTION::LEFT;

    }
    //cout << "down" << endl;
    double downNum = 0;
    if(y != 0 && map[x][y-1] != SQUARE::WALL) downNum += .8*utilities[x][y-1];
    else downNum += .8*utilities[x][y];
    if(x != 0 && map[x-1][y] != SQUARE::WALL) downNum += .1*utilities[x-1][y];
    else downNum += .1*utilities[x][y];
    if(x != 2 && map[x+1][y] != SQUARE::WALL) downNum += .1*utilities[x+1][y];
    else downNum += .1*utilities[x][y];
    //cout << downNum << endl;
    if(downNum > bestNum){
        bestNum = downNum;
        best = ACTION::DOWN;

    }
    //cout << "right" << endl;
    double rightNum = 0;
    if(x != 2 && map[x+1][y] != SQUARE::WALL) rightNum += .8*utilities[x+1][y];
    else rightNum += .8*utilities[x][y];
    if(y != 0 && map[x][y-1] != SQUARE::WALL) rightNum += .1*utilities[x][y-1];
    else rightNum += .1*utilities[x][y];
    if(y != 2 && map[x][y+1] != SQUARE::WALL) rightNum += .1*utilities[x][y+1];
    else rightNum += .1*utilities[x][y];
   // cout << rightNum << endl;
    if(rightNum > bestNum){
        bestNum = rightNum;
        best = ACTION::RIGHT;

    }

    //cout << bestNum << endl;

    //cout << best << endl;
    return best;



}

void Problem3::FindActions(){

   for(int i = 0; i < 3; ++i){

      for(int j = 0; j < 3; ++j){

         if(map[i][j] == SQUARE::OPEN) actions[i][j] = FindBestAction(i, j);


      }
   }
}

void BinarySearch3::search(double low, double high){


    if(thresholds.size() == 5) return;

    double med = (high+low)/2.0;

    Problem3 H = Problem3(high);
    Problem3 L = Problem3(low);
    Problem3 M = Problem3(med);

    //H.PrintActions();
    //cout << endl << endl;

    //L.PrintActions();
    //cout << endl << endl;

   if(!(L == M)){

      search(low, med);

   }else{

      Problem3 T = Problem3(med + .00001);
      if(!(M == T)){

         thresholds.push_back(med);
         cout << "New Threshold Found!" << endl;
         cout << "Gamma Val:" << med << endl;

         cout << "Gamma Policy:" << endl;
         M.PrintActions(); 
         //cout << endl << endl;
         cout << "Next-Sequence Policy:" << endl;
         T.PrintActions();
         cout << "----------------------------------------" << endl << endl;

         search(med+.00001, .99999);

      }else{

        search(med, high);


      }



   }

}




