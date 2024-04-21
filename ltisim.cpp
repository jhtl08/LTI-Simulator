// ltisim.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 21, 2024

#include "ltisim.h"
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

ltiSystem::ltiSystem()
{
  Mplus1 = 1;
  N = 1;
  acoef = new double[N];
  bcoef = new double[Mplus1];
  acoef[0] = 0;
  bcoef[0] = 0;
}

bool ltiSystem::ltiSystemImport(string fileName)
{ // NOTE: this is not yet robust
  // opens and checks the file
  ifstream isystemFile;
  isystemFile.open(fileName);
  if (!isystemFile.is_open())
  { // fail import feedback
    cout << "Unable to import a valid LTI system from "
    << fileName << endl;
    return false;
  }

  // parsing M+1 and N
  string line;
  getline(isystemFile, line);
  stringstream ss1(line);
  ss1 >> Mplus1;
  getline(isystemFile, line);
  stringstream ss2(line);
  ss2 >> N;

  // parsing nonrecursive coefs (b)
  bcoef = new double[Mplus1];
  for(int i=0; i<Mplus1; i++)
  {
    getline(isystemFile, line);
    stringstream ss(line);
    ss >> bcoef[i];
  }

  // parsing recursive coefs (a)
  acoef = new double[N];
  for(int i=0; i<N; i++)
  {
    getline(isystemFile, line);
    stringstream ss(line);
    ss >> acoef[i];
  }

  // successful import feedback
  cout << "System obtained from " << fileName 
  << ". recursive coefs: " << N 
  << ", nonrecursive coefs: " << Mplus1 << endl;
  return true;
}

int SignalImport(string fileName, double **xData)
{
  // opens and checks the files
  ifstream isignalFile;
  isignalFile.open(fileName);
  if (!isignalFile.is_open())
  { // fail import feedback
    cout << "Unable to import a valid signal from "
         << fileName << endl;
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
      cout << "Encountered nonfloat as first element in " << fileName
           << "\nNo elements imported.\n"
           << endl;
      return 0;
    }
  }
  else // Ex. 'a'
  {
    cout << "Encountered nonfloat as first element in " << fileName
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
      cout << "Encountered nonfloat as first element in " << fileName
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
      cout << "Parsing of input from " << fileName
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
        cout << "Parsing of input from " << fileName
             << " stopped at duration " << vect_elements.size()
             << " due to invalid element." << endl;
        break;
      }
    }
    else // Ex. 'a'
    {
      cout << "Parsing of input from " << fileName
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
       << ", obtained from " << fileName
       << endl;

  isignalFile.close();
  return duration;
}