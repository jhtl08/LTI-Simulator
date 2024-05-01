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

  bool systemFile = false;

  // System Variables
  int Mplus1 = 0, N = 0;
  double *bCoeff;
  double *aCoeff;

  // Signal Variables
  double *importedData;

  // Initial Conditions
  int xDataSize = 2;
  double *xData = new double[xDataSize];
  xData[0] = 0.0; // x(-2)
  xData[1] = 0.0; // x(-1)

  int yDataSize = 2;
  double *yData = new double[yDataSize];
  yData[0] = 0.0; // y(-2)
  yData[1] = 0.0; // y(-1)

  cout << "LTISim" << endl;
  cout << "Type \"help\" for more information" << endl; 
  if (logFile.is_open())
  {
    logFile << "LTISim booted\n" << endl;
  }

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
          if(logFile.is_open())
          {
            logFile << numInput << " \t" << result << endl;
          }
          pushData(yData, yDataSize, result);
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more"
          " information." << endl;
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
          cout << instructions << endl;
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more"
          " information." << endl;
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
                if (logFile.is_open())
                {
                  logFile << "new system" << endl;
                  logFile << Mplus1 << endl;
                  logFile << N << endl;
                  for (int i = 0; i < Mplus1; i++)
                  {
                    logFile << bCoeff[i] << endl;
                  }
                  for (int i = 0; i < N; i++)
                  {
                    logFile << aCoeff[i] << endl;
                  }
                  logFile << "ready" << endl;
                }
              }
              else
              {
                cout << "Previous LTI system still active" << endl;
              }
            }
            else
            {
              bool importSuccess = systemImport(userInput, Mplus1, N, 
              aCoeff, bCoeff);
              systemFile = importSuccess;
              if (logFile.is_open())
              {
                if (systemFile)
                {
                  logFile << "new system" << endl;
                  logFile << Mplus1 << endl;
                  logFile << N << endl;
                  for (int i = 0; i < Mplus1; i++)
                  {
                    logFile << bCoeff[i] << endl;
                  }
                  for (int i = 0; i < N; i++)
                  {
                    logFile << aCoeff[i] << endl;
                  }
                  logFile << "ready" << endl;
                }
                else
                {
                  logFile << "Invalid System File" << endl;
                }
              }
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
              int importSize = signalImport(userInput, 
              &importedData);
              for (int i = 0; i < importSize; i++)
              {
                pushData(xData, xDataSize, importedData[i]);
                double result = computeOutput(Mplus1, N, xDataSize, 
                  yDataSize, aCoeff, bCoeff, xData, yData);
                if (importSize < 10)
                {
                  cout << importedData[i] << " \t" << result << endl;
                }
                if (logFile.is_open())
                {
                  logFile << importedData[i] << " \t" << result 
                  << endl;
                }
                pushData(yData, yDataSize, result);
              }
              if (importSize >= 10)
              {
                cout << importSize << " inputs were simulated." 
                << endl;
                cout << yData[yDataSize-1] << 
                " is the latest output." << endl;
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

          for(int i = 0; i < xDataSize; i++)
          {
            xData[i] = 0;
          }
          for(int i = 0; i < yDataSize; i++)
          {
            yData[i] = 0;
          }

          Mplus1 = 0;
          N = 0;
          
          int xDataSize = 2;
          xData[0] = 0.0; // x(-2)
          xData[1] = 0.0; // x(-1)

          int yDataSize = 2;
          yData[0] = 0.0; // y(-2)
          yData[1] = 0.0; // y(-1)

          cout << "Application's memory cleared" << endl;
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more"
          " information." << endl;
        }
      }
      else if (userInput == "exit") // "exit" input
      {
        if (!(ss >> userInput) && ss.eof()) // no second argument
        {
          cout << "LTI System Simulator terminated\n" << endl;
          if (logFile.is_open())
          {
            logFile << "\nLTISim terminated\n" 
            << endl;
          }
          logFile.close();
          break;
        }
        else // second argument detected
        {
          cout << "Invalid input. Type \"help\" for more"
          " information." << endl;
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
          cout << "Invalid input. Type \"help\" for more"
          " information." << endl;
        }
      }
      else // invalid first argument
      {
        cout << "Invalid input. Type \"help\" for more"
        " information." << endl;
      }
    }
    else // invalid first argument format (not float or string)
    {
      cout << "Please input a command." << endl;
    }
  }

  return 0;
}