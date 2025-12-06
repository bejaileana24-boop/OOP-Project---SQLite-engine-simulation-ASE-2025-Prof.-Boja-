#pragma once
#include <iostream>
#include <cstring>
#include "Column.h"

class Table {
private:
    char* name;
    Column* columns;
    int columnCount;
    int capacity;

    static int tableCount;
    int usage[3];
    const int id;

public:
    // constructor default
    Table() : name(nullptr), columns(nullptr), columnCount(0), capacity(0), id(tableCount + 1)
    {
        tableCount++;
        for (int i = 0; i < 3; i++) usage[i] = 0;
    }


    Table(const char* n, int cap) : name(nullptr), columns(nullptr),
        columnCount(0), capacity(cap), id(tableCount + 1)
    {
        tableCount++;
        for (int i = 0; i < 3; i++) usage[i] = 0;

        if (n != nullptr && std::strlen(n) > 0) {
            name = new char[std::strlen(n) + 1];
            std::strcpy(name, n);
        }

        if (capacity > 0) {
            columns = new Column[capacity];
        }
    }

    // copy constructor
    Table(const Table& other) : name(nullptr), columns(nullptr),
        columnCount(other.columnCount), capacity(other.capacity), id(other.id)
    {
        tableCount++;
        for (int i = 0; i < 3; i++) usage[i] = other.usage[i];

        if (other.name != nullptr) {
            name = new char[std::strlen(other.name) + 1];
            std::strcpy(name, other.name);
        }

        if (capacity > 0) {
            columns = new Column[capacity];
            for (int i = 0; i < columnCount; i++) {
                columns[i] = other.columns[i];
            }
        }
    }

    // destructor
    ~Table()
    {
        delete[] name;
        delete[] columns;
        tableCount--;
    }

    // operator =
    Table& operator=(const Table& other)
    {
        if (this == &other) return *this;

        delete[] name;
        delete[] columns;

        name = nullptr;
        columns = nullptr;
        columnCount = other.columnCount;
        capacity = other.capacity;

        for (int i = 0; i < 3; i++) usage[i] = other.usage[i];

        if (other.name != nullptr) {
            name = new char[std::strlen(other.name) + 1];
            std::strcpy(name, other.name);
        }

        if (capacity > 0) {
            columns = new Column[capacity];
            for (int i = 0; i < columnCount; i++) {
                columns[i] = other.columns[i];
            }
        }

        // id is const
        return *this;
    }

    const char* getName() const {
        return name;
    }

    void setName(const char* n) {
        if (n == nullptr || std::strlen(n) == 0) return;
        delete[] name;
        name = new char[std::strlen(n) + 1];
        std::strcpy(name, n);
    }

    int getColumnCount() const {
        return columnCount;
    }


    void addColumn(const Column& c) {
        if (columns == nullptr || columnCount >= capacity) return;
        columns[columnCount] = c;
        columnCount++;
    }

    // access at column i
    Column& operator[](int index) {

        return columns[index];
    }

    // operator <<
    friend std::ostream& operator<<(std::ostream& out, const Table& t)
    {
        out << "Table[ id=" << t.id
            << ", name=" << (t.name ? t.name : "NULL")
            << ", columns=" << t.columnCount
            << " ]";
        return out;
    }

    // operator ==
    bool operator==(const Table& other) const {
        if (name == nullptr || other.name == nullptr) return false;
        return std::strcmp(name, other.name) == 0 && columnCount == other.columnCount;
    }


    explicit operator int() const {
        return columnCount;
    }

    // ++ 
    Table& operator++() {
        usage[0]++;
        return *this;
    }


    Table operator++(int) {
        Table temp(*this);
        usage[0]++;
        return temp;
    }
};

int Table::tableCount = 0;