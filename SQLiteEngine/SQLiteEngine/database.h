#pragma once
#include <string>
#include "table.h"
#include "command.h"

class Database {
private:
	std::string name = "";
	Table* tables = nullptr;
	int tableCnt = 0;

public:
	Database();

	Database(const std::string& name);

	Database(const Database& db);

	std::string getName() const;

	int getTableCount() const;

	Database& operator=(const Database& db);

	Database& operator+=(const Table& table);

	Database& operator-=(const std::string& tableName);

	Table& operator[](const int tableIndex);

	Table* findTable(const std::string& tableName) const;

	void processCommand(const Command& cmd);

	friend std::ostream& operator<<(std::ostream& os, const Database& db);

	~Database();

};