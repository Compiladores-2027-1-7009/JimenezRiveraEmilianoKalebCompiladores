/*
 * Reconoce literales binarias (0b seguido de al menos un 0 o 1)
 * y al final imprime la suma de todas en base 10.
 */
%{
#include <iostream>
#include <fstream>

long long suma = 0;
%}

%option c++
%option noyywrap

binario 0b[01]+

%%

{binario} {
    /*nos saltamos el prefijo 0b (posiciones 0 y 1) y convertimos a base 10*/
    long long valor = 0;
    for (int i = 2; i < yyleng; i++) {
        valor = valor * 2 + (yytext[i] - '0');
    }
    std::cout << "Literal binaria: " << yytext << " = " << valor << std::endl;
    suma += valor;
}
    /*todo lo demas se ignora*/
.|\n    { }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Faltan argumentos: ./binarios archivo" << std::endl;
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cout << "No se pudo abrir " << argv[1] << std::endl;
        return 1;
    }
    yyFlexLexer lexer(&entrada);
    lexer.yylex();
    std::cout << "Suma: " << suma << std::endl;
    return 0;
}
