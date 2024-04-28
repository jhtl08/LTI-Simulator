// ltimain.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 21, 2024

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

  int signalDuration = 0; 

  cout << "LTISim" << endl;
  cout << "Type \"help\" for more information" << endl; 

  // For Checking Inputs
  string line;
  string userInput;
  double numInput;

  // Set Initial Conditions
  setInitialConditions(xData, yData);

  while(true)
  {
    cout << "\nltisim> ";
    getline(cin, line);
    stringstream ss(line);

    // for float inputs
    if (ss >> numInput) // for floating point number inputs
    {
      if (systemFile && (signalDuration <= 3)) // check if LTI system is ready
      {
        // print output
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
          cout << getInstructions() << endl; // print instructions
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
            systemFile = SystemImport(userInput, Mplus1, N, aCoeff, bCoeff);
            systemDetails(Mplus1, N, aCoeff, bCoeff);
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