# SearchEngine

## Over dit project

SearchEngine is een persoonlijk C++-project waarin ik een eigen zoekmachine ontwikkel die zich geleidelijk uitbreidt richting een eenvoudige interpreter voor zoekopdrachten.

Het doel van dit project is om beter te begrijpen hoe zoekmachines, parsers en interpreters werken. Hierbij ligt de focus op het verwerken van data, het interpreteren van zoekopdrachten en het opbouwen van een flexibele architectuur waarin nieuwe functionaliteit toegevoegd kan worden.

Het project bevindt zich nog in ontwikkeling en wordt stap voor stap uitgebreid.

> 🚧 **Status:** In ontwikkeling

---

## Huidige functionaliteit

Op dit moment bevat het project onder andere:

- Document indexing
- Tokenizing van documenten
- Tokenizing van zoekopdrachten
- Opbouwen van een Abstract Syntax Tree (AST)
- Evalueren van AST-expressies
- Uitvoeren van zoekqueries op basis van de opgebouwde structuur
- Een eenvoudig ranking systeem voor het bepalen van de relevantie van resultaten
- Modulaire opbouw waarbij de verschillende onderdelen los ontwikkeld en getest kunnen worden

---

## Architectuur

De huidige verwerking verloopt globaal volgens:

```
Documenten
    │
    ▼
Tokenizer
    │
    ▼
Indexer
    │
    ▼
Zoekopdracht
    │
    ▼
Query Tokenizer
    │
    ▼
AST Builder
    │
    ▼
AST Evaluator
    │
    ▼
Ranking
    │
    ▼
Zoekresultaten
```
De architectuur en gegevensstroom van het project zijn verder uitgewerkt in een diagram gemaakt met **draw.io**.
**
De publieke versie van dit diagram bevat de uitgewerkte structuur van het systeem. Persoonlijke ontwikkelnotities, ideeën en tijdelijke opmerkingen zijn verwijderd om de documentatie overzichtelijk te houden.
---**

## Toekomstige ontwikkeling

De oorspronkelijke focus van dit project ligt op een zoekmachine. De verdere ontwikkeling gaat echter richting een 
eenvoudige interpreter voor zoekopdrachten.

Geplande uitbreidingen:

- Complexere query-expressies
- Meer zoekoperatoren
- Uitbreiden van de query-taal
- Verbeteren van ranking-algoritmen
- Optimaliseren van de indexstructuur
- Mogelijkheden voor gebruikers om complexere opdrachten te formuleren

---

## Gebruikte technieken

- C++
- CMake
- STL (Standard Template Library)
- Datastructuren en algoritmen
- Parsing technieken
- Abstract Syntax Trees (AST)
- Visual Studio 2022

---

## Leerdoelen

Met dit project wil ik ervaring opdoen met:

- Modern C++
- Datastructuren en algoritmen
- Parsers en interpreters
- Compiler- en interpreterconcepten
- Information Retrieval
- Softwarearchitectuur
- Het ontwerpen van grotere softwareprojecten
