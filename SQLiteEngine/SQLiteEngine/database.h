#pragma once
#include <string>
#include "table.h"
#include "command.h"

class Database {
private:
	char* name = nullptr;
	Table* tables = nullptr;
	int tableCnt = 0;

public:
	Database();

	Database(const char* name);

	Database(const Database& db);

	char* getName() const;

	int getTableCount() const;

	Database& operator=(const Database& db);

	Database& operator+=(const Table& table);

	Database& operator-=(const char* tableName);

	Table& operator[](const int tableIndex);

	Table* findTable(const char* tableName) const;

	void processCommand(const Command& cmd);

	friend std::ostream& operator<<(std::ostream& os, const Database& db);

	~Database();

};