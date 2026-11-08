#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

/**
 * 1 Definir en un comentario la gramática G'(N', T, P', S)
 *
 *   N' = { programa, declaraciones, declaraciones', declaracion, tipo, lista_var,
 *          lista_var', sentencias, sentencias', asignacion, control, expresion,
 *          expresion', termino, termino', factor }
 *   T  = { int, float, identificador, numero, ',', ';', '=', if, else,
 *          while, '(', ')', '+', '-', '*', '/' }
 *   S  = programa
 *
 * P' (sin ambiguedad, sin recursividad izquierda y factorizada, ver README),
 * el numero de cada produccion es con el que se carga en ParserLL.cpp:
 *
 *    1. programa       -> declaraciones sentencias
 *    2. declaraciones  -> declaracion declaraciones'
 *    3. declaraciones' -> declaracion declaraciones'
 *    4. declaraciones' -> ε
 *    5. declaracion    -> tipo lista_var ;
 *    6. tipo           -> int
 *    7. tipo           -> float
 *    8. lista_var      -> identificador lista_var'
 *    9. lista_var'     -> , identificador lista_var'
 *   10. lista_var'     -> ε
 *   11. sentencias     -> asignacion sentencias'
 *   12. sentencias     -> control
 *   13. sentencias'    -> sentencias
 *   14. sentencias'    -> ε
 *   15. asignacion     -> identificador = expresion ;
 *   16. control        -> if ( expresion ) sentencias else sentencias
 *   17. control        -> while ( expresion ) sentencias
 *   18. expresion      -> termino expresion'
 *   19. expresion'     -> + termino expresion'
 *   20. expresion'     -> - termino expresion'
 *   21. expresion'     -> ε
 *   22. termino        -> factor termino'
 *   23. termino'       -> * factor termino'
 *   24. termino'       -> / factor termino'
 *   25. termino'       -> ε
 *   26. factor         -> ( expresion )
 *   27. factor         -> identificador
 *   28. factor         -> numero
 */


/**
 * 2 Determinar la enumeración para el conjunto T
 */
enum Token { t_eof,                   // simbolo $, fin de la entrada
             t_int, t_float,          // tipos
             t_if, t_else, t_while,   // resto de palabras reservadas
             t_id, t_num,             // identificador y numero
             t_coma, t_pyc, t_asig,   // , ; =
             t_lpar, t_rpar,          // ( )
             t_mas, t_menos, t_mul, t_div, // + - * /
             t_error                  // caracter que no pertenece al lenguaje (no es parte de T)
};

/**
 * 3 Determinar la enumeración para el conjunto N
 */
enum NoTerm { strEmpty,               // representa a la cadena vacia ε en los cuerpos
              nt_programa, nt_declaraciones, nt_declaracionesP, nt_declaracion,
              nt_tipo, nt_lista_var, nt_lista_varP, nt_sentencias, nt_sentenciasP,
              nt_asignacion, nt_control, nt_expresion, nt_expresionP, nt_termino,
              nt_terminoP, nt_factor
};

#endif 
