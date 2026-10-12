/*
 * Analizador lexico para archivos .art
 * Escribe cada token como <clase lexica, valor lexico> en un archivo .tokens
 * con el mismo nombre que el archivo de entrada.
 */
%{
#include <iostream>
#include <fstream>
#include <string>

/*archivo .tokens donde se escribe todo*/
std::ofstream salida;

void token(std::string clase, const char *valor) {
    salida << "<" << clase << ", " << valor << ">" << std::endl;
}
%}

%option c++
%option noyywrap
/*flex lleva la cuenta de las lineas en yylineno (para los errores)*/
%option yylineno

/*estado exclusivo para los comentarios multilinea <* ... *>*/
%x COMENTARIO

DIG [0-9]
letra [a-zA-Z]

%%

    /* f. espacios en blanco: se reconocen pero no generan token */
[ \t\r\n]+                  { }

    /* g. comentario de una linea: desde -- hasta el salto de linea */
"--"[^\n]*                  { }

    /* h. comentario multilinea */
"<*"                        { BEGIN(COMENTARIO); }
<COMENTARIO>"*>"            { BEGIN(INITIAL); }
<COMENTARIO>\n              { }
<COMENTARIO>.               { }
<COMENTARIO><<EOF>>         { salida << "ERROR: comentario sin cerrar al final del archivo" << std::endl; yyterminate(); }

    /* e. palabras reservadas: antes de los ids para ganar el empate de longitud */
if|then|else|while|do|case|is|void|true|false|begin|end|not    { token("RESERVADA", yytext); }

    /* b. reales (antes o despues de enteros da igual, gana el mas largo) */
{DIG}+\.{DIG}+([eE][+-]?{DIG}+)?    { token("REAL", yytext); }
    /* a. enteros */
{DIG}+                              { token("ENTERO", yytext); }

    /* c. identificadores estilo C */
({letra}|_)({letra}|{DIG}|_)*       { token("ID", yytext); }

    /* d. cadenas entre comillas dobles (en una sola linea) */
\"[^\"\n]*\"                        { token("CADENA", yytext); }

    /* k. asignacion */
":="                        { token("ASIGNACION", yytext); }

    /* j. relacionales, <> va antes que < y > aunque gana por longitud */
"<>"|"<"|">"|"="            { token("RELACIONAL", yytext); }

    /* i. aritmeticos */
"+"|"-"|"*"|"/"|"%"         { token("ARITMETICO", yytext); }

    /* l. simbolos especiales */
"("|")"|"{"|"}"|";"|","     { token("ESPECIAL", yytext); }

    /* m. cualquier otro caracter es un error */
.                           { salida << "ERROR: caracter '" << yytext << "' en la linea " << yylineno << std::endl; }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Faltan argumentos: ./analizador archivo.art" << std::endl;
        return 1;
    }

    /*solo aceptamos archivos que terminen en .art*/
    std::string nombre = argv[1];
    if (nombre.size() < 5 || nombre.substr(nombre.size() - 4) != ".art") {
        std::cout << "El archivo debe tener extension .art" << std::endl;
        return 1;
    }

    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cout << "No se pudo abrir " << nombre << std::endl;
        return 1;
    }

    /*mismo nombre pero cambiando .art por .tokens*/
    std::string nombreSalida = nombre.substr(0, nombre.size() - 4) + ".tokens";
    salida.open(nombreSalida);

    yyFlexLexer lexer(&entrada);
    lexer.yylex();

    salida.close();
    std::cout << "Tokens escritos en " << nombreSalida << std::endl;
    return 0;
}
