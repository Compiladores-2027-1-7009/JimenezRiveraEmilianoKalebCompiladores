%{
  #include <iostream>
%}

%option c++
%option noyywrap


digito [0-9]
letra [a-zA-Z]
palabra {letra}+
espacio [ \t\n]

/*comentario para el ejercicio 3*/
%%


{espacio} {/* La acción léxica puede ir vacía si queremos que el escáner ignore la regla*/}
{digito}+ { std::cout << "Encontré un número: " << yytext << std::endl; } 
{palabra} { std::cout << "Encontré una palabra: " << yytext << std::endl; }

%%


int main() {
  FlexLexer* lexer = new yyFlexLexer;
  lexer->yylex();
}

