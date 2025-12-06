#pragma once
#include <iostream>
#include <string>
#include "Column.h"

class Table {
private:
    std::string name;
    Column* columns;     // dynamic array of columns
    int columnCount;
    int capacity;

    static int tableCount;
    int usage[3];
    const int id;

public:
    Table()
        : name(""), columns(nullptr), columnCount(0), capacity(0), id(tableCount + 1)
    {
        tableCount++;
        for (int i = 0; i < 3; i++) usage[i] = 0;
    }

    Table(const char* n, int cap)
        : name(""), columns(nullptr), columnCount(0), capacity(cap), id(tableCount + 1)
    {
        tableCount++;
        for (int i = 0; i < 3; i++) usage[i] = 0;

        if (n != nullptr && std::strlen(n) > 0) {
            name = n;
        }

        if (capacity > 0) {
            columns = new Column[capacity];
        }
    }

    Table(const Table& other)
        : name(other.name),
        columns(nullptr),
        columnCount(other.columnCount),
        capacity(other.capacity),
        id(other.id)
    {
        tableCount++;
        for (int i = 0; i < 3; i++) usage[i] = other.usage[i];

        if (capacity > 0) {
            columns = new Column[capacity];
            for (int i = 0; i < columnCount; i++) {
                columns[i] = other.columns[i];
            }
        }
    }

    ~Table()
    {
        delete[] columns;
        tableCount--;
    }

    Table& operator=(const Table& other)
    {
        if (this == &other) return *this;

        delete[] columns;
        columns = nullptr;
        columnCount = other.columnCount;
        capacity = other.capacity;
        name = other.name;

        for (int i = 0; i < 3; i++) usage[i] = other.usage[i];

        if (capacity > 0) {
            columns = new Column[capacity];
            for (int i = 0; i < columnCount; i++) {
                columns[i] = other.columns[i];
            }
        }

        return *this;
    }

    const char* getName() const {
        return name.c_str();
    }

    void setName(const char* n) {
        if (n == nullptr || std::strlen(n) == 0) return;
        name = n;
    }

    int getColumnCount() const {
        return columnCount;
    }

    int getCapacity() const {
        return capacity;
    }

    // add a column if there is space
    void addColumn(const Column& c) {
        if (columns == nullptr || columnCount >= capacity) return;
        columns[columnCount] = c;
        columnCount++;
    }

    // access column i
    Column& operator[](int index) {
        // simple bounds check
        if (index < 0) index = 0;
        if (index >= columnCount && columnCount > 0) index = columnCount - 1;
        return columns[index];
    }

    const Column& operator[](int index) const {
        if (index < 0) index = 0;
        if (index >= columnCount && columnCount > 0) index = columnCount - 1;
        return columns[index];
    }


    friend std::ostream& operator<<(std::ostream& out, const Table& t)
    {
        out << "Table[ id=" << t.id
            << ", name=" << (t.name.empty() ? "NULL" : t.name.c_str())
            << ", columns=" << t.columnCount
            << " ]";
        return out;
    }

    bool operator==(const Table& other) const {
        return name == other.name && columnCount == other.columnCount;
    }

    explicit operator int() const {
        return columnCount;
    }

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

// static field initialization
int Table::tableCount = 0;
