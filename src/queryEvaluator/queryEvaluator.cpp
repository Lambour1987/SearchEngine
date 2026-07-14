// 13-5-2026
// 15-5-2026: Waarom is nou bijv index niet geimporteerd maar herkent C++ het wel?
// Importeren Token library?


#include <stack>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "../queryParser/queryParser.h"

using namespace std;

// Functie evaluatePostfix die als input een vector met referenties die niet te wijzigen zijn naar postfix
// een hashmap van strings en vector van integers referentie naar index die we niet mogen wijzigen. Als
// output een vector van integers.
// 18-5-2026: gewijzigd van een als output: een vector van integers naar een set van strings, omdat de zoekmachine
// niet meer uitgaan van document IDs maar van documentanamen.
unordered_set<string> evaluatePostfix(const vector<Token>& postfix, const unordered_map<string, unordered_set<string>>&index)
{
    // maak een stack van vectoren van integers (noem hem stack): stack<vector<int>> St.
    // 18-5-2026: Dit gewijzigd naar een unordered_set<string>
    stack<unordered_set<string>> St;

    //28-5-2026: Unordered_set van strings voor alle files
    unordered_set<string> allFiles;
    
    //28-5-2026: Vul dit deze set result. Voor alle woorden, files in de index
    for(const auto& [word, files]:index)
    {
        allFiles.insert(files.begin(),files.end());
    }

    // voor elk postfixToken in in postfix (noem even anders want we hebben al een 'token')
    // deze variabele heeft daar niets mee te maken
    for(const auto& postfixToken:postfix)
    {
        //Als het tokentype gelijk is aan WORD (uit de struct)
        if(postfixToken.type == TokenType::WORD)
        {
            // push de documenten op de stack waarin we het token value vinden: dus bijv index["apple"] = [1,3,5]
            // Dus apple komt voor in document 1,3,5 en die zetten we op de stack
            // token.value is dus gewoon de tekst.
            // 18-5-2026: Dit St.push(index.at(postfixToken.value)); vervangen door het geheel (auto it, if else)

            auto it = index.find(postfixToken.value);

            if (it != index.end())
            {
                St.push(it->second);
            }
            else
            {
                St.push({});
            }


        }
        // Anders als tokentype gelijk is aan 'AND'
        else if(postfixToken.type == TokenType::AND)
        {
            //18-5-2026: Dit erbij gezet: Als de stack namelijk leeg is crasht die.
            // Gebruik <2: omdat je postfix evaluator met een stack werkt: je hebt 2 operanderen.
            // Je hebt altijd 2 woorden waar je AND/OR op moet doen. Bijv. apple AND banana.
            if(St.size() <2)
            {
                return {};
            }

            // Haal de twee laatste indices van waarden van de stack om daar AND of OR op toe te passen:
            // haal waarde van stack uit de top en stop deze in een variabele right (auto datatype)
            auto right = St.top(); St.pop();
            // haal waarde van stack uit de top en stop deze in een variabele left (autodatatype)
            auto left = St.top(); St.pop();

            // maak een vector van integers resultaat
            //18-5-2026: wordt een unordered set. en daarmee wordt pushback ook gewijzigd naar insert.
            unordered_set<string> result;

            // voor ieder document in links
            for (const auto& doc:left)
            {
                // Zoek of doc voorkomt in de vector right. Niet gevonden. Is iterator einde.
                // 18-5-2026: Gewijzigd van (find(right.begin(),right.end(),doc) != right.end()) naar:
                // 28-5-2026: weer teruggewijzigd van if(right.count(doc)) naar:
                if (right.find(doc) != right.end())
                {
                // Voeg doc toe aan het resultaat
                //
                    result.insert(doc);
                }
            }
            // Voeg toe aan stack
            St.push(result);
        }
        // Anders. Dan is het tokentype OR
        else if (postfixToken.type == TokenType::OR)
        {
            // maak een variable right en stop daarin de stack top. Haal daarna het bovenste element van de stack
            auto right = St.top(); St.pop();
            // maak daarna een variabele left en stop daarin de stack top. Haal daarna het bovenste element van de stack
            auto left = St.top(); St.pop();
            // maak een vector van integers genaamd left
            unordered_set<string> result = left;
        // Voor ieder document in right
            for(const auto& doc:right)
            {
                // Als doc niet in resultaat staat, voeg toe aan het resultaat?
                if(find(result.begin(), result.end(),doc)==result.end())
                // Dus == result.end() is hetzelfde als. Dus als het niet in het
                // resultaat staat. Voeg toe aan resultaat. Hashset is misschien makkelijker hier
                // 15-5-2026: Als optimalisatie later hashset toevoegen
                    result.insert(doc);
            }
            // resultaat toevoegen aan stack
            St.push(result);
        }
        else if (postfixToken.type == TokenType::NOT)
        {
            //28-5-2026: Als stack leeg is return {}
            if(St.empty())
            {
                return {};
            }
            //28-5-26: Voeg stacktop toe aan auto A, vervolges pop je van de stack
            auto A = St.top();
            St.pop();

            //Initialiseer een set<strings> result
            unordered_set<string> result;
            //Voor iedere file in alle Files
            for(const auto& file:allFiles)
            {
                //Zoek in A naar de file. Als NIET gevonden
                //BELANGRIJK: Dus A.end() betekent als niet gevonden.
                if(A.find(file) == A.end())
                {
                    //Voeg file toe aan resultaat
                    result.insert(file);
                }
            }
        
            // Voeg resultaat toe aan stack.
            St.push(result);
        }
        
    }
    // Return de stacktop
    if (St.empty())
        return {};

    return St.top();
}