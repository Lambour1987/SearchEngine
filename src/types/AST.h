// 28-5-2026 AST implementeren

// Voorkomen dat header meerdere keren wordt meegenomen
#pragma once
// Includeer string
#include <string>
#include <unordered_map>
#include <vector>

// maak een enum class Nodetype met daarin TERM, AND, OR, NOT
// Class: alles is bij default private (bedoelt voor 'gedrag + encapsulation': objet met logica en bescherming).
// Wat voor soort Node zit er in je AST
//
//  == NodeType
//

//16-6-2026: vernieuwd
// ===============================
//          AST STRUCTURE
// ===============================
enum class NodeType
{
    TERM,   // TERM is een AST-node type: een leaf in de AST (zoekwoord of phrase.)
    AND,    // binary operator
    OR,     // binary operator
    NOT,    // unary operator
};

//
// == AST Node structuur ==
//
// Struct: Default is alles public: we gebruiken een struct omdat een ASTNode puur een
// datastructuur is (geen logica). Lijkt het meest op een gewone tree: n-ary/ binary tree.
// Maak een struct ASTNOde met daarin Nodetype type, string value, left en right child (noem ze gewoon left en right.)

struct ASTNode
{
    NodeType type;
    std::string value;
    ASTNode* left;
    ASTNode* right;
    // Maak een parameterized constructor: Als ik een ASTNode maak wil je optioneel type + value meegeven
    ASTNode(NodeType t, std::string v = "") : type(t), value(v), left(nullptr), right (nullptr) {}

};

// Verder:
// TokenType heeft ook AND, OR, NOT maar hier bij de AST gebruiken we die anders. 
// TokenType is de taal lezen (herkennen woorden), AST is de betekenis begrijpen (structuur bouwen)

//Uitleg constructor: 
// zonder een parameterized constructor zouden we moeten doen:
// ASTNode* n = new ASTNode();
// n->type = NodeType::TERM;
// n->value = "apple";
// n->left = nullptr;
// n->right = nullptr;
// Dat is veel boilerplate code, foutgevoelig en traag om te schrijven

// Met parameterized constructor doen we
// ASTNode* n = new ASTNode(NodeType::TERM, "apple");
// Compacter, consitent


// 16-6-2026: NIEUW
// ==============================
// SHARED INDEX TYPE
// ==============================

using InvertedIndex = std::unordered_map<std::string, std::unordered_map<std::string,std::vector<int>>>;
