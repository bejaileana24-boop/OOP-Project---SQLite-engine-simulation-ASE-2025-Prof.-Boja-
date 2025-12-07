#pragma once
#include <iostream>
#include <string>
#include "Table.h"

class Database {
private:
    Table* tables;      // dynamic array of tables
    int tableCount;     // how many tables are currently stored
    int capacity;       // max number of tables allowed

    static int dbCount; // static field: how many Database objects exist
    int meta[3];        // simple stats array
    const int id;       // unique ID

public:
    Database()
        : tables(nullptr), tableCount(0), capacity(0), id(dbCount + 1)
    {
        dbCount++;
        for (int i = 0; i < 3; i++) {
            meta[i] = 0;
        }
    }

    Database(int cap)
        : tables(nullptr), tableCount(0), capacity(cap), id(dbCount + 1)
    {
        dbCount++;
        for (int i = 0; i < 3; i++) {
            meta[i] = 0;
        }

        if (capacity > 0) {
            tables = new Table[capacity];
        }
    }

    Database(const Database& other)
        : tables(nullptr), tableCount(other.tableCount),
        capacity(other.capacity), id(other.id)
    {
        dbCount++;

        for (int i = 0; i < 3; i++) {
            meta[i] = other.meta[i];
        }

        if (capacity > 0) {
            tables = new Table[capacity];
            for (int i = 0; i < tableCount; i++) {
                tables[i] = other.tables[i];
            }
        }
    }

    ~Database() {
        delete[] tables;
        dbCount--;
    }

    Database& operator=(const Database& other)
    {
        if (this == &other) return *this;

        delete[] tables;
        tables = nullptr;

        tableCount = other.tableCount;
        capacity = other.capacity;

        for (int i = 0; i < 3; i++) {
            meta[i] = other.meta[i];
        }

        if (capacity > 0) {
            tables = new Table[capacity];
            for (int i = 0; i < tableCount; i++) {
                tables[i] = other.tables[i];
            }
        }

        return *this;
    }

    bool addTable(const Table& t) {
        if (tables == nullptr || tableCount >= capacity) return false;

        tables[tableCount] = t;
        tableCount++;
        return true;
    }

    bool removeTable(const std::string& name)
    {
        if (tables == nullptr) return false;

        for (int i = 0; i < tableCount; i++) {
            if (tables[i].getName() == name) {
                // shift left
                for (int j = i + 1; j < tableCount; j++) {
                    tables[j - 1] = tables[j];
                }
                tableCount--;
                return true;
            }
        }
        return false;
    }

    Table* findTable(const std::string& name)
    {
        if (tables == nullptr) return nullptr;

        for (int i = 0; i < tableCount; i++) {
            if (tables[i].getName() == name) {
                return &tables[i];
            }
        }
        return nullptr;
    }

    const Table* findTable(const std::string& name) const
    {
        if (tables == nullptr) return nullptr;

        for (int i = 0; i < tableCount; i++) {
            if (tables[i].getName() == name) {
                return &tables[i];
            }
        }
        return nullptr;
    }

    Table& operator[](int index) {
        return tables[index];
    }

    const Table& operator[](int index) const {
        return tables[index];
    }

    int getTableCount() const {
        return tableCount;
    }

    bool operator==(const Database& other) const {
        return tableCount == other.tableCount;
    }

    explicit operator int() const {
        return tableCount;
    }

    Database& operator++() {
        meta[0]++;
        return *this;
    }

    Database operator++(int) {
        Database temp(*this);
        meta[0]++;
        return temp;
    }

    friend std::ostream& operator<<(std::ostream& out, const Database& db)
    {
        out << "Database[id=" << db.id
            << ", tables=" << db.tableCount
            << "]";
        return out;
    }
};

// static field init
int Database::dbCount = 0;
