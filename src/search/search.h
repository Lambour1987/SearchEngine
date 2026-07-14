//7-5-2026

// Vertel compiler dat we steeds 1 head bestand gebruiken
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//BELANGRIJK: Nooit using namespace std in headers

//void Zoekfunctie (Search) met als parameter een unordered map met leden string en set van strings genaamd index en een string query
void search(const std::unordered_map<std::string,std::unordered_map<std::string, std::vector<int>>>& index, const std::string&query);

//void Zoekfunctie (Multisearch) zelfde parameters als Search
void multiSearch(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>&index, const std::string&query);

//void Zoekfunctie RankedSearch idem
void rankedSearch(const std::unordered_map<std::string,std::unordered_map<std::string, std::vector<int>>>&index, const std::string&query);

