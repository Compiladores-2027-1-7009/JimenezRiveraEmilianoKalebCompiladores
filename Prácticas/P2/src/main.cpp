#include <iostream>
#include <string>
#include <fstream>
#include "headers/Lexer.hpp"
#include "headers/Parser.hpp"

using namespace std;
using namespace C_1;

/*
 * Analiza sintacticamente el archivo recibido como argumento e imprime
 * si la cadena es aceptada o el primer error que se encuentre
 */
int main(int argc, char *argv[]){
  
  if(argc < 2){
    cout<<"Faltan argumentos "<<endl;
    return EXIT_FAILURE;
  }
  
  // se abre el archivo y se le pasa como flujo de entrada al lexer
  filebuf fb;
  if(!fb.open(string(argv[1]), ios::in)){
    cout<<"No se pudo abrir "<<argv[1]<<endl;
    return EXIT_FAILURE;
  }
  istream in(&fb);
  Lexer lexer(&in);
  // el parser le va pidiendo los tokens al lexer conforme los necesita
  Parser parser(&lexer);
  parser.parse();
  fb.close();
  return 0;

}
