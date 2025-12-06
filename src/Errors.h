#pragma once

// Simple error codes used by the command processor
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
            return "OK";
        case ERR_UNKNOWN_COMMAND:
            return "Unknown or unsupported command.";
        case ERR_EMPTY_COMMAND:
            return "Empty command.";
        case ERR_TOO_FEW_TOKENS:
            return "Not enough elements in the command.";
        case ERR_INVALID_FORMAT:
            return "Invalid command format.";
        default:
            return "Unspecified error.";
        }
    }
};
