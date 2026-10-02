/*
 * Se comporta como wc: cuenta saltos de linea, palabras y bytes.
 */
%{
#include <iostream>
#include <fstream>

int lineas = 0;
int palabras = 0;
int bytes = 0;
%}

%option c++
%option noyywrap

/*igual que wc, una palabra es todo lo que no sea espacio en blanco*/
blanco [ \t\r\v\f]

%%

\n              { lineas++; bytes++; }
[^ \t\r\v\f\n]+ { palabras++; bytes += yyleng; }
{blanco}        { bytes++; }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Faltan argumentos: ./mi_wc archivo" << std::endl;
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cout << "No se pudo abrir " << argv[1] << std::endl;
        return 1;
    }
    yyFlexLexer lexer(&entrada);
    lexer.yylex();
    /*mismo formato que wc: lineas palabras bytes archivo*/
    std::cout << lineas << " " << palabras << " " << bytes << " " << argv[1] << std::endl;
    return 0;
}
