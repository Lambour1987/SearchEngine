// 13-5-2026

// Dus includes en de functies uit query evaluator en eventueel structs of constructors.
// geen using namespace std

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../tokenizer/tokenizer.h"
//28-5-2026: T.b.v. NOT operator ook een set
#include <unordered_set>

std::unordered_set<std::string> evaluatePostfix(
    const std::vector<Token>& postfix, 
    const std::unordered_map<std::string, 
    std::unordered_set<std::string>>&index);


