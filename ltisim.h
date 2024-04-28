// ltisim.h
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 21, 2024

#ifndef LTISIMHEADERDEF
#define LTISIMHEADERDEF

#include <iostream>
#include <sstream>

using namespace std;


void setInitialConditions(double*& xData, double*& yData);

// signal import function
int SignalImport(string fileName, double **xData);

// validate integer number function
bool isInteger(string data);

// validate floating number function
bool isFloat(string data);

// system import function
bool SystemImport(std::string systemFilename, int& Mplus1, int& N, double*& aCoeff, double*& bCoeff);

// instruction function
string getInstructions();

// system details function (to be removed before final submission)
void systemDetails( int& Mplus1, int& N, double*& aCoeff, double*& bCoeff);

#endif