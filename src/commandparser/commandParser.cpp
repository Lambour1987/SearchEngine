//7-5-2026: Met if statements 
//8-5-2026: If- statements vervangen door command/query/split

// Gebruik de headerfile "commandparser.h"
#include "commandparser.h"
#include "../utils/StringUtils.h"

// Omdat we binnen deze cpp ook search, MultiSearch en Ranked gebruiken
// moeten we de mapstructuur geven: allen staan binnen search.
// Dus search.h oproepen is mogelijk. Wat technisch kan maar wat we niet moeten doen:
// Is het oproepen van "../search/search.cpp": dat wordt dan dubbel functies uitvoeren
// geloof ik.

#include "../search/search.h"
#include <iostream>

// 10-5-2026: functie als object behandelen, lambda support, function wrappers, callbacks en callable objects
// Dit is belangrijk voor de dispatch table
#include <functional>

// t.b.v. transform
#include <algorithm>

//In cpp file dus wel weer using namespace std. In .h file niet doen.
using namespace std;

// helperfunctie: string trimmen:

//15-6-2026: dit naar een utilityfunctie verplaatst
// //functie trim met een input string en output ook een string
// string trim(const string& str)
// {
//     // functie find_first_not_of oproepen op string. Sla resultaat op in start
//     // Met deze functie zoeken we de eerste string op die geen spatie of tab is
//     // Dus '     hello' wordt start  = 5;
//     // size_t: int kan ook maar STL werkt met size_t
//     // let op:" spatie en dan \t": Dat is zoeken naar spaties en tabs.
//     size_t start = str.find_first_not_of(" \t");

//     //retourneer ternary operator: if statement. Als start niet gevonden is. Return "".
//     // Anders retourneer een substring. 
//     // we npos: no position.
//     return(start == string::npos)?"":str.substr(start) ;

// }

// 12-5-2026: Run en initCommands Gesplitst: Hier initCommands
void CommandParser::initCommands(const unordered_map<string, unordered_map<string, vector<int>>>&index)
{
    // Beoordeel of de string begint met "Search"
    // zoek in de input het woord "search". zoek substring maar begin exact op positie 0 (prefix check)
    // 9-5-2026: weg if(input.rfind("search",0)==0)
    // 11-5-2026: nu de if /else statements weghalen. Was if(command == "search") wordt commands["search"] = [&](const string&query)
    // Koppel de commandonaam (commands is unordered map) search aan een lambda functie. Dus geen if else statements meer
    // de lambda capture &: gebruik variabelen van buiten. De index.
    // BELANGRIJK: gewone functies hebben parameters, lambda's hebben daarnaast capture's.
    commands["search"] = [&](const string&query)
    {
        //Als dat zo is roep op de input de functie substring op en sla het resultaat op in een string query
        //8-5-2026: Gebruik trim helperfunctie: bijv als gebruikers niet "search" maar "search     "intypen
        //9-5-2026 Legacy Substring parser weggehaald. nu gebruiken split parser
        //9-5-2026: ipv positie 7 in trim(input.substr(7)); Nu zoeken waar eerste spatie zit:
        // string query = trim(input.substr(7));
        //roep de zoekfunctie op met als parameters index en query
        search(index, query);
    };
    //12-5-2026: Alias maken: i.p.v. 'search' mag je ook 's' typen
    commands["s"]=commands["search"];


    // Anders roep reverse vinden op het woord multi, start vanaf positie 0
    // 9-5-26: weg else if(input.rfind("multi",0)==0)
    commands["multiSearch"] = [&](const string& query)
    {
        //Als dat zo is roep op de input de functie substring op en sla het resultaat op in een string query
        // string query = trim(input.substr(6));
        //roep de multizoekfunctie op met als parameters index en query
        multiSearch(index, query);
    };
        // Anders roep reverse vinden op het woord rank, start vanaf positie 0
    // else if(input.rfind("rank",0)==0)
    commands["rank"] = [&](const string& query)
    {
        //Als dat zo is roep op de input de functie substring op en sla het resultaat op in een string query
        // string query = trim(input.substr(5));

        
        //roep de rankedSearch functie op met als parameters index en query
        rankedSearch(index, query);
    };
    // 12-5-2026 Het woord: 'help' (Zie ook waarom dispatch tables makkelijk zijn)
    // 12-5-2026: ipv endl; (bijv.cout<<"Available commands:"<<endl; nu \n ) gebruikt:
    // Verschil: \n = new line en endl = new line + meteen laten zien: dat is duurder.
    // want bij endl moetje flushen
    commands["help"]=[&](const string&query)
    {
        //Geef lijst weer met beschikbare commands
        cout<<"\nAvailable commands:\n";
        cout<<"search <query>\n";
        cout<<"multiSearch <query>\n";
        cout<<"- rank <query>\n";
        cout<<"help \n"<<endl;
        cout<<"- exit \n"<<endl; 
    };
}

// Dus nu een class: roep run op uit de class commandparser (ik zou zeggen zonder using namespace std)
// 12-5-2026: Run structuur verbeterd.
// 12-5-2026: Run en initCommands Gesplitst: Dus nu de class in commandParser.h veranderen
void CommandParser::run(const unordered_map<string, unordered_map<string, vector<int>>>&index)
{
    // Hier roepen we nu dus initcommands op
    initCommands(index);

    //de functie
    string input;
    //Zolang waar
    while(true)
    {
        //variabele input, \n output
        
        cout<<"\n> ";
        // roep functie getline op: lees een volledige regel uit de input (toetsenbord)
        // en sla die op in input.
        // BELANGRIJK: getline heeft input van de gebruiker nodig 
        // pass by reference parameter
        getline(cin, input);

        //1) zoek de eerste spatie en sla resultaat op in space
        size_t space = input.find(' ');

        // ifstatement met ternary operator. dus als er geen spatie is 
        // geef de volledige input en anders geef de substring tot aan de eerste spatie
        string command = (space ==string::npos)?input:input.substr(0,space);
        // ifstatement met ternary operator. dus als er geen spatie is 
        // geef "" en anders geef vanaf eerste spatie?
        string query = (space==string::npos)?"":trim(input.substr(space + 1));
        
        //12-5-2026 Lowercase command maken: Neem de string van begin tot einde en schrijf
        // het resultaat terug in dezelfde string
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        //stop het programma indien we 'exit' typen
        if(command =="exit")
        {
            break;
        }
        // 11-5-2026 Zoek nu of de command staat in de hashmap
        // if(commands.find(command) != commands.end())
        // 11-5-2026: Meteen vernieuwd:
        auto it = commands.find(command);

        // Als command gevonden in hashmap: Dus als niet gelijk is aan commands.end())
        if(it != commands.end())
        {
        // Pak de lambda bij key command
        // Voer lambda uit met query
        // 11-5-2026 commands[command](query); Geef terug 2e command.
            it->second(query);
        }
        // Anders
        else
        {
            cout<< "Unknown Command" <<endl;
        }
    }
}


