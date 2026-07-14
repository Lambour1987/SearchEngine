:: Dit is een comment in .bat file
:: 23-6-2026: in dit bestand moet de runbat file komen:
:: hierin alle cpp bestanden die hij moet compileren
:: 24-6-2026: dit is een windows scripting taal.Het is een command script taal.

::24-6-2026: Nieuw erbij gezet
@echo on
echo ML: RUN.BAT STARTED
pause

:: 26-6-2026: Zorg dat basisinstellingen niet dubbel worden getoond
@echo off
:: Maakt tijdelijke omgeving
setlocal


::Print tekst in terminal
echo ==========================
echo Building SearchEngine
echo ==========================

g++ -std=c++17 ^
src/main/main.cpp ^
src/fileLoader/fileLoader.cpp ^
src/indexer/indexer.cpp ^
src/search/search.cpp ^
src/queryParser/queryParser.cpp ^
src/queryParser/ASTBuilder.cpp ^
src/queryEvaluator/queryEvaluator.cpp ^
src/queryEvaluator/ASTEvaluator.cpp ^
src/tokenizer/tokenizer.cpp ^
src/commandparser/commandParser.cpp ^
src/test_queries/testqueries.cpp ^
-o searchengine.exe


::Errorcheck: wat als compile fout gaat.
if %errorlevel% neq 0 (
    echo ❌ ML: Er is een compile error
    pause
    exit /b
)

:: Als de build succesvol is.
echo ✅ BUILD SUCCESSFUL
echo Running program...

searchengine.exe

:: pause: wacht tot een gebruiker op de toets drukt: zonder pause zal het exe bestand
:: opstarten en meteen weer afsluiten. Nu wacht het programma tot de gebruiker een toets indrukt
pause