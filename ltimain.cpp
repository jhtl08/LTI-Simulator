// ltimain.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// May 1, 2024

#include "ltisim.h"
#include <fstream>

using namespace std;

int main()
{
  
  ofstream logFile;
  logFile.open("ltisim-log.txt", ios::app); // init to append
  if (!logFile.is_open())
  { // fail export feedback
    cout << "Error: Unable to open Log ltisim-log.txt" << endl;
    cout << "Application will run without logging" << endl;
  }

  bool systemFile = false;

  // System Variables
  int Mplus1 = 0, N = 0;
  double *bCoeff, *aCoeff;

  // Signal Variables
  double *xData;
  double *yData;
  double *importedData;

  // Initial Conditions
  int xDataSize = 2;
  xData = new double[xDataSize];
  xData[0] = 0.0; // x(-2)
  xData[1] = 0.0; // x(-1)

  int yDataSize = 2;
  yData = new double[yDataSize];
  yData[0] = 0.0; // y(-2)
  yData[1] = 0.0; // y(-1)

  cout << "LTISim" << endl;
  cout << "Type \"help\" for more information" << endl; 

  // For Checking Inputs
  string line;
  string userInput;
  double numInput;

  while(true)
  {
    cout << "\nltisim> ";
    getline(cin, line);
    stringstream ss(line);

    // for float inputs
    if (ss >> numInput) // for floating point number inputs
    {
      if (systemFile) // check if LTI system is ready
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          pushData(xData, xDataSize, numInput);
          double result = computeOutput(Mplus1, N, xDataSize, 
          yDataSize, aCoeff, bCoeff, xData, yData);
          cout << result << endl;
          logFile << numInput << " \t" << result << endl;
          
          pushData(yData, yDataSize, result);
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }
      else
      {
        cout << "Cannot simulate. No system defined" << endl;
      }
      continue; // if input is valid float, skip other input types
    }
    
    ss.clear();

    if (ss >> userInput) // for string inputs
    {
      // "help" input
      if (userInput == "help")
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          getInstructions();
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }
      else if (userInput == "system") // "system [filename]" input
      {
        if (ss >> userInput) // check for second arg (filename)
        {
          if (!(ss >> userInput) && ss.eof()) // check file name
          {
            if (systemFile) // if there's already a system
            {
              bool importSuccess = systemImport(userInput, 
              Mplus1, N, aCoeff, bCoeff);
              if (importSuccess) 
              {
                cout << "LTI system overwritten" << endl;
                logFile << "new system" << endl;
                logFile << "ready" << endl;
              }
              else
              {
                cout << "Previous LTI system still active" << endl;
              }
            }
            else
            {
              systemFile = systemImport(userInput, Mplus1, N, 
              aCoeff, bCoeff);
              logFile << "ready" << endl;
            }
          }
          else
          {
            cout << "Invalid System" << endl;
          }          
        }
        else
        {
          cout << "filename is missing from the input" << endl;
        }
      }
      else if (userInput == "signal") // "signal [filename]" input
      {
        if (ss >> userInput) // check for second arg (filename)
        {
          if (systemFile)
          {
            if (!(ss >> userInput) && ss.eof()) // check file name
            {
              int importSize = signalImport(userInput, &importedData);
              for (int i = 0; i < importSize; i++)
              {
                pushData(xData, xDataSize, importedData[i]);
                double result = computeOutput(Mplus1, N, xDataSize, 
                  yDataSize, aCoeff, bCoeff, xData, yData);
                cout << importedData[i] << " \t" << result << endl;
                logFile << importedData[i] << " \t" << result << endl;

                pushData(yData, yDataSize, result);
              }
            }
            else
            {
              cout << "Invalid System" << endl;
            }
          }
          else
          {
            cout << "Cannot simulate. No system defined" << endl;
          }
        }
        else
        {
          cout << "filename is missing from the input" << endl;
        }
      }
      else if (userInput == "clear") // "clear" input
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          systemFile = false;
          Mplus1 = 0;
          N = 0;
          double *bCoeff;
          double *aCoeff;
          
          int xDataSize = 2;
          xData = new double[xDataSize];
          xData[0] = 0.0; // x(-2)
          xData[1] = 0.0; // x(-1)

          int yDataSize = 2;
          yData = new double[yDataSize];
          yData[0] = 0.0; // y(-2)
          yData[1] = 0.0; // y(-1)

          cout << "Application's memory cleared" << endl;
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }
      else if (userInput == "exit") // "exit" input
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          cout << "LTI System Simulator terminated.\n" << endl;
          logFile.close();
          break;
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }
      else if (userInput == "cls") // "cls" input
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          system("cls");
          cout << "LTI System Simulator" << endl;
          cout << "Type \"help\" for more information" << endl; 
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }
      else // invalid first argument
      {
        cout << "Invalid input. Type \"help\" for more information."
        << endl;
      }
    }
    else // invalid first argument format (not a floating number or a string)
    {
      cout << "Please input a command." << endl;
    }
  }

  return 0;
}