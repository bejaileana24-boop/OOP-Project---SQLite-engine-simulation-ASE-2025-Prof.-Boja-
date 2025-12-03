#include <iostream>
#include <cstring>
#include "command.h"

using namespace std;

Command::Command() : type(UNKNOWN){}

Command::Command(const char* cmd)
{
	command = new char[strlen(cmd) + 1];
	strcpy_s(command, strlen(cmd) + 1, cmd);
	type = determineCommandType(cmd);
}

Command::Command(const Command& other)
{
	command = new char[strlen(other.command) + 1];
		strcpy_s(command, strlen(other.command) + 1, other.command);

		type = other.type;
}

Command::~Command()
{
	delete[] command;
}

Command& Command::operator=(const Command& other)
{
	if (this == &other)
		return;

	if (this->command != nullptr)
		delete[] this->command;
	else
	{
		this->command = new char[strlen(other.command) + 1];
		strcpy_s(this->command, strlen(other.command) + 1, other.command);

		this->type = other.type;
	}
}

bool Command::operator==(const Command& other) const
{
	return (strcmp(this->command, other.command) == 0);
}

bool Command::operator!() const
{
	return this->type == UNKNOWN;
}

CommandType Command::getType() const
{
	return type;
}

ostream& operator<<(ostream& out, const Command& cmd)
{
	out << "Command: " << cmd.command << "\nType: " << cmd.type << endl;
	return out;
}

CommandType Command::determineCommandType(const char* cmd)
{
	char* s = new char[strlen(cmd) + 1];
	strcpy_s(s, strlen(cmd) + 1, cmd);

	char* token = strtok(s, " ");
	if (token == nullptr)
	{
		delete[] s;
		return UNKNOWN;
	}

	if (strcmp(token, "CREATE") == 0  || strcmp(token, "create") == 0)
	{
		token = strtok(nullptr, " ");
		if (strcmp(token, "TABLE") == 0 || strcmp(token, "table") == 0)
		{
			delete[] s;
			return CREATE_TABLE;
		}
		if (strcmp(token, "INDEX") == 0 || strcmp(token, "index") == 0)
		{
			delete[] s;
			return CREATE_INDEX;
		}
	}

	if (strcmp(token, "DROP") == 0  || strcmp(token, "drop") == 0)
	{
		token = strtok(nullptr, " ");
		if (strcmp(token, "TABLE") == 0 || strcmp(token, "table") == 0)
		{
			delete[] s;
			return DROP_TABLE;
		}
		if (strcmp(token, "INDEX") == 0 || strcmp(token, "index") == 0)
		{
			delete[] s;
			return DROP_INDEX;
		}
	}

	if (strcmp(token, "DISPLAY") == 0  || strcmp(token, "display") == 0)
	{
		token = strtok(nullptr, " ");
		if (strcmp(token, "TABLE") == 0 || strcmp(token, "table") == 0)
		{
			delete[] s;
			return DISPLAY_TABLE;
		}
	}

	if (strcmp(token, "SELECT") == 0 || strcmp(token, "select") == 0)
	{
		delete[] s;
		return SELECT;
	}

	if (strcmp(token, "UPDATE") == 0 || strcmp(token, "update") == 0)
	{
		delete[] s;
		return UPDATE;
	}

	if (strcmp(token, "INSERT") == 0 || strcmp(token, "insert") == 0)
	{
		delete[] s;
		return INSERT;
	}

	if (strcmp(token, "DELETE") == 0 || strcmp(token, "delete") == 0)
	{
		delete[] s;
		return DELETE;
	}

	delete[] s;
	return UNKNOWN;
}

char* Command::getCommandString() const
{
	return command;
}


