//7-5-2026: 

// Voeg bestand toe waarin de headers ? staan
#include "fileLoader.h"

// lezen (ifstream) en schrijven (ofstream)
#include <fstream>

// input & output (console): cout: communicatie met terminal
#include <iostream>

// karakterfuncties
#include <cctype>

#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

// ============================
// == GET FILES ==
// ============================

// Functie om alle documenten uit een dataset op te halen
// Resultaat terug in een vector van bestandspaden (onderdeel van filesystem (fs)).
// Als input referentie naar folder (niet aan te passen)
vector<fs::path> getFiles(const string& folder)
{
    // Vector aanmaken om bestandspaden in op te slaan
    vector<fs::path> files;
    // Voor iedere bestandsfile in het filesystem (directory) maken we een referentie en wordt het datatype automatisch bepaald
    // Dit is dus gewoon door een directory lopen: 1 voor 1 bestanden geven.
    for(const auto&entry:fs::directory_iterator(folder))
    {
    // Als het bestand een gewone file is dan moeten we de bestandslocatie/ pad aan de vector toevoegen
    // is_regular_file is een functie die bepaalt of we met een echt bestand te maken hebben: dus geen
    // mappen, snelkoppelingen en systeembestanden
        if(entry.is_regular_file())
        {
            //15-6-2026: bijgevoegd:
            if(entry.path().filename() == "testqueries.txt")
            {
                continue;
            }
            // voeg pad toe aan vector getFiles
            files.push_back(entry.path());
        }
    }
    // retourneer de vector met bestandspaden
    return files;
}

// ============================
// == READ FILES ==
// ============================

// Functie die het bestand leest en als input heeft een referentie naar path van het type path (van fs)
// en als output een string geeft
string readFile(const fs::path& path)
{
    // bestand openen o.b.v. bestandslocatie/pad
    ifstream file(path);
    // probeer het bestand te openen. Als dit niet lukt melding. 
    if(!file)
    {
        cout<<"Bestand niet geopend"<<endl;
    }
    // declareer string variabelen content (voor alle regels) en line (1 regel)
    string content;
    string line;
    // zolang er regels in de file zijn, stop die in line. Gebruikt functie getline.
    while (getline(file, line))
    {
    // voeg de lijnen toe aan de grote string: In deze fase nog handig 1 grote string te gebruiken,
    // en niet een vector. Scheidt deze met een 'enter'. Let op niet met endl; want het gaat hier
    // over 1 string en niet verschillende strings;
        content += line + "\n";
    }
    // return content
    return content;
};

// ============================
// == TEXT PROCESSING ==
// ============================

// Tekst splitsen in woorden "Hallo Wereld Hallo Wereld Bye Wereld" 
// wordt ("hallo", "wereld", "hallo", "wereld", "bye", "wereld")
// 13-5-2026: We hebben nu 2 tokenizers. De huidige hier: Woorden uit
// bestanden halen (inverted index, frequency map). Nieuwe tokenizer erbij:
// Zoekopdrachten begrijpen.

//Functie tokenize met als input een snelkoppeling naar tekst die we niet willen wijzigen
//en als output een vector van strings
vector<string> tokenize(const string& text)
{
    // Maak een vector van woorden aan
    vector<string> words;
    // Maak een variabele 'woord' aan
    string word;
    // Doorloop ieder karakter van de tekst (voor een char is const en & niet nodig)
    for(char c:text)
        // Als de tekst een spatie bevat (' '), een tab bevat ('    ') of een endl;
        // gebruik (isspace). Dan controleer je alle 3.
        // Vergelijk: als we vragen of c leeg is: c==' '. maar nu met functie dus: isspace(c). (dan c als argument)
        if(isspace(c))
        {
        // check of het woord helemaal leeg is.
                if(!word.empty())
                {
                // niet leeg dan pushen we het woord op de vector
                    words.push_back(word);
                // wat is word.clear?
                    word.clear();
                }
        }
        else
        {
        // Geen spatie is. Dan de letter aan de woordstring toevoegen in kleinkapitaal:
            word +=static_cast<char>(tolower(c));
        }

    //Als het woord niet leeg is, voeg het woord toe aan de string woorden
    if(!word.empty())
    {
        words.push_back(word);
    }
    // Retourneer woorden
    return words;
}    