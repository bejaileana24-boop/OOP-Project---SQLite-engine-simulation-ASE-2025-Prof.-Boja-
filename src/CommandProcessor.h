#pragma once
#include <string>
#include <iostream>
#include "ParsedCommand.h"
#include "Errors.h"

class CommandProcessor {
public:
    CommandProcessor() {}

    ParsedCommand parse(const std::string& line) const {
        ParsedCommand cmd(line);
        return cmd;
    }

    void processLine(const std::string& line) const {
        ParsedCommand cmd;
        ErrorCode err = process(line, cmd);

        std::cout << "\n==============================\n";
        std::cout << "  PARSED SQL COMMAND\n";
        std::cout << "==============================\n";

        std::cout << "Raw Input   : " << cmd.getCommand() << "\n";
        std::cout << "Command Type: " << cmd.getType() << "\n";

        if (!cmd.getTableName().empty())
            std::cout << "Table       : " << cmd.getTableName() << "\n";
        if (!cmd.getIndexName().empty())
            std::cout << "Index       : " << cmd.getIndexName() << "\n";
        if (!cmd.getColumnList().empty())
            std::cout << "Columns     : " << cmd.getColumnList() << "\n";
        if (!cmd.getValuesList().empty())
            std::cout << "Values      : " << cmd.getValuesList() << "\n";
        if (!cmd.getSetColumn().empty())
            std::cout << "SET         : " << cmd.getSetColumn()
            << " = " << cmd.getSetValue() << "\n";
        if (!cmd.getWhereColumn().empty())
            std::cout << "WHERE       : " << cmd.getWhereColumn()
            << " = " << cmd.getWhereValue() << "\n";

        std::cout << "------------------------------\n";

        if (err != ERR_OK) {
            std::cout << "Error: " << Errors::getMessage(err) << "\n";
        }
        else {
            printCommandInfo(cmd);
        }

        std::cout << "==============================\n\n";
    }


    void printCommandInfo(const ParsedCommand& cmd) const {
        switch (cmd.getType()) {
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
            std::cout << "Error: invalid or unsupported command.\n";
            break;
        }
    }
    ErrorCode process(const std::string& line, ParsedCommand& out) const {
        if (line.size() == 0) {
            out = ParsedCommand("");
            return ERR_EMPTY_COMMAND;
        }

        out = ParsedCommand(line);

        if (out.getType() == CMD_UNKNOWN) {
            return ERR_UNKNOWN_COMMAND;
        }

        int n = out.getTokenCount();

        switch (out.getType()) {
        case CMD_CREATE_TABLE:
            if (n < 3) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_DROP_TABLE:
            if (n < 3) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_DISPLAY_TABLE:
            if (n < 3) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_CREATE_INDEX:
            if (n < 5) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_DROP_INDEX:
            if (n < 3) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_INSERT:
            if (n < 4) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_DELETE:
            if (n < 3) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_SELECT:
            if (n < 4) return ERR_TOO_FEW_TOKENS;
            break;

        case CMD_UPDATE:
            if (n < 6) return ERR_TOO_FEW_TOKENS;
            break;

        default:
            break;
        }

        return ERR_OK;
    }


};
