#include <iostream>
#include <string>
#include "ParsedCommand.h"
#include "CommandProcessor.h"

using namespace std;

int main() {
    CommandProcessor processor;

    cout << "SQLite Engine Simulation - Phase 1\n";
    cout << "Type EXIT to quit.\n\n";

    while (true) {
        cout << "sql> ";

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

        if (line.empty()) {
            continue;
        }

        processor.processLine(line);
    }

    return 0;
}
