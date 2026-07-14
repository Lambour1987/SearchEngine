// # vertel compiler dat hij maar 1x de header moet inladen

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//BELANGRIJK: Nooit using namespace std gebruiken in headers

namespace fs = std::filesystem;

using invertedIndex = std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>;

invertedIndex buildInvertedIndex(const std::vector<fs::path>&files);

std::unordered_map<std::string, int> buildFrequencyMap(const std::vector<std::string>&words);