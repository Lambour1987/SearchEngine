//13-6-2026: Dus bedoelt om testqueries te bouwen en uit te voeren

// zorg dat headerfiles 1x worden meegenomen
#pragma once

//15-6-2026: erbij gezet
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// functie runQueryTests met parameter filename en die niets retourneert
// (denk er weer aan: binnen '.h' files werken met std en ';')
void runQueryTests(
    const std::string& filename,
    const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>& index);