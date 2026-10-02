/*
 * Lineas de enteros separados por espacios que terminan en A o en B.
 * Si termina en A se imprime la suma de sus numeros,
 * si termina en B se imprime la linea tal cual.
 */
%{
#include <iostream>
#include <fstream>
#include <string>

/*no sabemos si la linea es A o B hasta llegar al final, entonces
  vamos guardando ambas cosas: la suma y el texto de la linea*/
long long suma = 0;
std::string linea = "";
%}

%option c++
%option noyywrap

entero -?[0-9]+

%%

{entero}  { suma += std::stoll(yytext); linea += yytext; }
[ \t]+    { linea += yytext; }
A         { std::cout << suma << std::endl; }
B         { std::cout << linea << yytext << std::endl; }
    /*al terminar la linea reiniciamos para la siguiente*/
\n        { suma = 0; linea = ""; }
.         { std::cout << "Caracter no valido: " << yytext << std::endl; }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Faltan argumentos: ./lineas_ab archivo" << std::endl;
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
