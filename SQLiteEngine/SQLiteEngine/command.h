#pragma once

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
	char* command = nullptr;
	CommandType type = UNKNOWN;

	CommandType determineCommandType(const char* cmd);

public:
	Command();

	Command(const char* cmd);

	Command(const Command& cmd);

	CommandType getType() const;

	char* getCommandString() const;

	Command& operator=(const Command& cmd);

	bool operator==(const Command& cmd) const;

	bool operator!() const;

	~Command();

	friend std::ostream& operator<<(std::ostream& os, const Command& cmd);
};


