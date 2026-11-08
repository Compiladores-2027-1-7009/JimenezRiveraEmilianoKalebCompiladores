#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

/*
 * Analizador lexico, el cuerpo de yylex() lo genera flex a partir de lexer.ll
 */
class Lexer : public yyFlexLexer
{
public:
    // in: flujo de donde se leen los caracteres a analizar
    Lexer(std::istream *in) : yyFlexLexer(in){};

    using FlexLexer::yylex;
    // regresa el siguiente token (enum Token de Symbols.hpp), t_eof al terminar
    virtual int yylex();
    // linea en la que va el analisis, para los mensajes de error
    virtual int getLine();

private:
    const int ERROR = -1;
};

#endif /* __LEXER_HPP__ */
