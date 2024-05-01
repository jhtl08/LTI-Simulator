// ltimain.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// May 1, 2024

#include "ltisim.h"

using namespace std;

int main()
{
  
  bool systemFile = false;
  bool signalFile = false;

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

  int signalDuration = 0; 

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

    // for string inputs
    if (ss >> userInput)
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

      // "system [filename]" input
      else if (userInput == "system")
      {
        if (ss >> userInput) // check for second arg (filename)
        {
          if (!(ss >> userInput) && ss.eof() && !systemFile) // check file name
          {
            systemFile = systemImport(userInput, Mplus1, N, aCoeff, bCoeff);
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

      // "signal [filename]" input
      else if (userInput == "signal") // for signal inputs
      {
        if (ss >> userInput) // check for second arg (filename)
        {
          if (!(ss >> userInput) && ss.eof() && !signalFile) // check file name
          {
            int importSize = signalImport(userInput, &importedData);
            for (int i = 0; i < importSize; i++)
            {
              pushData(xData, xDataSize, importedData[i]);
              double result = computeOutput(Mplus1, N, xDataSize, 
                yDataSize, aCoeff, bCoeff, xData, yData);
              cout << importedData[i] << " \t" << result << endl;

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
          cout << "filename is missing from the input" << endl;
        }
      }

      // "clear" input
      else if (userInput == "clear")
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          // clear file
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }

      // "exit" input
      else if (userInput == "exit")
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          cout << "LTI System Simulator terminated.\n" << endl;
            break;
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more information." << endl;
        }
      }

      // "cls" input
      else if (userInput == "cls")
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

      // invalid first argument
      else
      {
        cout << "Invalid input. Type \"help\" for more information."
        << endl;
      }
    }

    // invalid first argument format (not a floating number or a string)
    else
    {
      cout << "Please input a command." << endl;
    }
  }

  return 0;
}