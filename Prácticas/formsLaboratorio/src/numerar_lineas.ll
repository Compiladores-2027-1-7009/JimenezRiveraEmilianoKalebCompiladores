/*
 * Imprime la entrada con el numero de linea al principio de cada una.
 */
%{
#include <iostream>
#include <fstream>

/*contador de lineas, empieza en 1 porque la primera linea es la 1*/
int linea = 1;
%}

%option c++
%option noyywrap

%%

    /*una linea completa con su salto de linea (tambien las lineas vacias)*/
.*\n    { std::cout << linea << " " << yytext; linea++; }
    /*la ultima linea del archivo puede no terminar en salto de linea*/
.+      { std::cout << linea << " " << yytext << std::endl; linea++; }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Faltan argumentos: ./numerar archivo" << std::endl;
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cout << "No se pudo abrir " << argv[1] << std::endl;
        return 1;
    }
    yyFlexLexer lexer(&entrada);
    lexer.yylex();
    return 0;
}
