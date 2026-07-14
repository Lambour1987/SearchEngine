//12-5-2026: Tokenize Struct

//pragma once om header max 1x te includeren
#pragma once
#include <string>
#include <vector>

enum class TokenType
{
    // Selectie van woorden die we kunnen maken
    WORD,
    PHRASE,
    AND,
    OR,
    COMMAND,
    NOT,
    //7-6-2026: Toegevoegd: parentheses
    LPAREN,
    RPAREN,
};

struct Token
{
    TokenType type;
    std::string value;
};

//4-6-2026: Dus let op: hier sluiten we de functie dus wel af met ';'. In het cpp bestand niet.
std::vector<Token> tokenizeQuery(const std::string& input);

