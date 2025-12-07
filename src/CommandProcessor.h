#pragma once
#include <string>
#include <iostream>
#include "ParsedCommand.h"
#include "Errors.h"
#include "Database.h"
#include "Column.h"
#include "Table.h"

class CommandProcessor {
private:
    Database* db;

    std::string trim(const std::string& s) const {
        int n = (int)s.size();
        int i = 0;
        int j = n - 1;

        while (i <= j && (s[i] == ' ' || s[i] == '\t')) i++;
        while (j >= i && (s[j] == ' ' || s[j] == '\t')) j--;

        if (i > j) return "";
        return s.substr(i, j - i + 1);
    }

    ColumnType parseColumnType(const std::string& s) const {
        std::string r;
        for (int i = 0; i < (int)s.size(); i++) {
            char c = s[i];
            if (c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }
            if (c != ' ' && c != '\t') {
                r += c;
            }
        }

        if (r == "INT" || r == "INTEGER") return COL_INTEGER;
        if (r == "FLOAT" || r == "REAL")   return COL_FLOAT;
        if (r == "TEXT" || r == "STRING")  return COL_TEXT;

        return COL_UNKNOWN;
    }

    bool parseInt(const std::string& s, int& out) const {
        if (s.empty()) return false;
        int sign = 1;
        int i = 0;
        if (s[0] == '-') {
            sign = -1;
            i = 1;
        }
        int value = 0;
        for (; i < (int)s.size(); i++) {
            char c = s[i];
            if (c < '0' || c > '9') {
                return false;
            }
            value = value * 10 + (c - '0');
        }
        out = sign * value;
        return true;
    }

    void parseColumnsIntoTable(const std::string& colsText, Table& table) const {
        int n = (int)colsText.size();
        int pos = 0;

        while (pos < n) {
            // find '('
            while (pos < n && colsText[pos] != '(') {
                pos++;
            }
            if (pos >= n) {
                break;
            }
            int start = pos + 1;
            int end = start;
            int depth = 1;

            while (end < n && depth > 0) {
                if (colsText[end] == '(') depth++;
                else if (colsText[end] == ')') depth--;
                end++;
            }

            if (depth != 0) {
                break;
            }

            int sliceEnd = end - 1;
            if (sliceEnd <= start) {
                pos = end;
                continue;
            }

            std::string inside = colsText.substr(start, sliceEnd - start);
            std::string parts[4];
            int partCount = 0;
            std::string current;

            for (int i = 0; i < (int)inside.size(); i++) {
                char c = inside[i];
                if (c == ',' && partCount < 4) {
                    parts[partCount] = trim(current);
                    partCount++;
                    current.clear();
                }
                else {
                    current += c;
                }
            }
            if (!current.empty() && partCount < 4) {
                parts[partCount] = trim(current);
                partCount++;
            }

            if (partCount >= 2) {
                std::string nameStr = trim(parts[0]);
                std::string typeStr = trim(parts[1]);
                int size = 0;
                if (partCount >= 3) {
                    parseInt(trim(parts[2]), size);
                }
                std::string defStr = "";
                if (partCount >= 4) {
                    defStr = trim(parts[3]);
                }

                ColumnType ctype = parseColumnType(typeStr);

                if (!nameStr.empty() && ctype != COL_UNKNOWN && size > 0) {
                    Column col(nameStr, ctype, size, defStr);
                    table.addColumn(col);
                }
            }

            pos = end;
        }
    }

public:
    CommandProcessor(Database* database = nullptr)
        : db(database) {
    }

    void setDatabase(Database* database) {
        db = database;
    }

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
            std::cout << "WHERE       : "
            << cmd.getWhereColumn()
            << " = " << cmd.getWhereValue() << "\n";

        std::cout << "------------------------------\n";

