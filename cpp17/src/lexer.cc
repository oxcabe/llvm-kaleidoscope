#include <cstdio>
#include <cctype>
#include <string>

namespace Lexer {

// Lexer returns a positive, 8 bit value (0, 255) if character is
// unknown, otherwise one of these for known things.
enum Token {
    tok_eof = -1,

    // Commands
    tok_def = -2,
    tok_extern = -3,

    // Primary
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr; // Non-empty if tok_identifier
static double NumVal; // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
    static int LastChar = ' ';

    // Skip whitespace(s)
    while (std::isspace(LastChar)) {
        LastChar = std::getchar();
    }

    // Handle alphabetic characters and check for "command" tokens
    if (std::isalpha(LastChar)) {
        IdentifierStr = LastChar;

        while (std::isalnum((LastChar = std::getchar()))) {
            IdentifierStr += LastChar;
        }

        if (IdentifierStr == "def") {
            return tok_def;
        }

        if (IdentifierStr == "extern") {
            return tok_extern;
        }

        return tok_identifier;
    }

    // Handle numeric values
    // TODO: Fix cases like 1.23.456 allowing only a single '.' character
    if (std::isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;

        do {
            LastChar = std::getchar();
        } while (std::isdigit(LastChar) || LastChar == '.');

        NumVal = std::strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    // Handle comments
    if (LastChar == '#') {
        // Comment until end of line
        do {
            LastChar = std::getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            // Process next token after comment is ignored
            return gettok();
        }
    }

    // Check for EOF if nothing else matches
    if (LastChar == EOF) {
        return tok_eof;
    }

    // Finally, return character as ASCII value
    int ThisChar = LastChar;
    LastChar = std::getchar();
    return ThisChar;
}
}
