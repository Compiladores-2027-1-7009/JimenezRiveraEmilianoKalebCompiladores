#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "Lexer.hpp"
#include "Symbols.hpp"
using namespace std;

namespace C_1
{
  /*
   * Analizador sintactico de descenso recursivo para C_1.
   * Hay una funcion por cada no terminal de la gramatica transformada
   * (Symbols.hpp) y todas usan el token de preanalisis guardado en token.
   */
  class Parser
  {

  private:
    Lexer *lexer;
    int token; // token de preanalisis (lookahead)

    // si el token actual es t pide el siguiente, si no reporta que se esperaba esperado
    void match(Token t, string esperado);

  public:
    Parser();
    Parser(Lexer *lexer);
    ~Parser();

    void programa(); //función del símbolo incial
    /**
     * Funciones del resto de NTs, las terminadas en P son las primas
     * que aparecieron al eliminar la recursividad izquierda y al factorizar
     */
    void declaraciones();
    void declaracionesP();
    void declaracion();
    void tipo();
    void lista_var();
    void lista_varP();
    void sentencias();
    void sentenciasP();
    void asignacion();
    void control();
    void expresion();
    void expresionP();
    void termino();
    void terminoP();
    void factor();

    Token eat();
    void error(string msg);
    void parse();
  };

}


#endif
