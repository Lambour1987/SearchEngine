//13-6-2026: Dit bestand bevat testqueries (dus de zoekopdrachten van de gebruiker)

// inclusief het headerbestand testqueries.h
#include "testqueries.h"
// iostream, fstream, vector, string
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <sstream>

//tokenizer, ASTBuilder en ASTevaluator
#include "../tokenizer/tokenizer.h"
#include "../queryParser/ASTBuilder.h"
#include "../queryEvaluator/ASTEvaluator.h"
#include "../utils/StringUtils.h"

using namespace std;

// functie runQueryTests met als parameters filename die niets teruggeeft
// 16-6-2026: dit: void runQueryTests(const string& filename, const unordered_map<string, unordered_set<string>>&index)
// gewijzig naar":
void runQueryTests(const string& filename, const unordered_map<string, unordered_map<string, vector<int>>>&index)
{
    // input file stream lees bestand
    ifstream file(filename);
    // als er geen bestand (file is)
    if(!file)
    {
        // geef aan dat je het testbestand niet kan openen
        cout<<"Kon testbestand niet openen: "<< filename << endl;
        return;
    }

    // maak een string query
    //15-6-2026: nieuw. Was: string query;
    string line;

    //15-6-2026: nieuw
    int passed = 0;
    int failed = 0;

    // Zolang er nog regels zijn, lees de volgende regel
    // file is fstream, query is de string: dus lees dit bestand regel voor regel
    while(getline(file, line))
    {
        // Als de query leeg is, ga door. Let op: dus niet: if(query!=empty())
        //15-6-2026: nieuw: als line leeg is, continue (was if(query.empty())
        if(line.empty())
        // cout, cout
        {
            continue;    
        }

        // ===========================
        // 1. split Query => Expected
        // ===========================

        //15-6-2026: hier dus nu pas de string query aanmaken
        string query;
        string expectedPart;

        //15-6-2026: zoek in de string naar de plek waar "=>" voorkomt (gebruik positie)
        // en bewaar deze in een variabele pos (gebruik size_t: voor lengtes)
        // Vergelijk evt met find en count? 
        size_t pos = line.find("=>");

        // als de positie niet gevonden is ga door: let op. pos is de variabele die
        // we zo genoemd hebben en npos is dus een constante (static value) in de string class
        if(pos == string::npos) continue;

        // roep de line functie op en pak de substring van 0 tot positie en sla die op in de string query
        // (moet dit dan geen auto zijn? of is het altijd een string?)
        query = line.substr(0,pos);

        // pak alles vanaf positie +2 tot einde. (Gebruik +2 omdat we "=>" hebben). sla op in string expected part
        expectedPart = line.substr(pos+2);

        // 15-6-26: Let op mogelijk iets anders hiervoor gebruiken. We hebben namelijk al een trim functie
        // in de commandparser staan. Nog doen
        // trim de basics: 
        // Zolang de query niet leeg is en de query op positie 0 gelijk is aan spatie, verwijder dan alles tussen 0 en 1?
        //15-6-2026: Deze kan dus weg while(!query.empty() && query[0]==' ') query.erase(0,1) en wordt:
        query = trim(query);
        // Zolang het verwachte deel niet leeg is en verwachte deel gelijk is aan spatie verwijder dan 1 karakter vanaf index 0
        //15-6-2026: En deze: while(!expectedPart.empty() && expectedPart[0]==' ') expectedPart.erase(0,1) wordt:
        expectedPart = trim(expectedPart);

        // ===========================
        // 2. parse expected results
        // ===========================

        // maak een set van strings genaamd expected aan
        // 17-6-2026: Gewzijgt naar een map:  unordered_set<string> expected;
        unordered_map<string, int> expected;
        // maak een stringstream aan van de string expectedPart en noem deze ss:
        // je verandert dan een string in iets dat zich gedraagt als een input stream.
        // Zonder stringstream: zelf splitten, indexen zoeken, substr gebruiken. Met stringstream:
        // automatisch tokeniseren op spaties, super clean en perfect voor test parsing
        stringstream ss(expectedPart);
        // maak een string genaamd doc
        string doc;

        // zolang ik iets kan uitlezen uit de stream, blijf doorgaan
        while(ss>>doc)
        {
            // insert doc in expected
            //17-6-2026 expected.insert(doc); Wordt nu
            expected[doc] = 1;
        }

        // ===========================
        // 3. run engine
        // ===========================

        cout << "\n====================\n";
        cout << "QUERY: " << query << endl;
    //we gaan iets doen wat fout kan gaan, als het fout gaat vangen we dat op
        try
        {
            // roep tokenizeQuery op met de query en sla het resultaat op in een vector van Token genaamd tokens
            vector<Token>tokens = tokenizeQuery(query);
            //15-6-2026: eruit:
            // cout
            // cout<< "TOKENS: ";
            // // voor t in tokens
            // for(const auto& t: tokens)
            //     // geef waarde van t    
            //     cout<< t.value <<endl;
            // // cout
            // cout<< endl;
            // maak een builder van het type ASTbuilder (dit stond oorspronkelijk in main)
            ASTBuilder builder;
            // cout
            ASTNode* root = builder.build(tokens);
            // // print root
            // cout<<"AST:\n";
            // // HIER INDEX DOORGEVEN?
            // cout << "AST:\n";
            // printAST(root);

            auto actual = evaluateAST(root, index);

            // ===========================
            // 4. compare
            // ===========================

            // als actueel gelijk is aan expected
            if(actual == expected)
            {
                // print PASS en verhoog passed
                cout << "PASS" << endl;
                passed++;
            }
            // anders
            else
            {
                // print fail
                cout << "FAIL" << endl;  
                // cout Expected
                cout << "EXPECTED: ";
                // voor alle d in expected print d
                for(const auto& d: expected)
                {
                // cout Actual
                    //17-6-2026 cout << d << " "; wordt
                    cout<<d.first<<" "<<d.second;
                }

                cout<< "\nATUAL: ";
                // voor alle d in actual print d
                for(auto& d: actual) 
                {
                    //17-6-2026: aangepast van cout<< d << " ";
                    cout<< d.first <<"->" << d.second;
                                    
                }

                cout << endl;
                // verhoog failed
                failed++;
            }
        }
        // vang standaardfout op in e
        catch(const exception& e)
        //cout
        {
            cout<<"ERROR: "<< e.what() << endl;

            //verhoog failed:
            failed ++;
        }
    }

    cout << "\n====================\n";
    cout << "TEST RESULTEN:\n";
    cout << "PASSED: " << passed << endl;
    cout << "FAILED: " << failed << endl;


}