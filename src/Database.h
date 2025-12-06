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
        : tables(nullptr), tableCount(other.tableCount), capacity(other.capacity), id(other.id)
    {
        dbCount++;

        for (int i = 0; i < 3; i++) meta[i] = other.meta[i];

        if (capacity > 0) {
            tables = new Table[capacity];
            for (int i = 0; i < tableCount; i++) {
                tables[i] = other.tables[i];  // uses Table operator=
            }
        }
    }

    //Destructor
    ~Database()
    {
        delete[] tables;
        dbCount--;
    }

    //assignment operator
    Database& operator=(const Database& other)
    {
        if (this == &other) return *this;

        delete[] tables;
        tables = nullptr;

        tableCount = other.tableCount;
        capacity = other.capacity;

        for (int i = 0; i < 3; i++) meta[i] = other.meta[i];

        if (capacity > 0) {
            tables = new Table[capacity];
            for (int i = 0; i < tableCount; i++) {
                tables[i] = other.tables[i];
            }
        }

        return *this;
    }

    //add a table if there's space
    bool addTable(const Table& t)
    {
        if (tables == nullptr || tableCount >= capacity) return false;
        tables[tableCount] = t;
        tableCount++;
        return true;
    }

    //find a table by name
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

    //Access operator
    Table& operator[](int index)
    {
        return tables[index];
    }

    int getTableCount() const {
        return tableCount;
    }

    //operator ==
    bool operator==(const Database& other) const
    {
        return tableCount == other.tableCount;
    }

    //cast operator 
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

    //output operator
    friend std::ostream& operator<<(std::ostream& out, const Database& db)
    {
        out << "Database[id=" << db.id
            << ", tables=" << db.tableCount
            << "]";
        return out;
    }
};


int Database::dbCount = 0;