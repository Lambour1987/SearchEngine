//13-5-2026 Query Parser. Nog geen AST 


#include "queryParser.h"
#include <stack>
#include <vector>

using namespace std;

// functie precedence met als input een referentie naar 'teken' die we niet wijzigen
// en een output int van static: Static: Alleen zichtbaar binnen dit bestand en niet in andere
// cpp files. Je kan er niet naar linken.
static int precedence(const Token&token)
{
    // switch cases maken. case And return 2, or 1 anders 0. 
    // 'teken' komt uit het struct token (zie tokenizer.h)
    switch (token.type)
    {
        case TokenType::AND: return 2;
        case TokenType::OR: return 1;
        default: return 0;
    }
}

// maak een statische functie isOperator met als input een referentie naar 'teken' die we niet wijzigen
static bool isOperator(const Token &token)
{
    //retourneer teken.type als Tokentype AND of OR is
    return token.type == TokenType::AND || token.type == TokenType::OR;
}

// Functie toPostfix met als input een referentie naar de vector tokens die we niet aan wijzigen en als output
// een vector Token. Ik denk alas constructor? een vector van token output en een stack van token operatorStack
vector<Token> toPostfix(const vector<Token>&tokens)
{
    // maak een vector van token output
    vector<Token>output;
    // maak een stack van tokens operationStack
    stack<Token>operationStack;
    // voor iedere referentie die we niet veranderen naar teken in tokens
    //27-5-2026: het woord 'teken' in 'token' verandert want het gaat hier om 'token' (PHRASE, WORD, AND, OR)
    for(const auto&token:tokens)
    {
        //8-6-2026: Als tokentype LPAREN is
        // ==========================
        // 1. LPAREN
        // ==========================
        if(token.type == TokenType::LPAREN)
        {
            //Push dan token op stack en ga door
            operationStack.push(token);
            continue;
        }
        // ===========================
        // 2. RPAREN
        // ===========================

        //8-6-2026: Als tokentype RPAREN is: Schijnbaar ook weer Shunting Yard aanpak
        if(token.type == TokenType::RPAREN)
        {
            // Zolang stack niet leeg is en stacktoptype is niet gelijk aan LPAREN
            while(!operationStack.empty() && operationStack.top().type != TokenType::LPAREN)
            {
                //Duw top operationStack in output en pop van stack 
                //Let op: Hier dus bij vector push_back en bij stack 'push'
                output.push_back(operationStack.top());
                operationStack.pop();
            // Als operationstack niet leeg is
            }
            if(!operationStack.empty()
            && operationStack.top().type== TokenType::LPAREN)
            //8-6-26: DIt weggehaald: en weer terug
            {
                // Pop dan van de opertionstack (je verwijdert '('
                operationStack.pop();
            }
            continue;
        }

        // ====================================
        // SAFETY GUARD: voorkomt dat ( ) ooit verder in flow komen
        // ====================================
        if(token.type == TokenType::LPAREN ||
           token.type == TokenType::RPAREN)
        {
            continue;
        }
         // =========================
        // 3. OPERANDS (WORDS / PHRASES)
        // =========================


        //Als het teken geen Operator is
        //waarom geen teken.isOperator()? ofzo
        if(!isOperator(token))
        {
            //voeg aan output vector het teken toe
            output.push_back(token);
            //ga door;
            continue;
        }

         // =========================
        // 4. OPERATORS (AND / OR)
        // =========================

        // Zolang de stack (operationStack) niet leeg is en de top gelijk is of groter aan 
        // aan de huidige operator. Dan van de stack af halen en in output zetten
        
        // Zolang de stack niet leeg is
        while (!operationStack.empty() 
        // En bovenste element van de operationStack een operator is (roep functie op)
        && isOperator(operationStack.top())
        //8-6-2026 UItsluiten van LPAREN
        && operationStack.top().type !=TokenType::LPAREN
        // En de top van de operationStack groter of gelijk is aan het huidige teken
        // "De waarde" van AND is hoger dan OR. Vandaar.
        && precedence(operationStack.top())>= precedence(token))
        {
            //Voeg de top operationStack toe aan aan output
            output.push_back(operationStack.top());
            // Haal top eraf
            operationStack.pop();
        }
        // Push op de operationstack het teken
        // Push je huidige teken er weer op
        operationStack.push(token);
    }

    // =========================
    // 5. DRAIN STACK
    // =========================

    // Zolang operationStack niet leeg is
    while(!operationStack.empty())
    {
        // Voeg aan de output de top van operation stack toe
        //8-6-2026: if statement toegevoegd
        if(operationStack.top().type !=TokenType::LPAREN)
        {
            output.push_back(operationStack.top());
        }
        // Haal top eraf
        operationStack.pop();
    }
    //Retourneer output
    return output;
}