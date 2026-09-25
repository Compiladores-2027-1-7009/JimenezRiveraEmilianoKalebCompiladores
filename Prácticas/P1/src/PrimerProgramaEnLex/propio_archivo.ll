%{
  #include <iostream>
%}

%option c++
%option noyywrap

digito [0-9]
/*podemos poner tanto 0x o 0X*/
hex 0[xX]({digito}|[a-fA-F])+
/*al parecer si pones espacios entre | no trabaja bien*/
reservadas int|true|class|double|try
ids [a-zA-Z_][a-zA-Z0-9_]{0,31}
espacio [ \t\n]


%%


{espacio} {/*nada*/} 
{digito}+ { std::cout << "Encontré un número: " << yytext << std::endl; } 
{hex} { std::cout << "Encontré un número hexadecimal de c++: " << yytext << std::endl; } 
{reservadas} { std::cout << "Encontré una palabra reservada de c++: " << yytext << std::endl; }
{ids} { std::cout << "Encontré un identificador de c++: " << yytext << std::endl; }

%%


int main() {
  FlexLexer* lexer = new yyFlexLexer;
  lexer->yylex();
}

