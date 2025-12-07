#pragma once

// Error codes used by the command processor
enum ErrorCode {
    ERR_OK = 0,             // no error
    ERR_UNKNOWN_COMMAND,    // command type not recognized
    ERR_EMPTY_COMMAND,      // empty line
    ERR_TOO_FEW_TOKENS,     // not enough parts in the command
    ERR_INVALID_FORMAT      // wrong keyword order / structure
};

class Errors {
public:
    // Returns a const char* message for each error code
    static const char* getMessage(ErrorCode err) {
        switch (err) {
        case ERR_OK:
            return "OK.";

        case ERR_UNKNOWN_COMMAND:
            return "Unknown or unsupported command. "
                "Supported commands: CREATE TABLE, DROP TABLE, DISPLAY TABLE, "
                "CREATE INDEX, DROP INDEX, INSERT, SELECT, UPDATE, DELETE.";

        case ERR_EMPTY_COMMAND:
            return "Empty command. Please type a SQL-like command or EXIT.";

        case ERR_TOO_FEW_TOKENS:
            return "Incomplete command. One or more keywords/parameters are missing.";

        case ERR_INVALID_FORMAT:
            return "Invalid command format. Check the order of keywords and basic syntax. "
                "Example: CREATE TABLE name (col, type, size, default).";

        default:
            return "Unspecified error.";
        }
    }
};
