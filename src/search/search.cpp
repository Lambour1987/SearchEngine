#include "search.h"
#include "../fileLoader/fileLoader.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// ============================
// == SEARCH ==
// ============================

// Zoekfunctie search met als input(map-set genaamd index en een query). Output void.
// Denk bij void: soms een printfunctie maar met referenties werken we gewoon direct in de variabele
// de we hebben opgeroepen. Dit is dus geen lineair search of binary search
void search(const unordered_map<string,unordered_map<string, vector<int>>>& index, const string&query)
{
    //Zet query naar lowercase (consistent met tokenize)
    //Voor iedere letter in de query moeten we deze naar lowercase zetten
    //Tussendoor: technisch gezien gaat omzetten van 'hello' naar 'hello' itt'HELLO' naar 'hello' net iets sneller.
    //Dus conversie kost minimale tijd

    //BELANGRIJK: Nu maken we bij de parameters een referentie naar een query, en hier kopieren we de query (we kopieren niet de referentie)
    // toch is dit sneller dan direct in de parameteroproep een kopie te maken (const string& query wordt dan 'string query'). Mag wel maar
    // is iets langzamer. Nu: we maken pas een kopie als hem nodig hebben. Blijkbaar.
    string q = query;
    // char dus geen const en & nodig
    // 18-5-2026: itt MultiSearch hier alleen 'tolower' nodig omdat het om 1 woord gaat'
    // Bijvoorbeeld:
    
    for(char& c:q)
    {
        c=tolower(c);
    }

    // We zoeken de query in de map-set index. Als niet gevonden, geef melding.
    // Dus we zoeken dan in de map-set index de query. Dat is gelijk aan index.end());
    // Wat is index.end()); ook alweer?
    if(index.find(q) == index.end())
    {
        cout<<"Woord niet gevonden"<<endl;
        return;
    }
    
    // Als deze wel gevonden is zoek in de index naar de query (nog niet eerder zo gezien)
    // en retourneer de filename.
    // Dit zou ook binnen de if statement kunnen? In dit geval betekent het precies hetzelfde?
    // Verschil: eerst if statement. Als gevonden dan returnen. Of. Hij is niet- niet gevonden.
    // Dus wel gevonden. Dus door met volgende regel
    for(const auto& [file,positions]:index.at(q))
    {
        cout<<file<<endl;
    }
}

// ============================
// == MULTISEARCH ==
// ============================
// Als we zoeken (query): "hello bye"
// hello staat in a.txt en b.txt en bye staat in b.txt
// Maar we willen alleen waar Beide in voorkomen: Intersection (denk aan set, union, intersection, difference)

// Functie searchMulti heeft als input de map-set index en de query. Als output niks
// Bijvoorbeeld: De unordered_map<string, unordered_set<string>>&index:
// "apple"  → {"a.txt", "b.txt"}
// "banana" → {"b.txt"}
// "orange" → {"a.txt"}
// De const string&query
// "apple banana orange"
void multiSearch(const unordered_map<string, unordered_map<string, vector<int>>>&index, const string&query)
{
    
    // Roep de functie tokenize met argurment query en sla op in een woordvector
    // Bijvoorbeeld: 
    // query = "apple banana orange"
    // Tokenize: → split + lowercase + parsing
    // Words: words = ["apple", "banana", "orange"]
    vector<string> words = tokenize(query);
    // Als woord is leeg dan retourneer
    if(words.empty())
    {
        return;
    }
    // start met eerste woord (dat is a[0]) en sla het resultaat op in result: geef de waarde die hoort bij key hello
    // Bijvoorbeeld:
    // words[0] = "apple"
    // index["apple"]
    // "apple" → {"a.txt", "b.txt"}
    // result = {"a.txt", "b.txt"}
    auto result = index.at(words[0]);
    // intersect met de rest: begin bij 1 en ga door tot einde (let op dit lijkt wel een beetje op LL maken)
    // BELANGRIJK: waarom niet int i =1 maar size_t=1? int werkt ook. maar size_t is correcter: is een unsigned
    // integer type speciaal voor groottes en indices. size_t komt komt uit de words vector.
    // als je int i zou doen vergelijk je int(signed) met size_t(unsigned). compiler moet dan converteren.
    // Dus size_t is het datatype
    for(size_t i=1; i<words.size(); i++)
    {
        //volgende woord in index opslaan in variabele nextSet
        // Bijvoorbeeld
        // words[i] = "banana"
        // index["banana"]
        // "banana" → {"b.txt", "c.txt"}
        // nextSet = {"b.txt", "c.txt"}
        const auto& nextMap = index.at(words[i]);
        //maak een string set 'temp' aan.
        //17-6-2026: ook gewijzigd
        unordered_map<string, vector<int>> temp;
        // Loop door iedere file heen uit resultaat:
        // Bijvoorbeeld:
        // result = {"a.txt", "b.txt"}
        for(const auto& [file, positions]:result)
        {
            // Als de file in nextset zit? (gebruik Count)
            if(nextMap.count(file))
            {
                //voeg de file toe aan tijdelijke set
                //17-6-2026: aangepast temp.insert(file);
                temp[file] = positions;
            }
        // sla tijdelijk resultaat op in result
        }
        result = temp;
    }
    
    // Doorloop iedere file in het resultaat en print resultaat.
    // Bijvoorbeeld:
    // result = {a.txt, b.txt}
    for(const auto& [file,positions]: result)
    {
        cout<<file<<endl;
    }
};

