#include  <iostream>
#include <fstream>
#include "Lexer.hpp"

using namespace std;
using namespace C_1;


/*
 * Analiza el archivo recibido como argumento e imprime cada token
 * con el formato: numero de token, lexema i.e. el que esta 
 * especificado como formato
 */
int main(int argc, char *argv[]) {
  
    if(argc < 2){
        cout << "Faltan argumentos" << endl;
        return EXIT_FAILURE;
    }

    
    // se abre el archivo y se usa 
    filebuf fb;
    fb.open(string(argv[1]), ios::in);
    istream in(&fb);
    Lexer lexer(&in);
    // se piden tokens hasta que yylex() regrese 0 (i.e. fin de archivo)
    int token = lexer.yylex();

    while(token != 0){
        //imprimimos en formato 12, int
        cout << token << ", " << lexer.YYText() << endl;
        token = lexer.yylex();
    }
    
    fb.close();    
    return 0;
}
