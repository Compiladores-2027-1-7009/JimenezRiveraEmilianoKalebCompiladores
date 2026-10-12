#include "headers/Parser.hpp"
#include <iostream>
#include <cstdlib>

namespace C_1
{
  Parser::Parser(/* args */)
  {
  }

  Parser::Parser(Lexer *lexer)
  {
    this->lexer = lexer;
  }

  Parser::~Parser()
  {
  }

  // programa -> declaraciones sentencias
  void Parser::programa(){
    declaraciones();
    sentencias();
  }

  /************************************************************************/
  /**                                                                    **/
  /**                       Funciones por cada NT                        **/
  /**                                                                    **/
  /************************************************************************/

  // declaraciones -> declaracion declaraciones'
  void Parser::declaraciones(){
    declaracion();
    declaracionesP();
  }

  // declaraciones' -> declaracion declaraciones' | ε
  void Parser::declaracionesP(){
    // si viene un tipo hay otra declaracion, si no ya empiezan las sentencias
    if(token == t_int || token == t_float){
      declaracion();
      declaracionesP();
    }
  }

  // declaracion -> tipo lista_var ;
  void Parser::declaracion(){
    tipo();
    lista_var();
    match(t_pyc, "';'");
  }

  // tipo -> int | float
  void Parser::tipo(){
    if(token == t_int || token == t_float)
      token = eat();
    else
      error("se esperaba un tipo (int o float)");
  }

  // lista_var -> identificador lista_var'
  void Parser::lista_var(){
    match(t_id, "un identificador");
    lista_varP();
  }

  // lista_var' -> , identificador lista_var' | ε
  void Parser::lista_varP(){
    if(token == t_coma){
      token = eat();
      match(t_id, "un identificador");
      lista_varP();
    }
  }

  // sentencias -> asignacion sentencias' | control
  void Parser::sentencias(){
    if(token == t_id){
      asignacion();
      sentenciasP();
    }
    else if(token == t_if || token == t_while)
      control();
    else
      error("se esperaba una sentencia (asignación, if o while)");
  }

  // sentencias' -> sentencias | ε
  void Parser::sentenciasP(){
    if(token == t_id || token == t_if || token == t_while)
      sentencias();
    // ε solo es valido si sigue algo de FOLLOW(sentencias') = { else, $ }
    else if(token != t_else && token != t_eof)
      error("se esperaba una sentencia, else o fin de archivo");
  }

  // asignacion -> identificador = expresion ;
  void Parser::asignacion(){
    match(t_id, "un identificador");
    match(t_asig, "'='");
    expresion();
    match(t_pyc, "';'");
  }

  // control -> if ( expresion ) sentencias else sentencias | while ( expresion ) sentencias
  void Parser::control(){
    if(token == t_if){
      token = eat();
      match(t_lpar, "'('");
      expresion();
      match(t_rpar, "')'");
      sentencias();
      match(t_else, "else");
      sentencias();
    }
    else if(token == t_while){
      token = eat();
      match(t_lpar, "'('");
      expresion();
      match(t_rpar, "')'");
      sentencias();
    }
    else
      error("se esperaba if o while");
  }

  // expresion -> termino expresion'
  void Parser::expresion(){
    termino();
    expresionP();
  }

  // expresion' -> + termino expresion' | - termino expresion' | ε
  void Parser::expresionP(){
    if(token == t_mas || token == t_menos){
      token = eat();
      termino();
      expresionP();
    }
  }

  // termino -> factor termino'
  void Parser::termino(){
    factor();
    terminoP();
  }

  // termino' -> * factor termino' | / factor termino' | ε
  void Parser::terminoP(){
    if(token == t_mul || token == t_div){
      token = eat();
      factor();
      terminoP();
    }
  }

  // factor -> ( expresion ) | identificador | numero
  void Parser::factor(){
    if(token == t_lpar){
      token = eat();
      expresion();
      match(t_rpar, "')'");
    }
    else if(token == t_id || token == t_num)
      token = eat();
    else
      error("se esperaba un identificador, un número o '('");
  }

  void Parser::match(Token t, string esperado){
    if(token == t)
      token = eat();
    else
      error("se esperaba " + esperado);
  }

  Token Parser::eat(){
    return (Token) lexer->yylex();
  }

  // al primer error se detiene el analisis, ya no tiene caso seguir
  void Parser::error(string msg){
    cout << "ERROR DE SINTAXIS " << msg << " en la línea "<< lexer->getLine() <<endl;
    cout << "La cadena no pertenece al lenguaje generado por la gramática" << endl;
    exit(EXIT_FAILURE);
  }

  void Parser::parse(){
    token = eat();
    programa();
    if(token == t_eof)
      cout << "La cadena es aceptada" << endl;
    else // sobro algo despues de las sentencias, p. ej. un else sin if
      error("se esperaba el fin del archivo");
  }

}
