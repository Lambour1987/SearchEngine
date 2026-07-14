// 13-5-2026: 

// Voorkomen dat header meerdere keren wordt meegenomen
# pragma once
# include <vector>

// Verwijzing naar token.h (nog maken?)
#include "../tokenizer/tokenizer.h"

// onder andere de functie toPostfix (zie cpp)
std::vector<Token> toPostfix(const std::vector<Token>& tokens);
