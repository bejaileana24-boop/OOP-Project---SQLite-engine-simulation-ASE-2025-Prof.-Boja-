#include <iostream>
#include <string>
#include "ParsedCommand.h"
#include "CommandProcessor.h"
#include "Database.h"

using namespace std;

int main() {
    Database db(10);
    CommandProcessor processor(&db);

    cout << "SQLite Engine Simulation - Phase 1\n";
    cout << "Type EXIT to quit.\n\n";

    while (true) {
        cout << "sql> ";

        string line;
        if (!std::getline(cin, line)) {
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
