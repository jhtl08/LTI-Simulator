// ltimain.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 21, 2024

#include "ltisim.h"

using namespace std;

int main()
{

  string line, userInput;
  double numInput;
  bool ltisystemLoaded = 0;
  double *xData;
  int signalDuration;
  ltiSystem yLTI;

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

  cout << "LTI System Simulator" << endl;
  cout << "Type \"help\" for more information" << endl; 

  while(true)
  {
    cout << "\nltisim> ";
    getline(cin, line);
    stringstream ss(line);
    if (ss >> numInput) // for floating point number inputs
    {
      if (ltisystemLoaded) // check if LTI system is ready
      {
        cout << "System function for " << numInput << " runs here." <<endl;
      }
      else
      {
        cout << "Cannot simulate. No system defined" << endl;
      }
      continue; // if input is valid float, skip other input types
    }
    ss.clear(); // clears ss error flags if input is nonfloat
    if (ss >> userInput) // for string inputs
    {
      ss.clear();
      if (userInput == "help")
      {
        cout << instructions << endl;
      }
      else if (userInput == "system") // for system inputs
      {
        if (ss >> userInput) // check for second arg (filename)
        {
          if (yLTI.ltiSystemImport(userInput))
          {
            if (ltisystemLoaded) // check if another LTI system is ready
            {
              cout << "Previous system file overwritten." << endl;
            }
            else
            {
              ltisystemLoaded = true;
            }
          }
          else
          {
            cout << "No system obtained." << endl;
          }          
        }
        else
        {
          cout << "filename is missing from the input" << endl;
        }
      }
      else if (userInput == "signal") // for signal inputs
      {
        if (ss >> userInput) // check for second arg (filename)
        {
          if (ltisystemLoaded) // check if LTI system is ready
          {
            signalDuration = SignalImport(userInput, &xData);
            if (signalDuration == 0)
            {
              cout << "No outputs computed." << endl;
            }
            else
            {
              cout << "System function for xData runs here." <<endl;
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
      else if (userInput == "cls")
      {
        system("cls");
        cout << "LTI System Simulator" << endl;
        cout << "Type \"help\" for more information" << endl; 
      }
      else if (userInput == "exit")
      {
        cout << "LTI System Simulator terminated.\n" << endl;
        break;
      }
      else if (userInput == "clear")
      {
        yLTI = ltiSystem();
        ltisystemLoaded = false;
        delete[] xData;
      }
      else
      {
        cout << "Invalid input. Type \"help\" for more information."
        << endl;
      }
    }
    else
    {
      cout << "Please input a command." << endl;
    }
  }

  return 0;
}