// ============================
//      == RANKING ==
// ============================

// Per file geven we een score. Woord komt 1x voor +1, 

// Functie RankedSearch(map-set: index en query) en void output
void rankedSearch(const unordered_map<string,unordered_map<string, vector<int>>>&index, const string&query)
{
    // Roep tokenize functie aan(input is query) en sla resultaat op in words
    // We hebben bijv als query: 'hello bye'. We roepen tokenize functie op en die geeft 
    // dan terug: ["hello", "bye"]
    const auto& words = tokenize(query);
    
    //Score map: maak een hashmap (string, int) voor de score
    unordered_map<string,int> score;
    //Per woord files ophalen
    for(const auto& word:words)
    {
        //zoek woord op in index. Als gevonden ga door. zoek in volledige index (na einde)
        //17-6-2026: gewijzigd van if(index.find(word)==index.end()) naar
        if(!index.count(word))
        {
            continue;
        }
        // voor iedere file op de positie word van de index
        // dus niet: for(const auto&file:word[index])
        for(const auto&[file,positions]:index.at(word))
        {
            //Verhoog de score van die file
            // gewijzigd van core[file]++; naar
            //17-6-2026 gewijzigd van score[file] +=positions.size(); // of+1 naar
            score[file] +=1;
        }
    }
    //maak een vector van paren (string, int) aan 'ranked'
    //17-6-2026: gewzijgigd van vector<pair<string, int>> ranked; naar
    vector<pair<string, int>> ranked(score.begin(),score.end());
    //voor ieder paar in de score
    //17-6-2026: weggehaald
    //for(const auto&[file,s]: score)
        // Voeg paar (file, s) toe aan de vector van paren ranked
        //ranked.push_back({file, s});

    // Sorteer rankden van begin tot eind, gebruik een lambda (inline) functie
    // lambda [capture](parameters) -> return type { body }. Hier geen capture
    // BELANGRIJK: lambda is hier dus onderdeel van sort. Je zet het achter sort neer. Normaal
    // niet verplicht. Nu wel: Ik bepaal hier nu de volgorde: we zetten a voor b als a.second 
    // groter is dan b.second. Return een boolean: want je doet eigenlijk een if statement: is a>b.

    sort(ranked.begin(), ranked.end(),
    //capture en daarna tussen haakjes de parameters
    // 17-6-2026: gewijzigd van [](const auto& a, const auto&b) naar
    [](auto&a, auto&b)
    {
        return a.second>b.second;
    });

    // We gaan nu weer ieder paar uit ranked printen
    for(const auto&[file,s]:ranked)
    {
        cout<<file <<" -> "<<s<<endl;
    }
}
