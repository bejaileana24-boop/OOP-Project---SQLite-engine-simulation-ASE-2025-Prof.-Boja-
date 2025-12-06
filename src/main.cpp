#include <iostream>
#include <string>
#include "ParsedCommand.h"
#include "CommandProcessor.h"

using namespace std;

int main() {
    CommandProcessor processor;

    cout << "Simple SQL Parser (Part 1)\n";
    cout << "Type EXIT to quit.\n\n";

    while (true) {
        cout << ">> ";

        string line;

        getline(cin, line);

        if (!cin) {
            cout << "\nInput stream closed.\n";
            break;
        }

        if (line == "EXIT" || line == "exit") {
            cout << "Exiting...\n";
            break;
        }

        ParsedCommand cmd = processor.parse(line);

        cout << cmd << endl;

        processor.printCommandInfo(cmd);

        cout << "\n";
    }

    return 0;
}
