/*
 * Lee un archivo con palabras en minusculas y escribe otro archivo
 * con las mismas palabras en mayusculas.
 */
%{
#include <iostream>
#include <fstream>

/*archivo donde se escribe el resultado*/
std::ofstream salida;
%}

%option c++
%option noyywrap

%%

    /*una palabra es lo que queda entre espacios*/
[^ \t\n]+ {
    for (int i = 0; i < yyleng; i++) {
        char c = yytext[i];
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        salida << c;
    }
}
    /*los espacios y saltos de linea se copian tal cual*/
[ \t\n]+  { salida << yytext; }

%%

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Faltan argumentos: ./mayusculas entrada salida" << std::endl;
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cout << "No se pudo abrir " << argv[1] << std::endl;
        return 1;
    }
    salida.open(argv[2]);
    yyFlexLexer lexer(&entrada);
    lexer.yylex();
    salida.close();
    return 0;
}
