// 4-6-2026

// Voorkom dat headers vaker ingeladen worden
#pragma once

// Voeg een map, set en string toe
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

// Verwijs naar AST.h
#include "../types/AST.h"

// Maak een alias voor die hashmap string, set (van strings) en noem die InvertedIndex
// gewijzigd: using InvertedIndex = std::unordered_map<std::string, std::unordered_set<std::string>>;
using InvertedIndex = std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>;

// maak een functie evaluateAST met als parameters een pointer node naar een ASTNode, en
// de index (map incl set: maar gebruik de alias). Als output een set van strings
std::unordered_map<std::string,int> evaluateAST(ASTNode* node, const InvertedIndex& index);


