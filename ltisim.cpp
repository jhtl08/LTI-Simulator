// ltisim.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// May 1, 2024

#include "ltisim.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <cmath>

using namespace std;


int signalImport(string signalFileName, double **importedData)
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
  *importedData = new double[duration];

  // convert vector to array
  for (int i = 0; i < duration; i++)
  {
    (*importedData)[i] = vect_elements[i];
  }

  // successful import feedback
  cout << "Signal with start index 0, duration " << duration
       << ", obtained from " << signalFileName
       << endl;

  isignalFile.close();
  return duration;
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
      cout << "Unable to import a valid LTI system from " << 
      systemFilename << endl;
      return false;
  }
  
  // temp variables to avoid immediately overwriting parameters
  int temp_Mplus1, temp_N;
  double *temp_aCoeff;
  double *temp_bCoeff;

  // parsing elements to vector
  string line;
  string data;

  // Check Mplus1
  getline(isystemFile, line);
  stringstream ss(line);
  ss >> data;

  if (isInteger(data)) 
  {
    temp_Mplus1 = stoi(data);
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
    temp_N = stoi(data);
  } 
  else 
  {
    cout << "Error: N is not an integer." << endl;
    return false;
  }

  // Check bCoeff
  temp_bCoeff = new double[temp_Mplus1];

  for(int i=0; i < temp_Mplus1; i++)
  {
    if(!getline(isystemFile, line))
    {
      cout << "Error: insufficient coefficients" << endl;
      return false;
    }
    if (line.empty())
    {
      cout << "Error: encountered empty line implying eof " << 
      "while there's insufficient coefficients" << endl;
      return false;
    }
    stringstream ss2(line);
    ss2 >> data;

    if(isFloat(data))
    {
      temp_bCoeff[i] = stof(data);
    }
    else
    {
      cout << "Error: encountered non-float coefficient" << endl;
      return false;
    }

    data = "";
  }

  // Check aCoeff
  temp_aCoeff = new double[temp_N];

  for(int i=0; i < temp_N; i++)
  {
    if(!getline(isystemFile, line))
    {
      cout << "Error: insufficient coefficients" << endl;
      return false;
    }
    if (line.empty())
    {
      cout << "Error: encountered empty line implying eof " << 
      "while there's insufficient coefficients" << endl;
      return false;
    }
    stringstream ss3(line);
    ss3 >> data;

    if(isFloat(data))
    {
      temp_aCoeff[i] = stof(data);
    }
    else
    {
      cout << "Error: encountered non-float coefficient" << endl;
      return false;
    }

    data = "";
  }
  
  // Since no errors, overwrite LTI system
  Mplus1 = temp_Mplus1;
  N = temp_N;

  bCoeff = new double[Mplus1];

  for (int i = 0; i < Mplus1; i++)
  {
    bCoeff[i] = temp_bCoeff[i];
  }

  aCoeff = new double[N];
  
  for (int i = 0; i < N; i++)
  {
    aCoeff[i] = temp_aCoeff[i];
  }

  delete[] temp_aCoeff;
  delete[] temp_bCoeff;
  
  // Successful system import feedback
  cout << "New system obtained from " << systemFilename 
  << "\nrecursive coefs: " << N 
  << ", nonrecursive coefs: " << Mplus1 << endl;
  cout << "READY" << endl;

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