        if (err != ERR_OK) {
            std::cout << "Error: " << Errors::getMessage(err) << "\n";
        }
        else {
            printCommandInfo(cmd);
            execute(cmd);
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

        // 1) Very basic validation: nr. minim de token-uri
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

        switch (out.getType()) {
        case CMD_CREATE_TABLE:
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            if (out.getColumnList().empty()) return ERR_INVALID_FORMAT;
            break;

        case CMD_DROP_TABLE:
        case CMD_DISPLAY_TABLE:
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            break;

        case CMD_CREATE_INDEX:
            // CREATE INDEX nume ON tabel (coloana)
            if (out.getIndexName().empty()) return ERR_INVALID_FORMAT;
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            if (out.getColumnList().empty()) return ERR_INVALID_FORMAT;
            break;

        case CMD_DROP_INDEX:
            // DROP INDEX nume
            if (out.getIndexName().empty()) return ERR_INVALID_FORMAT;
            break;

        case CMD_INSERT:
            // INSERT INTO table VALUES(...)
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            if (out.getValuesList().empty()) return ERR_INVALID_FORMAT;
            break;

        case CMD_DELETE:
            // DELETE FROM table WHERE col = val
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            // conform cerinței, DELETE are obligatoriu WHERE col = val
            if (out.getWhereColumn().empty() || out.getWhereValue().empty())
                return ERR_INVALID_FORMAT;
            break;

        case CMD_SELECT:
            // SELECT col | ALL FROM table [WHERE col = val]
            if (out.getColumnList().empty()) return ERR_INVALID_FORMAT;
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            // dacă apare WHERE, vrem ambele părți (col și val)
            if ((!out.getWhereColumn().empty() && out.getWhereValue().empty()) ||
                (out.getWhereColumn().empty() && !out.getWhereValue().empty()))
                return ERR_INVALID_FORMAT;
            break;

        case CMD_UPDATE:
            // UPDATE table SET col = val WHERE col = val
            if (out.getTableName().empty()) return ERR_INVALID_FORMAT;
            // SET col = val e obligatoriu
            if (out.getSetColumn().empty() || out.getSetValue().empty())
                return ERR_INVALID_FORMAT;
            // Dacă ai început WHERE, vrei ambele
            if ((!out.getWhereColumn().empty() && out.getWhereValue().empty()) ||
                (out.getWhereColumn().empty() && !out.getWhereValue().empty()))
                return ERR_INVALID_FORMAT;
            break;

        default:
            break;
        }

        return ERR_OK;
    }

    void execute(const ParsedCommand& cmd) const {
        if (db == nullptr) {
            std::cout << "[EXEC] No database attached. Command not applied.\n";
            return;
        }

        switch (cmd.getType()) {
        case CMD_CREATE_TABLE:
            execCreateTable(cmd);
            break;
        case CMD_DROP_TABLE:
            execDropTable(cmd);
            break;
        case CMD_DISPLAY_TABLE:
            execDisplayTable(cmd);
            break;
        case CMD_CREATE_INDEX:
            std::cout << "[EXEC] (Phase 1) CREATE INDEX '"
                << cmd.getIndexName()
                << "' ON table '" << cmd.getTableName() << "'.\n";
            break;
        case CMD_DROP_INDEX:
            std::cout << "[EXEC] (Phase 1) DROP INDEX '"
                << cmd.getIndexName() << "'.\n";
            break;
        default:
            // other commands are only parsed and printed for now
            break;
        }
    }

    void execCreateTable(const ParsedCommand& cmd) const {
        std::string tname = cmd.getTableName();
        if (tname.empty()) {
            std::cout << "[EXEC] CREATE TABLE: missing table name.\n";
            return;
        }

        if (db->findTable(tname) != nullptr) {
            std::cout << "[EXEC] CREATE TABLE: table '" << tname
                << "' already exists.\n";
            return;
        }

        Table t(tname.c_str(), 20);

        std::string cols = cmd.getColumnList();
        if (!cols.empty()) {
            parseColumnsIntoTable(cols, t);
        }

        db->addTable(t);
        std::cout << "[EXEC] Table '" << tname << "' created with "
            << t.getColumnCount() << " column(s).\n";
    }

    void execDropTable(const ParsedCommand& cmd) const {
        std::string tname = cmd.getTableName();
        if (tname.empty()) {
            std::cout << "[EXEC] DROP TABLE: missing table name.\n";
            return;
        }

        if (db->findTable(tname) == nullptr) {
            std::cout << "[EXEC] DROP TABLE: table '" << tname
                << "' does not exist.\n";
            return;
        }

        bool removed = db->removeTable(tname);
        if (removed) {
            std::cout << "[EXEC] Table '" << tname << "' dropped.\n";
        }
        else {
            std::cout << "[EXEC] DROP TABLE: internal error while removing table.\n";
        }
    }

    void execDisplayTable(const ParsedCommand& cmd) const {
        std::string tname = cmd.getTableName();
        if (tname.empty()) {
            std::cout << "[EXEC] DISPLAY TABLE: missing table name.\n";
            return;
        }

        Table* t = db->findTable(tname);
        if (t == nullptr) {
            std::cout << "[EXEC] DISPLAY TABLE: table '" << tname
                << "' not found.\n";
            return;
        }

        std::cout << "=== TABLE: " << t->getName() << " ===\n";
        std::cout << "Columns: " << t->getColumnCount() << "\n";

        for (int i = 0; i < t->getColumnCount(); i++) {
            std::cout << "  - " << (*t)[i] << "\n";
        }
    }
};
