/*
 * Analizador lexico para el lenguaje C_1.
 * Regresa al parser, por cada lexema, el token de la enumeracion Token
 * definida en headers/Symbols.hpp y lleva la cuenta de las lineas para los errores.
 */
%{
#include <iostream>
#include <string>
using namespace std;

#include "headers/Symbols.hpp"
#include "headers/Lexer.hpp"

/*linea actual de la entrada, se usa en los mensajes de error*/
int line=1;
%}

%option c++
/*el codigo generado se escribe en Lexer.cpp en lugar de lex.yy.cc*/
%option outfile="Lexer.cpp"
/*las acciones lexicas se vuelven el cuerpo de Lexer::yylex()*/
%option yyclass="Lexer"

DIG [0-9]
letra [a-zA-Z]
/*enteros y flotantes, con parte decimal y exponente opcionales, p. ej. 23 o 1.2e6*/
numero {DIG}+(\.{DIG}+)?([eE][+-]?{DIG}+)?
/*empiezan con letra o guion bajo, seguidos de letras, digitos o guiones bajos*/
ids ({letra}|_)({letra}|{DIG}|_)*

%%

[ \t\r]+ {}
"\n" 	 {line++;}
    /*palabras reservadas: van antes de {ids} para ganar el empate de longitud*/
"int"    { return t_int; }
"float"  { return t_float; }
"if"     { return t_if; }
"else"   { return t_else; }
"while"  { return t_while; }
{numero} { return t_num; }
{ids}    { return t_id; }
    /*simbolos de puntuacion y asignacion*/
","      { return t_coma; }
";"      { return t_pyc; }
"="      { return t_asig; }
"("      { return t_lpar; }
")"      { return t_rpar; }
    /*operadores aritmeticos*/
"+"      { return t_mas; }
"-"      { return t_menos; }
"*"      { return t_mul; }
"/"      { return t_div; }
    /*sin esta regla yylex() regresaria 0 (primero) al terminar el archivo*/
<<EOF>>  { return t_eof; }
    /*cualquier otro caracter: se reporta y se le avisa al parser con t_error*/
.    	 { cout << "ERROR LEXICO " << yytext << " en la línea " << line << endl; return t_error; }

%%

/*al llegar al fin de la entrada no hay mas archivos por leer*/
int yyFlexLexer::yywrap(){
    return 1;
}

int Lexer::getLine()
{
    return line;
}
