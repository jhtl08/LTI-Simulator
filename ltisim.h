// ltisim.h
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 28, 2024

#ifndef LTISIMHEADERDEF
#define LTISIMHEADERDEF

#include <iostream>
#include <sstream>

using namespace std;

// signal import function
int signalImport(string fileName, double **xData);

// validate integer number function
bool isInteger(string data);

// validate floating number function
bool isFloat(string data);

// system import function
bool systemImport(std::string systemFilename, int& Mplus1, int& N, 
double*& aCoeff, double*& bCoeff);

// instruction function
void getInstructions();

void pushData(double*& xData, int& dataSize, double numInput);

double computeOutput(int& Mplus1, int& N, int& xsize, int& ysize, 
double* aCoeff, double* bCoeff, double* xData, double* yData);

#endif