// ltisim.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 28, 2024

#include "ltisim.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <cmath>

using namespace std;


int signalImport(string signalFileName, double **xData)
{
  // opens and checks the files
  ifstream isignalFile;
  isignalFile.open(signalFileName);
  if (!isignalFile.is_open())
  { // fail import feedback
    cout << "Unable to import a valid signal from "
         << signalFileName << endl;
    return 0;
  }
  // parsing elements to vector
  string line;
  getline(isignalFile, line);

  vector<double> vect_elements;

  double temp;
  string word;
  stringstream ss(line);

  ss >> word;
  stringstream ss1(word);
  if (ss1 >> temp) // Ex. '-4' '-4a'
  {
    if (ss1.eof()) // Ex. '-4'
    {
      vect_elements.push_back(temp);
    }
    else // Ex. '-4a'
    {
      cout << "Encountered nonfloat as first element in " << signalFileName
           << "\nNo elements imported.\n"
           << endl;
      return 0;
    }
  }
  else // Ex. 'a'
  {
    cout << "Encountered nonfloat as first element in " << signalFileName
         << "\nNo elements imported.\n"
         << endl;
    return 0;
  }
  // Clear word to reuse it
  word.clear();

  // Check if there is another double
  ss >> word;
  stringstream ss2(word);
  if (ss2 >> temp) // Ex. '-4' '-4a'
  {
    if (ss2.eof()) // Ex. '-4'
    {
      if (vect_elements[0] != 0)
      { // Message to inform disregarding of signal file start index
        cout << "Starting index of " << vect_elements[0]
             << " ignored.\nStarting index set to 0." << endl;
      }
      vect_elements[0] = temp;
    }
    else // Ex. '-4a'
    {
      cout << "Encountered nonfloat as first element in " << signalFileName
           << "\nNo elements imported.\n"
           << endl;
      return 0;
    }
  }

  // Clear word to reuse it
  word.clear();

  // Check valid values
  while (getline(isignalFile, line))
  {
    if (line.empty())
    {
      cout << "Parsing of input from " << signalFileName
           << " stopped at duration " << vect_elements.size()
           << " due to an empty line." << endl;
      break;
    }

    stringstream ss3(line);
    ss3 >> word;

    stringstream ss4(word);
    if (ss4 >> temp)
    {
      if (ss4.eof()) // Ex. '4'
      {
        vect_elements.push_back(temp);
      }
      else // Ex. '4a'
      {
        cout << "Parsing of input from " << signalFileName
             << " stopped at duration " << vect_elements.size()
             << " due to invalid element." << endl;
        break;
      }
    }
    else // Ex. 'a'
    {
      cout << "Parsing of input from " << signalFileName
           << " stopped at duration " << vect_elements.size()
           << " due to invalid element." << endl;
      break;
    }
    word.clear();
  }

  int duration = vect_elements.size();

  // allocate memory
  *xData = new double[duration];

  // convert vector to array
  for (int i = 0; i < duration; i++)
  {
    (*xData)[i] = vect_elements[i];
  }

  // successful import feedback
  cout << "Signal with start index 0, duration " << duration
       << ", obtained from " << signalFileName
       << endl;

  isignalFile.close();
  return duration;
}

void getInstructions() 
{
  string instructions = 
  "----------------------------------------------------"
  "\nValid Input Commands\n"
  "help - provides instructions on how to use the application\n"
  "system filename* - extract coefficients of an LTI system from "
  "filename if it is a valid LTI system file\n"
  "signal filename* - extract a signal from filename if it is a "
  "valid signal file, signal is treated as input to the system\n"
  "any floating point number (e.g., 0, 1.1) - the inputted number "
  "is treated as the next input to the system\n"
  "clear - clear the application's memory of previous inputs and "
  "outputs to 0 \n"
  "cls - only clears the screen\n" 
  "exit - exit the application\n\n"
  "*change filename accordingly\n"
  "----------------------------------------------------\n";

  cout << instructions << endl;
}

bool isInteger(string data)
{
  istringstream iss(data);
  int integer_number;
  iss >> integer_number;

  return iss.eof() && !iss.fail();
}

bool isFloat(string data) 
{
  istringstream iss(data);
  float floating_number;
  iss >> floating_number;
  
  return iss.eof() && !iss.fail(); 
}

bool systemImport(string systemFilename, int& Mplus1, int& N, 
double*& aCoeff, double*& bCoeff)
{
  ifstream isystemFile;
  isystemFile.open(systemFilename);
  if (!isystemFile.is_open()) {
      cout << "Unable to import a valid LTI system from " << systemFilename << endl;
      return false;
  }

  // parsing elements to vector
  string line;
  string data;

  // Check Mplus1
  getline(isystemFile, line);
  stringstream ss(line);
  ss >> data;

  if (isInteger(data)) 
  {
    Mplus1 = stoi(data);
  } 
  else 
  {
    cout << "Error: Mplus1 is not an integer." << endl;
    return false;
  }

  // Check N
  getline(isystemFile, line);
  stringstream ss1(line);
  ss1 >> data;

  if (isInteger(data)) 
  {
    N = stoi(data);
  } 
  else 
  {
    cout << "Error: N is not an integer." << endl;
    return false;
  }

  // Check bCoeff
  bCoeff = new double[Mplus1];

  for(int i=0; i < Mplus1; i++)
  {
    getline(isystemFile, line);
    stringstream ss2(line);
    ss2 >> data;

    if(isFloat(data))
    {
      bCoeff[i] = stof(data);
    }
    else
    {
      cout << "Error: bCoeff is not a float" << endl;
      return false;
    }
  }

  // Check aCoeff
  aCoeff = new double[N];

  for(int i=0; i < N; i++)
  {
    getline(isystemFile, line);
    stringstream ss3(line);
    ss3 >> data;

    if(isFloat(data))
    {
      aCoeff[i] = stof(data);
    }
    else
    {
      cout << "Error: aCoeff is not a float" << endl;
      return false;
    }
  }

  cout << "System obtained from " << systemFilename 
  << ". recursive coefs: " << N 
  << ", nonrecursive coefs: " << Mplus1 << endl;

  return true;
}

double sumProduct(double* Coeff, double* data, int& Coeff_size, 
int& size) {
    double result = 0.0;

    for (int i = 0; i < Coeff_size; i++)
    {
        result += Coeff[i] * data[(size - 1) - i];
    }

    return result;
}

double computeOutput(int& Mplus1, int& N, int& xsize, int& ysize, 
double* aCoeff, double* bCoeff, double* xData, double* yData) {
    double output;

    output = -sumProduct(aCoeff, yData, N, ysize) + sumProduct(bCoeff, xData, Mplus1, xsize) ;

    return output;
}

void pushData(double*& Data, int& dataSize, double numInput)
{
  double* temp = new double[dataSize + 1];
  
  for (int i = 0; i < dataSize; ++i) 
  {
    temp[i] = Data[i];
  }

  temp[dataSize] = numInput;

  delete[] Data;

  Data = temp;

  dataSize++;
}
