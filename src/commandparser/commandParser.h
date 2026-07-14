//7-5-2026: CommandParser om te kijken of een zoekvraag met het woord 'search','multi' of 'ranked' begint.
// parser: stuk code dat input begrijpt en opsplitst in bruikbare onderdelen

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>

// In tegenstelling tot search, parser en indexer gaan we hier een class maken: dit doen 
// we omdat de andere pure functies zijn en geen interne toestand hebben. geen geheugen tussen calls
// en stateless zijn. De Commandparser beheert een interactieflow: blijft draaien, leest input,
// beslist wat er geberut, roept functies aan: gedrag over tijd, niet 1 losse berekening.

//15-6-2026: de functie 'trim' zat nog niet in de header: dat hoeft niet want alleen in header zetten: functie declaraties,
// class definities, templetlates, inline functions. 
// En... je hoeft denk ik ook geen geimporteerde functies in je header te zetten.

class CommandParser
{
    public:
        //start REPL loop: functie run met als parameters een map (string,set-> string) genaamd index.
        void run(const std::unordered_map<std::string,std::unordered_map<std::string,std::vector<int>>>&index);
        void initCommands(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>>& index);

    private:
        // 11-5-2026: Maak een hashmap met als key een string en value een functie (value is dus een functie)
        // De functie returned niks en krijgt een query als string. Dit doen we om if else statements
        // te voorkomen en gebruik te maken van een dispatch map (makkelijk uitbreidbaar)
        // 12-5-2026: deze functie dus in commandParser.h gezet
        std::unordered_map<std::string, std::function<void(const std::string&)>> commands;

};
