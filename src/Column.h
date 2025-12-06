#pragma once
#include <iostream>
#include <string>

enum ColumnType {
    COL_UNKNOWN = 0,
    COL_INTEGER = 1,
    COL_FLOAT = 2,
    COL_TEXT = 3
};

class Column {
private:
    std::string name;          // column name
    ColumnType type;           // integer / float / text
    int size;                  // size (for text / numeric limits)
    std::string defaultValue;  // default value as string

    static int columnCount;    // how many Column objects exist
    int stats[3];              // small statistics array
    const int id;              // const field

public:
    Column()
        : name(""), type(COL_UNKNOWN), size(0), defaultValue(""), id(columnCount + 1)
    {
        columnCount++;
        for (int i = 0; i < 3; i++) {
            stats[i] = 0;
        }
    }

    Column(const std::string& n, ColumnType t, int s, const std::string& def = "")
        : name(""), type(COL_UNKNOWN), size(0), defaultValue(""), id(columnCount + 1)
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

    Column(const Column& other)
        : name(other.name),
        type(other.type),
        size(other.size),
        defaultValue(other.defaultValue),
        id(other.id)
    {
        columnCount++;
        for (int i = 0; i < 3; i++) {
            stats[i] = other.stats[i];
        }
    }

    ~Column()
    {
        // no manual delete needed (std::string handles memory)
        columnCount--;
    }

    Column& operator=(const Column& other)
    {
        if (this == &other) {
            return *this;
        }

        // id is const
        name = other.name;
        type = other.type;
        size = other.size;
        defaultValue = other.defaultValue;

        for (int i = 0; i < 3; i++) {
            stats[i] = other.stats[i];
        }

        return *this;
    }

    const std::string& getName() const {
        return name;
    }

    void setName(const std::string& n) {
        if (n.empty()) {
            // do not accept empty names
            return;
        }
        name = n;
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

    const std::string& getDefaultValue() const {
        return defaultValue;
    }

    void setDefaultValue(const std::string& def) {
        // we accept empty default
        defaultValue = def;
    }

    int getId() const {
        return id;
    }

    static int getColumnCount() {
        return columnCount;
    }

    bool operator==(const Column& other) const
    {
        return name == other.name &&
            defaultValue == other.defaultValue &&
            type == other.type &&
            size == other.size;
    }

    explicit operator int() const {
        return size;
    }

    bool operator!() const {
        return name.empty() || type == COL_UNKNOWN;
    }

    bool operator<(const Column& other) const {
        return size < other.size;
    }

    friend std::ostream& operator<<(std::ostream& out, const Column& c)
    {
        out << "Column[ id=" << c.id
            << ", name=" << (c.name.empty() ? "NULL" : c.name)
            << ", type=" << c.cTypeAsText()
            << ", size=" << c.size
            << ", default=" << (c.defaultValue.empty() ? "NULL" : c.defaultValue)
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

// static field initialization
int Column::columnCount = 0;
