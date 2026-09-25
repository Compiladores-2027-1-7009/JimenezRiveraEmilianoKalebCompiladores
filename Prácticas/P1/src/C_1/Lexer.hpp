#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__ 1

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace C_1
{

    /*
     * Analizador lexico 
     * yytext y yyleng admeas el cuerpo de yylex() lo genera flex a partir de lexer.ll.
     */
    class Lexer : public yyFlexLexer
    {
    public:
        // in: flujo de donde se leen los caracteres a analizar
        Lexer(std::istream *in) : yyFlexLexer(in)
        {
        };

        using FlexLexer::yylex;
        // regresa el siguiente token (tokens.hpp) o 0 al llegar al final 
        virtual int yylex();

    private:
        const int ERROR = -1;        
    };

}

#endif /* END __SCANNER_H__ */
