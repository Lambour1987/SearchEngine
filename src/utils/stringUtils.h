//15-6-2026: hier de trimfunctie gezet.
// Deze wordt gebruikt in zowel de commandParser als in testqueries.
// Een cpp bestand maken is niet nodig: header only utility

#pragma once
#include <string>

inline std::string trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start);
}