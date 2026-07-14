// 29-5-2026 ASTBuilder.cpp
// 4-6-2026: MIsschien ook nog een aparte map maken voor ASTBuilder en ASTEvaluator? nu vallen ze resp onder queryParser en queryEvaluator

// Neem ASTbuilder en stack mee
#include "ASTBuilder.h"
#include <stack>

//
// == PRECEDENCE ==
//

// Gebruik precedence functie
// precedence functie (uit class ASTBuilder) met als parameter een type en integer als output
// 3-6-2026: Let op hier dus geen const, ref
// VRAAG: Zou je deze functie dan ook weer anders kunnen doen. Niet met cases maar met (commands?)
// En ipf if if if kan je dus ook if else if, else if else doen.
// 4-6-2026: Maar inderdaad gedaan: switch case. Ipv if's.
int ASTBuilder::precedence(TokenType type)
{
    switch(type)
    {
        case TokenType::NOT: return 3;
        case TokenType::AND: return 2;
        case TokenType::OR: return 1;
        default: return 0;
    }

}

//
// == APPLY OPERATOR ==
//

// FUNCTIE applyOperator van de class ASTbuilder met parameter nodes (stack met pointers naar AST nodes) referentie naar nodes,
// stack TokenType en referentie naar ops. Geen return value;
// nodes en ops komen uit Build()
void ASTBuilder::applyOperator(
        std::stack<ASTNode*>& nodes, 
        std::stack<TokenType>& ops)
{
    
    //4-6-2026: opnieuw een safetycheck in de code gevoerd
    if (ops.empty())
        return;
    
    // kopieer de stackwaarde van de stack ops naar de variabele op (maak nieuwe aan)
    // Het datatype van de stackwaarde van ops is dus een std::stack<TokenType> ops; dus 
    // dan moet de variabele ook een Tokentype worden. Dus type enum class Tokentype: WORD, PHRASE, AND, OR, NOT
    TokenType op = ops.top();
    // haal vervolgens de top van de stack eraf
    ops.pop();

    //8-6-2026 Toegevoegd
    // Wen weer weggehaald
    // if(op == TokenType::LPAREN)
    // {
    //     return;
    // }

    // ===== NOT (unary opeartor) =====
    // Als de variabele op gelijk is aan NOT (van class TokenType)
    if(op == TokenType::NOT)
    {
        // Maak een nieuw ASTNode-object op de heap en het type op NOT
        // 3-6: Na checken: Chat zegt doe eerst safetycheck en daarna nieuwe node maken ipv andersom
        // SafetyCheck: Als nodes leeg zijn retourneer
        if(nodes.empty())return;
        //4-6-2026: Let op direct doorgaan met dit (geen elif)
        ASTNode* node = new ASTNode(NodeType::NOT);
    
        // Pak de top van de nodes stack en ze die als rechter kind van deze node
        // Dus kopieer de pointer adres.
        // 3-6-2026: Check nog even: ik die eerst ASTNode->right maar het moet zijn node->right
        node->right = nodes.top(); 
        // Haal de top van de stack nodes af
        nodes.pop();
        // Push de node terug op de nodes stack: 
        // Zet de nieuwe AST-subtree terug op de stack
        nodes.push(node);
        // retourneer
        return;
    }

    // ===== AND/ OR (binary operators) ===== 
    // Safety check
    // Als de omvang van de nodes kleiner is dan 2 return
    if(nodes.size()<2) return;
    
    // Pak de top van de stack nodes af en zet die als rechter kind van deze node
    // BELANGRIJK: Let dus op: Hier gebruiken we ASTNode* Right en boven Node->right
    ASTNode*right = nodes.top();
    // Haal de top van de stack nodes af
    nodes.pop();
    // Pak de top van de stack nodes af en zet die als linker kind van deze node
    ASTNode*left = nodes.top();
    // Haal de top van de stack nodes af
    nodes.pop();
    // Maak een pointer genaamd node en laat hem voorlopig naar niks wijzen.
    // Later wijzen we hem naar een ASTNode op de heap.
    ASTNode* node = nullptr;
    // Als stack 'op' gelijk is aan AND (van class TokenType)
    // BELANGRIJK: Let op we hebben dus een AST node die wijst naar niks. Maar deze is
    // wel in scope. Dus straks hoeven we niet neer te zetten ASTNode * node = new ASTNode(TokenType::AND)
    // maar kunnen we volstaan met 'node = new ASTNode(TokenType::AND)'.
    // BELANGRIJK: Check nou evven hoe het met die brackets {} zit.
    if(op == TokenType::AND)
    {
        // Maak een nieuw ASTNode-object op de heap en het type op AND
        node = new ASTNode(NodeType::AND);
    }
    // Anders als stack 'op' gelijk is aan OR
    else if(op == TokenType::OR)
    {
        // Maak een nieuw ASTNode-object op de heap en het type op OR
        node = new ASTNode(NodeType::OR);
    }
    else
    {
        return;
    }
    // Koppel de twee kinederen aan deze AST node:
    // Verschuif linkerkind naar left: het linkerkind krijgt het adres van left
    node->left = left;
    // Verschuif rechterkind naar recht
    node->right = right;
    //push node weer op nodes stack
    nodes.push(node);
};     

