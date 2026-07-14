//4-6-2026 ASTEvaluator

#include <iostream>
#include <unordered_set>
#include <unordered_map>

// Voeg ASTEvaluator.h toe 
#include "ASTEvaluator.h"
// begin weer met de functie evaluateAST
// (parameters node en index). Als output een set van strings.
// gebruik weer die alias Inverted index
std::unordered_map<std::string, int> evaluateAST(ASTNode* node, const InvertedIndex& index)
{
    // Als er geen node is return .
    // Alternatief: if(!node)
    if(node == nullptr)
    {
        //17-6-2026: van return {}; naar
        return std::unordered_map<std::string,int>{};
    }

    // We werken hier met if statements: dat is hier logisch omdat je werkt met een node->type
    // Dat is puur branching logic. Kan wel met switch cases maar in AST evaluators if. Met commands
    // kan ook maar dat is overkill? Daar is AST te klein voor nu.
    
    //
    //  =====  TERM =====
    //
    // Als node type TERM is 
    // Vergelijk dit weer met type == NodeType::NOT en postfixToken.type == TokenType::WORD
    if(node->type==NodeType::TERM)
    {
        //4-6-2026: Test
        std::cout << "TERM VALUE: " << node->value << std::endl;

        //9-6-2026: Toegevoegd:
        std::cout << "LOOKUP KEY: [" << node->value << "]" << std::endl;

        //9-6-2026: Toegevoegd:
        std::cout << "NODE VALUE RAW: [" << node->value << "] size=" << node->value.size() << std::endl;

        // Zoek in de index de waarde op van de node en stop deze in een variabele it (auto)
        auto it = index.find(node->value);
        // Vervolgens: als it niet gevonden is: end is de iterator die 1 positie voorbij het laatste element
        // staat. 'End' kan je gebruiken omdat index een set is
        //9-6-2026: Ook weer toegevoegd.
        if(it == index.end())
        {
            return {};
        }

        //16-6-2026: omdat we de index hebben gewijzigd moeten we e.e.a. aanpassen
        //17-6-2026: dus dit wordt ook een map
        std::unordered_map<std::string, int>result;

            // weggehaald:
            // std::cout << "DOCS FOR " << node->value << ": ";
        for(const auto& pair : it->second)
        {
            const std::string& doc = pair.first;
            const std::vector<int>& positions = pair.second;
            
            std::cout << "DOC" << doc << " POSITIONS: ";

            for(int p: positions)
            {
                std::cout << p << " ";
            }
            // std::cout << std::endl;
            // return it->second;
            std::cout<<std::endl;

            //17-6-2026: van result.insert(doc); naar: (omdat we map ipv set gebruiken)
            //17-6-2026 nu gewijzigd naar result[doc] += positions.size();
            result[doc] = 1;
        }   
        // Return leeg
        // 16-6-2026: moet zijn return resultaat
        return result;
    }

    //
    // =====   AND =====
    //
    // Als node type AND is
    if(node->type==NodeType::AND)
    {
        //roep evaluateAST functie op met parameter linkernode en index en sla op in left
        auto left = evaluateAST(node->left, index);
        //roep evaluateAST functie op met parameter rechternode en index en sla op in right
        auto right = evaluateAST(node->right, index);
        //maak een set van strings genaamd resultaat
        //17-6-2026: wordt een map: std::unordered_set<std::string> result;
        std::unordered_map<std::string,int> result;
        //voor ieder document in left
        //17-6-2026: logica aangepast t.b.v. map. was: for(const auto&doc:left)
        for(const auto&[doc, score]:left)
        {
            // Als document doc ook in de rechter verzameling zit, voeg het dan toe aan het resultaat.
            // 17-6-2026: ook aangepast if(right.count(doc)) 
            if(right.count(doc)) 
            {
                //17-6-2026: gewijzigd van result[doc] = left[doc]+right[doc]; naar
                result[doc] = 1;
            }
        }
        // retourneer resultaat
        return result;
    }
    //
    // ===== OR =====
    //
    // Als node type OR is
    if (node->type==NodeType::OR)
    {
        // roep evaluateAST functie op met parameter linkernode en index en sla op in left
        auto left = evaluateAST(node->left, index);
        // roep evaluateAST functie op met parameter rechternode en index en sla op in right
        auto right = evaluateAST(node->right, index);
        // alle elementen uit van de rechterset in de linkerset
        //17-6-2026: Dit is aangepast naar een forloop: left.insert(right.begin(), right.end());
        for(const auto& [doc, score]:right)
        {
            //17-6-2026: gewzijgid van left[doc] += score; naar
            left[doc] = 1;
        }
        // returneer links
        return left;
    }
    //
    // ===== NOT =====
    //
    // als Node type NOT is
    if (node->type==NodeType::NOT)
    {
        // roep evaluateAST functie op en parameter met parameter rechternode en index en sla op in child
        // 4-6-2026: auto child = evaluateAST(node->left, index); wordt right. Dit heeft te maken met dat
        // NOT altijd maar 1 branch heeft, namelijk zijn tegenovergestelde.
        auto child = evaluateAST(node->right, index);
        // maak een set van strings voor allDocs
        //17-6-2026: van std::unordered_set<std::string> allDocs; aangepast naar:
        std::unordered_map<std::string, int> allDocs;
        // voor linker en rechterdoc in de index
        for(const auto& [word,docs]:index)
        {
            //Voeg alle documenten toe aan allDocs
            //4-6-2026: Was eerst: allDocs.insert(allDocs.begin(), allDocs.end()); gewijzigd naar:
            for(const auto&doc: docs)
            {
                //17-6-2026: van allDocs.insert(doc.first);
                //17-6-2026: van allDocs[doc.first] = 0;
                allDocs[doc.first] = 1;

            }
        }
        // voor alle documenten in child
        for(const auto& doc: child)
        {
            // Verwijder alle documenten uit allDocs
            //17-6-2026: gewzijgd van allDocs.erase(doc);
            allDocs.erase(doc.first);
        }
        // retourneer allDocs
        return allDocs;
    }
    // return {};
    return {};
}



        
    
