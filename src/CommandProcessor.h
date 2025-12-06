#pragma once
#include <string>
#include <iostream>
#include "ParsedCommand.h"
#include "Errors.h"
#include "Database.h"

class CommandProcessor {
private:
    Database* db;   // in-memory database (single instance)

public:
    // default: fara DB atasata
    CommandProcessor() : db(nullptr) {}

    // constructor cu DB
    CommandProcessor(Database* database) : db(database) {}

    void setDatabase(Database* database) {
        db = database;
    }

    // doar parse simplu
    ParsedCommand parse(const std::string& line) const {
        ParsedCommand cmd(line);
        return cmd;
    }

    // parse + validare de baza + cod de eroare
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
            if (n < 3) return ERR_TOO_FEW_TOKENS;   // CREATE TABLE nume
            break;
        case CMD_DROP_TABLE:
            if (n < 3) return ERR_TOO_FEW_TOKENS;   // DROP TABLE nume
            break;
        case CMD_DISPLAY_TABLE:
            if (n < 3) return ERR_TOO_FEW_TOKENS;   // DISPLAY TABLE nume
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

    // executa efectiv comanda asupra bazei de date
    void execute(const ParsedCommand& cmd) const {
        if (db == nullptr) {
            std::cout << "[EXEC] No Database attached. Skipping execution.\n";
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
        default:
            // pentru celelalte comenzi, deocamdata doar parsare + afisare
            break;
        }
    }

    // afisare + erori + executie
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
            execute(cmd);   // modificam Database-ul
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

private:

    void execCreateTable(const ParsedCommand& cmd) const {
        std::string name = cmd.getTableName();
        if (name.empty()) {
            std::cout << "[EXEC] CREATE TABLE: missing table name.\n";
            return;
        }

        Table* existing = db->findTable(name.c_str());
        if (existing != nullptr) {
            std::cout << "[EXEC] Table '" << name << "' already exists.\n";
            return;
        }

        // deocamdata 20 coloane max
        Table t(name.c_str(), 20);

        if (!db->addTable(t)) {
            std::cout << "[EXEC] Database is full, cannot add table '" << name << "'.\n";
        }
        else {
            std::cout << "[EXEC] Table '" << name << "' created in memory.\n";
        }
    }

    void execDropTable(const ParsedCommand& cmd) const {
        std::string name = cmd.getTableName();
        if (name.empty()) {
            std::cout << "[EXEC] DROP TABLE: missing table name.\n";
            return;
        }

        if (db->removeTable(name.c_str())) {
            std::cout << "[EXEC] Table '" << name << "' dropped.\n";
        }
        else {
            std::cout << "[EXEC] Table '" << name << "' not found.\n";
        }
    }

    void execDisplayTable(const ParsedCommand& cmd) const {
        std::string name = cmd.getTableName();

        if (name.empty()) {
            // daca nu primeste nume, afiseaza toate tabelele
            int count = db->getTableCount();
            std::cout << "[EXEC] DISPLAY TABLE: all tables (" << count << "):\n";
            for (int i = 0; i < count; i++) {
                std::cout << "  " << (*db)[i] << "\n";
            }
        }
        else {
            Table* t = db->findTable(name.c_str());
            if (t != nullptr) {
                std::cout << "[EXEC] DISPLAY TABLE '" << name << "':\n";
                std::cout << "  " << *t << "\n";
            }
            else {
                std::cout << "[EXEC] Table '" << name << "' not found.\n";
            }
        }
    }
};