//
// == BUILD AST ==
//
// functie build van de class ASTbuilder met als parameter een vector tokens (const) en als output ASTNode*
// BELANGRIJK: Nog bekijken: ASTBUilder gebruikt vaak tijdelijke nullptr pointers. Dit is een design patroon
// Dus dit weer opnieuw?
// bouw weer een stack nodes van pointers naar ASTNodes en een stack ops van type TokenType
ASTNode* ASTBuilder::build(const std::vector<Token>& tokens)
{
    std::stack<ASTNode*>nodes;
    std::stack<TokenType>ops;

    //voor ieder token in tokens (const van type Token):
    //Let op Token is dus het datatype (en niet een class), token het element en tokens de vector.
    for(const Token& token : tokens)
    {
        // ===== TERMS (words/ phrases) =====
        // als het tokentype gelijk is aan WORD (van class tokentype) of aan PHRASE
        if(token.type==TokenType::WORD
            ||token.type==TokenType::PHRASE)
        {
            // Maak een nieuw ASTNode-object op de heap en het type op TERM en de waarde token.value
            ASTNode* node = new ASTNode(NodeType::TERM, token.value);
            // push de node op stack nodes
            nodes.push(node);
        }
        // 8-6-2026: Erbij gezet
        // ====  2. LPAREN =====
        //
        else if(token.type == TokenType::LPAREN)
        {
            ops.push(token.type);
        }

        // ===== NOT =====
        // anders als tokentype gelijk is aan NOT
        else if (token.type==TokenType::NOT)
        {
        // Push dan NOT op de stack 'ops'
        // 8-6-2026: Dit wegghaald: en weer neergezet: ops.push(TokenType::NOT);
        ops.push(TokenType::NOT);
        //Dit weer weggehaald: applyOperator(nodes, ops);

        }
        // Erbij gezet en wee rweggehaald
        // ===== RPAREN ====
        // RPAREN: Tekst er nog bij schrijven
        else if(token.type == TokenType::RPAREN)
        {
            // 12-6-2026:BELANGRIJK: Moet niet zijn RPAREN? En precedence hier belangrijk?
            while(!ops.empty() && ops.top() != TokenType::LPAREN)
            {
                applyOperator(nodes, ops);
            }

            if(!ops.empty() && ops.top() == TokenType::LPAREN)
            {
                ops.pop();
            }
        }
        // ===== AND / OR =====
        // anders als tokentype gelijk is aan AND of OR
        else if (token.type == TokenType::AND || token.type == TokenType::OR)
        {
            // Let op dus hier weer een while binnen een if
            // Zolang er operators op de stack staan en de bovenste operator een hogere of gelijke prioriteit
            // heeft dan de huidige operator (denk dus: niet leeg = zolang er nog iets op de stack staat)
            // Dus geef het bovenste element van de stack en beoordeel hoe belangrijk die is (precedence)
            while(!ops.empty() 
            && ops.top() != TokenType::LPAREN
            && precedence(ops.top()) >= precedence(token.type))
            {
            // Roep dan de functie applyOperator op en geef nodes en ops mee als argumenten
                applyOperator(nodes, ops);
            }
            // Push het tokentype op de stack ops
            ops.push(token.type);
        //6-6-2026: moet hier niet ergens 'else' staan?
        }
    }

    //11-6-2026 BELANGRIJK: LET OP JE GAAT PAS NAAR FLUSH ALS JE ALLE TOKEN BINNEN TOKENS 
    // HEBT GEHAD. DUS NIET DIRECT NA EEN ELSE IF AND OF OR.

    //
    // ===== Flush Remaining Operators =====
    //
    // zolang er nog operators op de stack staan
    while(!ops.empty())
    {
        //8-6-2026: Erbij gezet
        if(ops.top() == TokenType::LPAREN)
        {
            ops.pop();
            continue;
        }
        // Roep dan de functie applyOperator op en geef nodes en ops mee als argumenten
        applyOperator(nodes, ops);
    }
    // return root van AST (dat is dus de top van de stack)
    // 4-6-2026: Fix erbij gedaan: want als input leeg is dan crasht die: je wilt top() teruggeven
    // maar zonder top kan dat niet.
    if(nodes.empty())
    {
        return nullptr;
    }

    return nodes.top();
}