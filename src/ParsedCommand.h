#pragma once
#include <iostream>
#include <string>

static const int MAX_TOKENS = 30;

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
    std::string original;        // full raw command
    CommandType type;            // detected type

    // tokens[0->n-1]
    std::string tokens[MAX_TOKENS];
    int tokenCount;

    // extracted info
    std::string tableName;
    std::string indexName;
    std::string columnList;
    std::string valuesList;
    std::string whereColumn;
    std::string whereValue;
    std::string setColumn;
    std::string setValue;

    std::string toUpper(const std::string& s) const {
        std::string r = s;
        for (int i = 0; i < (int)r.size(); i++) {
            char c = r[i];
            if (c >= 'a' && c <= 'z') {
                r[i] = c - 'a' + 'A';
            }
        }
        return r;
    }

    void tokenize(const std::string& line) {
        tokenCount = 0;
        std::string current;

        for (int i = 0; i <= (int)line.size(); i++) {
            char c = (i < (int)line.size() ? line[i] : ' ');

            if (c == ' ' || c == '\n') {
                if (!current.empty()) {
                    if (tokenCount < MAX_TOKENS) {
                        tokens[tokenCount] = current;
                        tokenCount++;
                    }
                    current.clear();
                }
            }
            else {
                current += c;
            }
        }
    }

    CommandType detectType() const {
        if (tokenCount == 0) return CMD_UNKNOWN;

        std::string first = toUpper(tokens[0]);
        std::string second = (tokenCount > 1 ? toUpper(tokens[1]) : "");

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

    std::string joinFrom(int fromIndex) const {
        std::string result;
        for (int i = fromIndex; i < tokenCount; i++) {
            if (!result.empty()) result += " ";
            result += tokens[i];
        }
        return result;
    }

    void clearExtracted() {
        tableName.clear();
        indexName.clear();
        columnList.clear();
        valuesList.clear();
        whereColumn.clear();
        whereValue.clear();
        setColumn.clear();
        setValue.clear();
    }

    void parseCreateTable() {
        // CREATE TABLE tableName (...columns...)
        if (tokenCount >= 3) {
            tableName = tokens[2];
        }
        if (tokenCount >= 4) {
            // everything after table name we'll consider "column definition"
            columnList = joinFrom(3);
        }
    }

    void parseDropTable() {
        // DROP TABLE tableName
        if (tokenCount >= 3) {
            tableName = tokens[2];
        }
    }

    void parseDisplayTable() {
        // DISPLAY TABLE tableName
        if (tokenCount >= 3) {
            tableName = tokens[2];
        }
    }

    void parseCreateIndex() {
        if (tokenCount >= 3) {
            indexName = tokens[2];
        }
        if (tokenCount >= 5 && toUpper(tokens[3]) == "ON") {
            tableName = tokens[4];
        }
        if (tokenCount >= 6) {
            columnList = joinFrom(5);
        }
    }

    void parseDropIndex() {
        // DROP INDEX idxName
        if (tokenCount >= 3) {
            indexName = tokens[2];
        }
    }

    void parseInsert() {
        if (tokenCount >= 3) {
            tableName = tokens[2];
        }
        if (tokenCount >= 4) {
            valuesList = joinFrom(3);
        }
    }

    void parseDelete() {
        if (tokenCount >= 3) {
            tableName = tokens[2];
        }
        if (tokenCount >= 7 && toUpper(tokens[3]) == "WHERE") {
            whereColumn = tokens[4];
            whereValue = tokens[6];
        }
    }

    void parseSelect() {
        if (tokenCount >= 2) {
            columnList = tokens[1];
        }
        if (tokenCount >= 4 && toUpper(tokens[2]) == "FROM") {
            tableName = tokens[3];
        }
        if (tokenCount >= 8 && toUpper(tokens[4]) == "WHERE") {
            whereColumn = tokens[5];
            whereValue = tokens[7];
        }
    }

    void parseUpdate() {
        if (tokenCount >= 2) {
            tableName = tokens[1];
        }
        if (tokenCount >= 6 && toUpper(tokens[2]) == "SET") {
            setColumn = tokens[3];
            setValue = tokens[5];
        }
        if (tokenCount >= 10 && toUpper(tokens[6]) == "WHERE") {
            whereColumn = tokens[7];
            whereValue = tokens[9];
        }
    }

    void analyze() {
        clearExtracted();

        switch (type) {
            case CMD_CREATE_TABLE: parseCreateTable();  break;
            case CMD_DROP_TABLE:   parseDropTable();    break;
            case CMD_DISPLAY_TABLE:parseDisplayTable(); break;
            case CMD_CREATE_INDEX: parseCreateIndex();  break;
            case CMD_DROP_INDEX:   parseDropIndex();    break;
            case CMD_INSERT:       parseInsert();       break;
            case CMD_DELETE:       parseDelete();       break;
            case CMD_SELECT:       parseSelect();       break;
            case CMD_UPDATE:       parseUpdate();       break;
            default: break;
        }
    }

public:
    ParsedCommand()
        : original(""), type(CMD_UNKNOWN), tokenCount(0) {
    }

    ParsedCommand(const std::string& line)
        : original(line), type(CMD_UNKNOWN), tokenCount(0)
    {
        tokenize(line);
        type = detectType();
        analyze();
    }

    ParsedCommand(const ParsedCommand& other) {
        original = other.original;
        type = other.type;
        tokenCount = other.tokenCount;

        for (int i = 0; i < tokenCount; i++) {
            tokens[i] = other.tokens[i];
        }

        tableName = other.tableName;
        indexName = other.indexName;
        columnList = other.columnList;
        valuesList = other.valuesList;
        whereColumn = other.whereColumn;
        whereValue = other.whereValue;
        setColumn = other.setColumn;
        setValue = other.setValue;
    }

    ParsedCommand& operator=(const ParsedCommand& other) {
        if (this != &other) {
            original = other.original;
            type = other.type;
            tokenCount = other.tokenCount;

            for (int i = 0; i < tokenCount; i++) {
                tokens[i] = other.tokens[i];
            }

            tableName = other.tableName;
            indexName = other.indexName;
            columnList = other.columnList;
            valuesList = other.valuesList;
            whereColumn = other.whereColumn;
            whereValue = other.whereValue;
            setColumn = other.setColumn;
            setValue = other.setValue;
        }
        return *this;
    }

    ~ParsedCommand() {}

    // getters
    CommandType   getType()       const { return type; }
    std::string   getCommand()    const { return original; }
    std::string   getTableName()  const { return tableName; }
    std::string   getIndexName()  const { return indexName; }
    std::string   getColumnList() const { return columnList; }
    std::string   getValuesList() const { return valuesList; }
    std::string   getWhereColumn()const { return whereColumn; }
    std::string   getWhereValue() const { return whereValue; }
    std::string   getSetColumn()  const { return setColumn; }
    std::string   getSetValue()   const { return setValue; }
    int           getTokenCount() const { return tokenCount; }

    // operators
    bool operator==(const ParsedCommand& other) const {
        return original == other.original && type == other.type;
    }

    bool operator!() const {
        return type == CMD_UNKNOWN;
    }

    friend std::ostream& operator<<(std::ostream& out, const ParsedCommand& c) {
        out << "Command: \"" << c.original << "\"\n";
        out << "Type: " << c.type << "\n";

        if (!c.tableName.empty())
            out << "Table: " << c.tableName << "\n";
        if (!c.indexName.empty())
            out << "Index: " << c.indexName << "\n";
        if (!c.columnList.empty())
            out << "Columns: " << c.columnList << "\n";
        if (!c.valuesList.empty())
            out << "Values: " << c.valuesList << "\n";
        if (!c.setColumn.empty())
            out << "SET " << c.setColumn << " = " << c.setValue << "\n";
        if (!c.whereColumn.empty())
            out << "WHERE " << c.whereColumn << " = " << c.whereValue << "\n";

        return out;
    }
};
