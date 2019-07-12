#include <vector>
#include <string>
#include <iostream>



using namespace std;


enum class ACTION {BEGIN_OF_LIST=-1, UP=0, RIGHT=1, DOWN=2, LEFT=3, END_OF_LIST=4};

ACTION& operator++( ACTION &c ) {
  using IntType = typename std::underlying_type<ACTION>::type;
  c = static_cast<ACTION>( static_cast<IntType>(c) + 1 );
  if ( (int)c == 4 )
    c = static_cast<ACTION>(0);
  return c;
}

ACTION& operator--( ACTION &c ) {
  using IntType = typename std::underlying_type<ACTION>::type;
  c = static_cast<ACTION>( static_cast<IntType>(c) - 1 );
  if ( (int)c == -1 )
    c = static_cast<ACTION>(3);
  return c;
}

enum SQUARE {WALL=0, POS=1, NEG=2, OPEN=3};

class Policy{

    public:
    ACTION actions[4][3];
    SQUARE map[4][3];
    double utilities[4][3];
    double R_S = 0;

    Policy(){};
    Policy(double RofS);
    bool operator==(const Policy &other);
    Policy & operator=(const Policy &rhs);
    bool Equals(Policy other);
    double FindMaxSum(int x, int y);
    double FindUtilityOfState(int x, int y);
    void ValueIteration();
    void PrintActions();
    ACTION FindBestAction(int x, int y);
    void FindActions();

};

class BinarySearch{

    public:
    vector<double> thresholds;

    void search(double low, double high);

};

class MonteCarlo{

   public:
   double reward;
   Policy p;
   int curX, curY;
   MonteCarlo(double r, int startX, int startY);



   vector<double> run(int runs);
   double rewardFromSingleRun();

};

class Problem3{

   public:
   ACTION actions[3][3];
   SQUARE map[3][3];
   double utilities[3][3];
   double rewards[3][3];
   double gamma;

   Problem3(){};
   Problem3(double discountRate){

      for(int i = 0; i < 3; ++i){
         for(int j = 0; j < 3; ++j){
            map[i][j] = SQUARE::OPEN;
            rewards[i][j] = -1.0;
            utilities[i][j] = ((double)(rand() % 10))/((double)10);

         }
      }

      map[2][2] = SQUARE::POS;
      rewards[0][2] = 3.0;
      rewards[2][2] = 10.0;
      utilities[2][2] = 10.0;

      gamma = discountRate;
      ValueIteration();
      FindActions();

   };

   bool operator==(const Problem3 &other);
   Problem3 & operator=(const Problem3 &other);
   double FindMaxSum(int x, int y);
   double FindUtilityOfState(int x, int y);
   void ValueIteration();
   void PrintActions();
   ACTION FindBestAction(int x, int y);
   void FindActions();
   



};

class BinarySearch3{

    public:
    vector<double> thresholds;

    void search(double low, double high);

};
