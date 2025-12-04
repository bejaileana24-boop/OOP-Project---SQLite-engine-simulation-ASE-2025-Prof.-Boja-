# 📁 Project Structure (Phase 1)
```
SQLite Engine Project/
│
├─ src/
│   ├─ main.cpp
│   ├─ CommandProcessor.h
│   ├─ ParsedCommand.h
│   ├─ Database.h
│   ├─ Table.h
│   ├─ Column.h
│   ├─ Index.h
│   └─ Errors.h
│
├─ .gitignore
└─ README.md
```


# 📄 HEADER FILES

## 1. CommandProcessor
### Definition:
Responsible for identifying which SQL command was entered and directing it to the correct parser.
### Purpose:
Receives raw command strings, detects the command type, validates format, and outputs a structured ParsedCommand.

## 2. ParsedCommand
### Definition:
A structured object representing a successfully parsed SQL command.
### Purpose:
Stores all relevant fields (command type, table name, columns, WHERE clause, values). Printed by main.cpp after validation.

## 3. Database
### Definition:
Represents the entire database (multiple tables).
### Purpose:
Manages table metadata. Fully used in Phase 2 when actual files and storage are introduced.

## 4. Table
### Definition:
Represents a single database table.
### Purpose:
Stores table name, number of columns, and column definitions.

## 5. Column
### Definition:
Represents one column inside a table definition.
### Purpose:
Stores name, type, size, and default value. A building block for table definitions.

## 6. Index
### Definition:
Represents an index created via: <br>
```CREATE INDEX index_name ON table_name (column_name)```
### Purpose:
Validates and stores index definitions for later implementation in Phase 2.

## 7. Errors
### Definition:
Centralized error code definitions.
### Purpose:
Keeps parsing consistent and readable by avoiding hardcoded numeric errors.