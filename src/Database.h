#pragma once
#include <iostream>
#include <cstring>
#include "Table.h"

//This class represents a simple database that stores multiple tables


class Database {
private:
    Table* tables;        // dynamic array of tables
    int tableCount;       // how many tables are currently stored
    int capacity;         // max number of tables allowed

    static int dbCount;   // static field
    int meta[3];          // static vector
    const int id;         // constant field

public:
    // Default constructor
    Database() : tables(nullptr), tableCount(0), capacity(0), id(dbCount + 1)
    {
        dbCount++;
        for (int i = 0; i < 3; i++) meta[i] = 0;
    }

    //Constructor with capacity
    Database(int cap) : tables(nullptr), tableCount(0), capacity(cap), id(dbCount + 1)
    {
        dbCount++;
        for (int i = 0; i < 3; i++) meta[i] = 0;

        if (capacity > 0) {
            tables = new Table[capacity]; // default-constructed tables
        }
    }

    // Copy constructor
    Database(const Database& other)
        : tables(nullptr), tableCount(0), capacity(other.capacity), id(other.id)
    {
        dbCount++;

        for (int i = 0; i < 3; i++) meta[i] = other.meta[i];

        if (capacity > 0) {
            tables = new Table[capacity];

            // numarul maxim de elemente pe care le copiem in siguranta
            int limit = other.tableCount;
            if (limit > capacity) {
                limit = capacity;
            }

            for (int i = 0; i < limit; i++) {
                tables[i] = other.tables[i];
            }

            tableCount = limit;
        }
    }

    //Destructor
    ~Database()
    {
        delete[] tables;
        dbCount--;
    }

    // assignment operator
    Database& operator=(const Database& other)
    {
        if (this == &other) return *this;

        delete[] tables;
        tables = nullptr;

        capacity = other.capacity;

        for (int i = 0; i < 3; i++) meta[i] = other.meta[i];

        if (capacity > 0) {
            tables = new Table[capacity];

            int limit = other.tableCount;
            if (limit > capacity) {
                limit = capacity;
            }

            for (int i = 0; i < limit; i++) {
                tables[i] = other.tables[i];
            }

            tableCount = limit;
        }
        else {
            tableCount = 0;
        }

        return *this;
    }


    int getTableCount() const {
        return tableCount;
    }

    //add a table if there's space
    bool addTable(const Table& t)
    {
        if (tables == nullptr || tableCount >= capacity) return false;
        tables[tableCount] = t;
        tableCount++;
        return true;
    }

    // remove a table by name (used by DROP TABLE)
    bool removeTable(const char* name)
    {
        if (tables == nullptr || name == nullptr) return false;

        for (int i = 0; i < tableCount; i++) {
            if (tables[i].getName() != nullptr &&
                std::strcmp(tables[i].getName(), name) == 0)
            {
                // shift tables to the left
                for (int j = i + 1; j < tableCount; j++) {
                    tables[j - 1] = tables[j];
                }
                tableCount--;
                return true;
            }
        }
        return false;
    }

    Table* findTable(const char* name)
    {
        if (!name) return nullptr;
        for (int i = 0; i < tableCount; i++) {
            if (tables[i].getName() != nullptr &&
                std::strcmp(tables[i].getName(), name) == 0)
            {
                return &tables[i];
            }
        }
        return nullptr;
    }

    Table& operator[](int index) {
        if (index < 0) index = 0;
        if (index >= tableCount && tableCount > 0) index = tableCount - 1;
        return tables[index];
    }

    const Table& operator[](int index) const {
        if (index < 0) index = 0;
        if (index >= tableCount && tableCount > 0) index = tableCount - 1;
        return tables[index];
    }

    bool operator==(const Database& other) const
    {
        return tableCount == other.tableCount;
    }

    explicit operator int() const {
        return tableCount;
    }
   
    Database& operator++()
    {
        meta[0]++;
        return *this;
    }

    Database operator++(int)
    {
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


int Database::dbCount = 0;