java -jar c:/data/libraries/java/antlr-4.10.1-complete.jar -Dlanguage=Cpp  -o . VecMathLexer.g4
java -jar c:/data/libraries/java/antlr-4.10.1-complete.jar -Dlanguage=Cpp -listener -o . VecMathParser.g4
pause