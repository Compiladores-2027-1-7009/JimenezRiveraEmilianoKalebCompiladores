/*
 * Lee cuadriculas de gato de 3x3 (X, O y _ para casilla vacia),
 * separadas por lineas en blanco, e indica si gana X, gana O,
 * si no hay ganador todavia o si es empate.
 */
%{
#include <iostream>
#include <fstream>

/*regresa true si el simbolo s tiene tres en linea en el tablero t*/
bool gana(char t[9], char s) {
    /*las 8 formas de ganar: 3 filas, 3 columnas y 2 diagonales*/
    int lineas[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    for (int i = 0; i < 8; i++) {
        if (t[lineas[i][0]] == s && t[lineas[i][1]] == s && t[lineas[i][2]] == s) {
            return true;
        }
    }
    return false;
}
%}

%option c++
%option noyywrap

/*una fila del tablero; aceptamos 0 (cero) como O por si el archivo lo trae asi*/
fila [XO0_]{3}

%%

{fila}\n{fila}\n{fila} {
    /*yytext trae 11 caracteres: 3 + \n + 3 + \n + 3, copiamos solo las casillas*/
    char t[9];
    int k = 0;
    for (int i = 0; i < yyleng; i++) {
        if (yytext[i] != '\n') {
            t[k] = (yytext[i] == '0') ? 'O' : yytext[i];
            k++;
        }
    }

    std::string resultado;
    if (gana(t, 'X')) {
        resultado = "gana X";
    } else if (gana(t, 'O')) {
        resultado = "gana O";
    } else {
        /*si no gano nadie revisamos si aun quedan casillas vacias*/
        bool hayVacias = false;
        for (int i = 0; i < 9; i++) {
            if (t[i] == '_') hayVacias = true;
        }
        resultado = hayVacias ? "sin ganador" : "empate";
    }

    /*imprimimos el tablero con el resultado en la fila de en medio*/
    std::cout << t[0] << t[1] << t[2] << std::endl;
    std::cout << t[3] << t[4] << t[5] << "  ->  " << resultado << std::endl;
    std::cout << t[6] << t[7] << t[8];
}
    /*las lineas en blanco entre tableros se copian igual*/
\n      { std::cout << std::endl; }
[ \t\r] { }
.       { std::cout << "Caracter no valido en la linea: " << yytext << std::endl; }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Faltan argumentos: ./gato archivo" << std::endl;
        return 1;
    }
    std::ifstream entrada(argv[1]);
    if (!entrada) {
        std::cout << "No se pudo abrir " << argv[1] << std::endl;
        return 1;
    }
    yyFlexLexer lexer(&entrada);
    lexer.yylex();
    std::cout << std::endl;
    return 0;
}
