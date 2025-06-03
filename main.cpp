#include <iostream>
#include <string>
#include <sstream>
#include "parser.h"

using namespace std;

const string VERSION = "0.0.2";

string readMultilineInput()
{
    string input, line;
    cout << "Enter Python code (Ctrl+Z on Windows to finish):\n";

    while (getline(cin, line))
    {
        input += line + "\n";
    }

    cin.clear(); // Clear EOF flag
    return input;
}

int main()
{
    cout << "Python Parser Version " << VERSION << endl;
    
    while (true)
    {
        string input = readMultilineInput();

        if (input.empty())
        {
            break;
        }

        Parser parser(input);
        parser.parse();

        if (parser.hasError())
        {
            cout << "\nError: " << parser.getErrorMessage() << endl;
        }
        else
        {
            cout << "\nNo syntax errors found!" << endl;
            // Symbol and token tables are automatically printed by the parser
        }

        cout << "\nCtrl+Z (Windows) twice to exit, or continue entering code.\n" << endl;
    }

    return 0;
}