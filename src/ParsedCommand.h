#pragma once
#include <iostream>
#include <string>
#include <vector>

enum CommandType {
    CMD_UNKNOWN,
    CMD_CREATE_TABLE,
    CMD_DROP_TABLE,
    CMD_DISPLAY_TABLE,
    CMD_CREATE_INDEX,
    CMD_DROP_INDEX,
    CMD_INSERT,
    CMD_SELECT,
    CMD_UPDATE,
    CMD_DELETE
};

class ParsedCommand {
private:
    std::string command;
    CommandType type;

    // Tokenizer
    std::vector<std::string> tokenize(const std::string& line) const {
        std::vector<std::string> tokens;
        std::string current;

        for (int i = 0; i <= (int)line.size(); i++) {
            char c;

            if (i < (int)line.size()) {
                c = line[i];
            }
            else {
                c = ' '; // add last word
            }

            if (c == ' ' || c == '\n') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            }
            else {
                current += c;
            }
        }

        return tokens;
    }

    // Uppercase helper
    std::string toUpper(const std::string& s) const {
        std::string r = s;

        for (int i = 0; i < (int)r.size(); i++) {
            char c = r[i];

            if (c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }

            r[i] = c;
        }

        return r;
    }

    // Command type detector
    CommandType determineCommandType(const std::string& cmd) {
        std::vector<std::string> tokens = tokenize(cmd);
        if (tokens.empty()) return CMD_UNKNOWN;

        std::string first = toUpper(tokens[0]);
        std::string second = (tokens.size() > 1 ? toUpper(tokens[1]) : "");

        if (first == "CREATE") {
            if (second == "TABLE") return CMD_CREATE_TABLE;
            if (second == "INDEX") return CMD_CREATE_INDEX;
        }

        if (first == "DROP") {
            if (second == "TABLE") return CMD_DROP_TABLE;
            if (second == "INDEX") return CMD_DROP_INDEX;
        }

        if (first == "DISPLAY") return CMD_DISPLAY_TABLE;
        if (first == "INSERT")  return CMD_INSERT;
        if (first == "DELETE")  return CMD_DELETE;
        if (first == "SELECT")  return CMD_SELECT;
        if (first == "UPDATE")  return CMD_UPDATE;

        return CMD_UNKNOWN;
    }

public:

    // Constructors
    ParsedCommand() : command(""), type(CMD_UNKNOWN) {}

    ParsedCommand(const std::string& cmd) : command(cmd) {
        type = determineCommandType(cmd);
    }

    ParsedCommand(const ParsedCommand& other) {
        command = other.command;
        type = other.type;
    }

    ParsedCommand& operator=(const ParsedCommand& other) {
        if (this != &other) {
            command = other.command;
            type = other.type;
        }
        return *this;
    }

    ~ParsedCommand() {}

    CommandType getType() const { return type; }
    std::string getCommandString() const { return command; }

    bool operator==(const ParsedCommand& other) const {
        return command == other.command && type == other.type;
    }

    bool operator!() const { return type == CMD_UNKNOWN; }

    friend std::ostream& operator<<(std::ostream& out, const ParsedCommand& c) {
        out << "ParsedCommand: \"" << c.command
            << "\" (type=" << c.type << ")";
        return out;
    }
};
