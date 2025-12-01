#pragma once
#include <string>

enum CommandType {
	UNKNOWN,
	CREATE_TABLE,
	DROP_TABLE,
	DISPLAY_TABLE,
	CREATE_INDEX,
	DROP_INDEX,
	INSERT,
	SELECT,
	UPDATE,
	DELETE
};

class Command {
private:
	std::string command = "";
	CommandType type = UNKNOWN;

	CommandType determineCommandType(const std::string& cmd);

public:
	Command();

	Command(const std::string& cmd);

	Command(const Command& cmd);

	CommandType getType() const;

	std::string getCommandString() const;

	Command& operator=(const Command& cmd);

	bool operator==(const Command& cmd) const;

	bool operator!() const;

	~Command();

	friend std::ostream& operator<<(std::ostream& os, const Command& cmd);
};


