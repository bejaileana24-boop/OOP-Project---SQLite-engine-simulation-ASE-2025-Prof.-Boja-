#pragma once
#include <iostream>
#include <cstring>

enum ColumnType {
    COL_UNKNOWN = 0,
    COL_INTEGER = 1,
    COL_FLOAT = 2,
    COL_TEXT = 3
};

class Column {
private:
    char* name;          //name column
    ColumnType type;     // type: integer / float / text
    int size;          
    char* defaultValue;  

    static int columnCount;   
    int stats[3];            
    const int id;             

public:

    // Constructor default
    Column() : name(nullptr), type(COL_UNKNOWN), size(0), defaultValue(nullptr), id(columnCount + 1)
    {
        columnCount++;
        for (int i = 0; i < 3; i++) {
            stats[i] = 0;
        }
    }

    // Constructor w parameters
    Column(const char* n, ColumnType t, int s, const char* def = "")
        : name(nullptr), type(COL_UNKNOWN), size(0), defaultValue(nullptr), id(columnCount + 1)
    {
        columnCount++;
        for (int i = 0; i < 3; i++) {
            stats[i] = 0;
        }

        
        setName(n);
        setType(t);
        setSize(s);
        setDefaultValue(def);
    }

    // Copy constructor 
    Column(const Column& other) : name(nullptr), type(COL_UNKNOWN), size(0), defaultValue(nullptr), id(other.id)
    {
        columnCount++;
        for (int i = 0; i < 3; i++) {
            stats[i] = other.stats[i];
        }

        if (other.name != nullptr) {
            name = new char[std::strlen(other.name) + 1];
            std::strcpy(name, other.name);
        }

        type = other.type;
        size = other.size;

        if (other.defaultValue != nullptr) {
            defaultValue = new char[std::strlen(other.defaultValue) + 1];
            std::strcpy(defaultValue, other.defaultValue);
        }
    }

    // DESTRUCTOR
    ~Column()
    {
        delete[] name;
        delete[] defaultValue;
        columnCount--;
    }

    // Operator =
    Column& operator=(const Column& other)
    {
        if (this == &other) {
            return *this;
        }

        //free whatt we had
        delete[] name;
        delete[] defaultValue;

        name = nullptr;
        defaultValue = nullptr;

        if (other.name != nullptr) {
            name = new char[std::strlen(other.name) + 1];
            std::strcpy(name, other.name);
        }

        type = other.type;
        size = other.size;

        if (other.defaultValue != nullptr) {
            defaultValue = new char[std::strlen(other.defaultValue) + 1];
            std::strcpy(defaultValue, other.defaultValue);
        }

        for (int i = 0; i < 3; i++) {
            stats[i] = other.stats[i];
        }

        // id is const
        return *this;
    }

    // Getters / setters

    const char* getName() const {
        return name;
    }

    void setName(const char* n) {
        if (n == nullptr || std::strlen(n) == 0) {
            // nu acceptam nume gol
            return;
        }
        delete[] name;
        name = new char[std::strlen(n) + 1];
        std::strcpy(name, n);
    }

    ColumnType getType() const {
        return type;
    }

    void setType(ColumnType t) {
        if (t == COL_UNKNOWN) {
            return;
        }
        type = t;
    }

    int getSize() const {
        return size;
    }

    void setSize(int s) {
        if (s <= 0) {
            return;
        }
        size = s;
    }

    const char* getDefaultValue() const {
        return defaultValue;
    }

    void setDefaultValue(const char* def) {
        if (def == nullptr) {
            return;
        }
        delete[] defaultValue;
        defaultValue = new char[std::strlen(def) + 1];
        std::strcpy(defaultValue, def);
    }

    int getId() const {
        return id;
    }

    static int getColumnCount() {
        return columnCount;
    }

    // operator ==  
    bool operator==(const Column& other) const
    {
        bool sameName = false;
        if (name == nullptr && other.name == nullptr) {
            sameName = true;
        }
        else if (name != nullptr && other.name != nullptr) {
            sameName = (std::strcmp(name, other.name) == 0);
        }

        bool sameDefault = false;
        if (defaultValue == nullptr && other.defaultValue == nullptr) {
            sameDefault = true;
        }
        else if (defaultValue != nullptr && other.defaultValue != nullptr) {
            sameDefault = (std::strcmp(defaultValue, other.defaultValue) == 0);
        }

        return sameName && sameDefault && type == other.type && size == other.size;
    }

    // cast operator 
    explicit operator int() const {
        return size;
    }

    // operator ! 
    bool operator!() const {
        return name == nullptr || type == COL_UNKNOWN;
    }

    // operator < 
bool operator<(const Column& other) const {
        return size < other.size;
    }

    // operator <<
friend std::ostream& operator<<(std::ostream& out, const Column& c)
    {
        out << "Column[ id=" << c.id
            << ", name=" << (c.name ? c.name : "NULL")
            << ", type=" << c.cTypeAsText()
            << ", size=" << c.size
            << ", default=" << (c.defaultValue ? c.defaultValue : "NULL")
            << " ]";
        return out;
    }

private:
    const char* cTypeAsText() const {
        switch (type) {
        case COL_INTEGER: return "integer";
        case COL_FLOAT:   return "float";
        case COL_TEXT:    return "text";
        default:          return "unknown";
        }
    }
};


int Column::columnCount = 0;