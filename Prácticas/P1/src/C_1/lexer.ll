/*
 * Analizador lexico para el lenguaje C_1.
 * Reconoce los terminales definidos en tokens.hpp y regresa, por cada lexema,
 * el numero de token correspondiente; el lexema queda disponible en yytext.
 */
%{
#include <iostream>
#include <string>
using namespace std;

#include "tokens.hpp"
#include "Lexer.hpp"

%}

%option c++
/*el codigo generado se escribe en Lexer.cpp en lugar de lex.yy.cc*/
%option outfile="Lexer.cpp"
/*las acciones lexicas se vuelven el cuerpo de C_1::Lexer::yylex()*/
%option yyclass="C_1::Lexer"
/*las palabras reservadas se reconocen sin importar mayusculas o minusculas*/
%option case-insensitive


DIG [0-9]
letra [a-zA-Z]
/*enteros y flotantes, con parte decimal y exponente opcionales, p. ej. 12345 o 1.2e6*/
numero {DIG}+(\.{DIG}+)?([eE][+-]?{DIG}+)?
/*empiezan con letra o guion bajo, seguidos de letras, digitos o guiones bajos*/
ids ({letra}|_)({letra}|{DIG}|_)*
espacio [ \t\n\r]

%%


{espacio} {/*se ignoran*/}
    /*operadores aritmeticos y de asignacion*/
"+" { return MAS; }
"-" { return MENOS; }
"*" { return MUL; }
"/" { return DIV; }
"=" { return ASIG; }
    /*simbolos de puntuacion*/
"(" { return LPAR; }
")" { return RPAR; }
"," { return COMA; }
";" { return PYC; }
    /*palabras reservadas: van antes de {ids} para ganar el empate de longitud*/
"if" { return IF; }
"int" { return INT; }
"while" { return WHILE; }
"else" { return ELSE; }
"float" { return FLOAT; }
{numero} { return NUMERO; }
{ids} { return ID; }
    /*cualquier caracter que no coincidio con las reglas anteriores*/
.   { cout << "ERROR LEXICO" << yytext << endl;}

%%

/*al llegar al fin de la entrada no hay mas archivos por leer*/
int yyFlexLexer::yywrap(){
    return 1;
}

