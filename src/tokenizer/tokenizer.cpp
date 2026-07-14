// 12-5-2026: Tokenizer: Losse functie, scheiding van verantwoordelijkheden. Dit is de tweede tokenizer.
// bedoelt om zoekopdrachten te begrijpen: 'quotes', AND/ OR, haakjes, operators, command syntax.
// Hier: query tokenizer. Oude tokenizer: Document tokenizer.
// BELANGRIJK: hier geen dispatch table. Want dispatch table: ik kies 1 actie op basis van 1 key
// Hier: ik verwerk elke char afhankelijk van vorige state. Tokenizer is een finite state machine
// We kunnen het wel verbeteren met enums of switches of helper functions. 
// 13-5-2026: Goed genoeg en functioneel correct: Maar moeten nog upgrades in. bijv. case sensitivity
#include "tokenizer.h"
#include <sstream>
// t.b.v. whitespace
#include <cctype>

using namespace std;

// Functie TokenizeQuery met als output een vector van Tokens (struct token uit tokenizer.h) en
// als input een referentie naar een string input die we niet willen wijzigen
vector<Token> tokenizeQuery(const string& input)
{
    //Initialiseer een vector van Token genaamd tokens
    vector<Token> tokens;

    //Initialiseer een string genaamd current
    string current;

    //Initialiseer een boolean variabele genaamd inQuotes met startwaarde false;
    bool inQuotes = false;

    // Voor ieder karakter in de input: Eigenlijk voor iedere plek in de input (dus naast karakters ook spaties)
    // Dus de term karakter is een beetje misleidend
    for(char karakter:input)
    {
        // ===========================
        // 1)    QUOTES TOGGELEN 
        // ===========================
        // Als karakter gelijk is aan ' " ': Dan quotes toggelen
        if(karakter == '"')
        {
            //En als deze inQuotes bestaat
            if(inQuotes)
            {
                // Voeg toe aan tokens: PHRASE van de tokentype: Tokens is een vector dus 
                // we voegen van de struct TokenType een phrase en current toe
                // Gebruik {} Dat is uniform initialization: push back verwacht 1 object
                tokens.push_back({TokenType::PHRASE,current});
                //Delete Current
                current.clear();            
            }  

            // Vervolgens:
            // Toggle inQuotes == false naar inQuotes == true.
            inQuotes = !inQuotes;
            continue;
        }
        // ===========================
        // 2)    BINNEN QUOTES 
        // ===========================
        // Als karakter wel in quotes staat: dan alles opslaan, dan doorgaan
        if(inQuotes)
        {
            current += karakter;
            continue;
        }

        //7-6-2026: Parentheses toegevoegd
        // ===========================
        //3)    PARENTHESES
        // ===========================
        // Als karakter gelijk is aan openingshaakje (
        if(karakter =='(')
        {
            // Voeg nieuw token object LPAREN toe aan tokens met waarde openingshaakje
            tokens.push_back({TokenType::LPAREN,"("});
            // Ga door
            continue;
        }
        //Als karakter gelijk is aan sluitingshaakje
        if(karakter == ')')
        {
            // Duw rechterparen met waarde sluitingshaakje op tokens
            tokens.push_back({TokenType::RPAREN, ")"});
            // Ga door
            continue;
        }
        //7-6-2026: Whitespace nummer opgeschoven
        // ===========================
        //4)    WHITESPACE 
        // ===========================
        // Anders als dit karakter een whitespace is(spatie, tab, newline etc)
        if(isspace(karakter))
        {
            //En als current niet leeg is
            if(!current.empty())
            {
                //En als current gelijk is aan 'AND"
                if(current=="AND")
                {
                    //Voeg toe aan tokens met type AND en geef "AND"
                    tokens.push_back({TokenType::AND, "AND"});
                }
                //Als current niet gelijk is aan 'AND' maar aan 'OR"
                else if(current=="OR")
                {
                    //Voeg toe aan tokens met type OR en geef OR
                    tokens.push_back({TokenType::OR, "OR"});
                    //Anders
                }
                //4-6-2026: NOT is nieuw.
                else if(current=="NOT")
                {
                    tokens.push_back({TokenType::NOT, "NOT"});
                }
                else
                {
                    //Voeg WORD toe aan tokens met type current
                    tokens.push_back({TokenType::WORD,current});
                }
                //Maak current leeg
                current.clear();
            }
            continue;
        }
        // ===========================
        //4)    NORMAAL KARAKTER
        // ===========================
        else
        {
            //Current is current + c
            current += karakter;
        }
    }

    // =============================
    //        LAATSTE TOKEN
    // =============================
    // Laatste token (buiten for loop)
    // Als current niet leeg is (dus niet if(current !=empty))
    if(!current.empty())
    {
        // Als current gelijk is aan "AND"
        if(current == "AND")
        {
            //Voeg toe aan tokens: AND, AND
            tokens.push_back({TokenType::AND,"AND"});
        }
        // Anders als current gelijk is aan OR
        else if(current=="OR")
        {
            // Voeg toe aan tokens OR, OR
            tokens.push_back({TokenType::OR,"OR"});
        }
        //10-6-2026: Toegevoegd: 'NOT" want die stond er nog niet.
        else if(current=="NOT")
        {
            tokens.push_back({TokenType::NOT, "NOT"});
        }

        // Anders
        else
        {
            // Voeg toe aan tokens Word, current
            tokens.push_back({TokenType::WORD,current});
        }
        
    }
    return tokens;
}

