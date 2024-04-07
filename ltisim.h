// ltisim.h
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 08, 2024

#ifndef LTISIMHEADERDEF
#define LTISIMHEADERDEF

#include <iostream>
#include <sstream>

using namespace std;

class ltiSystem
{
  private:
    int Mplus1, N;
    double *acoef;
    double *bcoef;

  public:
    //empty constructor
    ltiSystem();

    // import funciton
    bool ltiSystemImport(string fileName);
};

// signal import function
int SignalImport(string fileName, double **xData);

#endif