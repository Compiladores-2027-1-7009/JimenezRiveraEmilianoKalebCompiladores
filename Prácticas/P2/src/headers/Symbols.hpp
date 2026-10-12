#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

/**
 * 1. Definir en un comentario la gramática G(N, T, P, S)
 *
 * Gramatica original:
 *   N = { programa, declaraciones, declaracion, tipo, lista_var,
 *         sentencias, sentencia, expresion }
 *   T = { int, float, identificador, numero, ',', ';', '=', if, else,
 *         while, '(', ')', '+', '-', '*', '/' }
 *   S = programa
 *
 * Gramatica transformada G'(N', T, P', S) que es la que implementa Parser.cpp
 * (sin ambiguedad, sin recursividad izquierda y factorizada, ver README):
 *
 *   programa       -> declaraciones sentencias
 *   declaraciones  -> declaracion declaraciones'
 *   declaraciones' -> declaracion declaraciones' | ε
 *   declaracion    -> tipo lista_var ;
 *   tipo           -> int | float
 *   lista_var      -> identificador lista_var'
 *   lista_var'     -> , identificador lista_var' | ε
 *   sentencias     -> asignacion sentencias' | control
 *   sentencias'    -> sentencias | ε
 *   asignacion     -> identificador = expresion ;
 *   control        -> if ( expresion ) sentencias else sentencias
 *                   | while ( expresion ) sentencias
 *   expresion      -> termino expresion'
 *   expresion'     -> + termino expresion' | - termino expresion' | ε
 *   termino        -> factor termino'
 *   termino'       -> * factor termino' | / factor termino' | ε
 *   factor         -> ( expresion ) | identificador | numero
 */


/**
 * 2. Determinar la enumeración para el conjunto T
 */
enum Token { primero, t_eof,
             t_int, t_float,          // tipos
             t_if, t_else, t_while,   // resto de palabras reservadas
             t_id, t_num,             // identificador y numero
             t_coma, t_pyc, t_asig,   // , ; =
             t_lpar, t_rpar,          // ( )
             t_mas, t_menos, t_mul, t_div, // + - * /
             t_error                  // caracter que no pertenece al lenguaje
};

/**
 * 3. Determinar la enumeración para el conjunto N
 */
enum NoTerm { strEmpty,
              nt_programa, nt_declaraciones, nt_declaracionesP, nt_declaracion,
              nt_tipo, nt_lista_var, nt_lista_varP, nt_sentencias, nt_sentenciasP,
              nt_asignacion, nt_control, nt_expresion, nt_expresionP, nt_termino,
              nt_terminoP, nt_factor
};

#endif // !__SYMBOLS_HPP__
