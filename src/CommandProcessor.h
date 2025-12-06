#pragma once
#include <string>
#include <iostream>
#include "ParsedCommand.h"

class CommandProcessor {
public:
    CommandProcessor() {}

    ParsedCommand parse(const std::string& line) const {
        ParsedCommand cmd(line);
        return cmd;
    }

    // Placeholder: Afiseaza comanda recunoscuta
    void printCommandInfo(const ParsedCommand& cmd) const {
        CommandType t = cmd.getType();

        switch (t) {
        case CMD_CREATE_TABLE:
            std::cout << "Recognized: CREATE TABLE\n";
            break;
        case CMD_DROP_TABLE:
            std::cout << "Recognized: DROP TABLE\n";
            break;
        case CMD_DISPLAY_TABLE:
            std::cout << "Recognized: DISPLAY TABLE\n";
            break;
        case CMD_CREATE_INDEX:
            std::cout << "Recognized: CREATE INDEX\n";
            break;
        case CMD_DROP_INDEX:
            std::cout << "Recognized: DROP INDEX\n";
            break;
        case CMD_INSERT:
            std::cout << "Recognized: INSERT\n";
            break;
        case CMD_SELECT:
            std::cout << "Recognized: SELECT\n";
            break;
        case CMD_UPDATE:
            std::cout << "Recognized: UPDATE\n";
            break;
        case CMD_DELETE:
            std::cout << "Recognized: DELETE\n";
            break;
        default:
            std::cout << "Unknown command type.\n";
            break;
        }
    }
};
