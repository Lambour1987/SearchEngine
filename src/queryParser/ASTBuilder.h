//29-5-2026: 

//Voorkom dat headers vaker opgenomen worden
#pragma once

//Gebruik vector, stack
#include <vector>
#include <stack>

// ASTNode.h en tokenizer.h moeten meegenomen worden
#include "../tokenizer/tokenizer.h"
#include "../types/AST.h"

//class AST builder met public
//Hier maken we dus eigenlijk een function declaration (soort forward declartion maar dan voor functies, geen echte forward declartion)
class ASTBuilder
{
public: 
// Public: 
// functie declaratie van de AST builder. dus een functie build met als parameter token en als output een pointer naar een ASTNode.
// Een pointer omdat de AST dynamisch gebouwd wordt. Nodes worden met new aangemaakt en boomstructuur zijn verbonden met pointers.
    ASTNode* build(const std::vector<Token>&tokens);
private:
    // Private
    // Een functie precedence met parameter type en output een integer
    int precedence(TokenType type);

    // Een functie applyOperator met parameter nodes en ops en output void
    void applyOperator( std::stack<ASTNode*>&nodes, std::stack<TokenType>& ops);
};

// 14-6-2026: functie bestaat ook buiten de class
// bedoelt voor de test?
void printAST(ASTNode* node);