#pragma once
#include <iostream>
#include <cstring>



// This class stores basic information about a database index
// It keeps the index name, table name, column name and simple metadata

class Index {
private:
    char* indexName;   // index name
    char* tableName;   // table name
    char* columnName;  // column name

    static int indexCount;  // static counter
    int meta[3];            // static vector
    const int id;           // constant field

public:
    // Default constructor
    Index() : indexName(nullptr), tableName(nullptr), columnName(nullptr), id(indexCount + 1)
    {
        indexCount++;
        for (int i = 0; i < 3; i++) meta[i] = 0;
    }

    // Constructor w parameters
    Index(const char* idxName, const char* tblName, const char* colName)
        : indexName(nullptr), tableName(nullptr), columnName(nullptr), id(indexCount + 1)
    {
        indexCount++;
        for (int i = 0; i < 3; i++) meta[i] = 0;

        setIndexName(idxName);
        setTableName(tblName);
        setColumnName(colName);
    }

    // Copy constructor
    Index(const Index& other)
        : indexName(nullptr), tableName(nullptr), columnName(nullptr), id(other.id)
    {
        indexCount++;

        if (other.indexName) {
            indexName = new char[std::strlen(other.indexName) + 1];
            std::strcpy(indexName, other.indexName);
        }

        if (other.tableName) {
            tableName = new char[std::strlen(other.tableName) + 1];
            std::strcpy(tableName, other.tableName);
        }

        if (other.columnName) {
            columnName = new char[std::strlen(other.columnName) + 1];
            std::strcpy(columnName, other.columnName);
        }

        for (int i = 0; i < 3; i++)
            meta[i] = other.meta[i];
    }

    // Destructor
    ~Index()
    {
        delete[] indexName;
        delete[] tableName;
        delete[] columnName;
        indexCount--;
    }

    // Assignment operator
    Index& operator=(const Index& other)
    {
        if (this == &other) return *this;

        delete[] indexName;
        delete[] tableName;
        delete[] columnName;

        indexName = tableName = columnName = nullptr;

        if (other.indexName) {
            indexName = new char[std::strlen(other.indexName) + 1];
            std::strcpy(indexName, other.indexName);
        }

        if (other.tableName) {
            tableName = new char[std::strlen(other.tableName) + 1];
            std::strcpy(tableName, other.tableName);
        }

        if (other.columnName) {
            columnName = new char[std::strlen(other.columnName) + 1];
            std::strcpy(columnName, other.columnName);
        }

        for (int i = 0; i < 3; i++)
            meta[i] = other.meta[i];

        return *this;
    }

    // Getters / setters
    const char* getIndexName() const { return indexName; }
    void setIndexName(const char* n) {
        if (!n || std::strlen(n) == 0) return;
        delete[] indexName;
        indexName = new char[std::strlen(n) + 1];
        std::strcpy(indexName, n);
    }

    const char* getTableName() const { return tableName; }
    void setTableName(const char* n) {
        if (!n || std::strlen(n) == 0) return;
        delete[] tableName;
        tableName = new char[std::strlen(n) + 1];
        std::strcpy(tableName, n);
    }

    const char* getColumnName() const { return columnName; }
    void setColumnName(const char* n) {
        if (!n || std::strlen(n) == 0) return;
        delete[] columnName;
        columnName = new char[std::strlen(n) + 1];
        std::strcpy(columnName, n);
    }

    int getId() const { return id; }
    static int getIndexCount() { return indexCount; }

    // Operator ==
    bool operator==(const Index& other) const
    {
        bool sameIndex = (indexName && other.indexName) ?
            (std::strcmp(indexName, other.indexName) == 0) :
            (indexName == other.indexName);

        bool sameTable = (tableName && other.tableName) ?
            (std::strcmp(tableName, other.tableName) == 0) :
            (tableName == other.tableName);

        bool sameColumn = (columnName && other.columnName) ?
            (std::strcmp(columnName, other.columnName) == 0) :
            (columnName == other.columnName);

        return sameIndex && sameTable && sameColumn;
    }

    // Cast operator (to int -> id)
    explicit operator int() const { return id; }

    // Operator ! (invalid index)
    bool operator!() const {
        return !indexName || !tableName || !columnName;
    }

    // Operator < (alphabetical compare)
    bool operator<(const Index& other) const {
        if (!indexName || !other.indexName) return false;
        return std::strcmp(indexName, other.indexName) < 0;
    }

    // Operator <<
    friend std::ostream& operator<<(std::ostream& out, const Index& idx)
    {
        out << "Index[id=" << idx.id
            << ", name=" << (idx.indexName ? idx.indexName : "NULL")
            << ", table=" << (idx.tableName ? idx.tableName : "NULL")
            << ", column=" << (idx.columnName ? idx.columnName : "NULL")
            << "]";
        return out;
    }
};


int Index::indexCount = 0;