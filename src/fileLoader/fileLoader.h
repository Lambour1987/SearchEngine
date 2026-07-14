//7-5-2026: parser.h: alleen functiedeclaraties, includes maar geen implementeaties
// Geeft aan dat functies ergens bestaan, maar niet hoe ze werken. Die staan in parser.cpp

//instructie voor compiler: zorg dat header bestand maar 1 x wordt ingeladen tijdens compilatie
#pragma once

//include
#include <filesystem>
#include <string>
#include <vector>

//ipv using namespace std; wel std gebruiken:
//BELANGRIJK: Extra gevaarlijk om using namespace toe te voegen aan header.

namespace fs = std::filesystem;

//functie getfiles met als parameter folder
std::vector<fs::path> getFiles(const std::string& folder);


//functie readfile met parameter pad uit pad
std::string readFile(const fs::path& path);


//functie tokenize met parameter tekst
std::vector<std::string> tokenize(const std::string& text);













