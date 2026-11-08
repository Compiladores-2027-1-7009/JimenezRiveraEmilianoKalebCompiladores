#include "headers/ParserLL.hpp"
#include <iostream>
#include <cstdlib>

// nombre de cada terminal para los mensajes de error, en el orden del enum Token
static const string nombres[] = {
  "fin de archivo", "'int'", "'float'", "'if'", "'else'", "'while'",
  "un identificador", "un número", "','", "';'", "'='", "'('", "')'",
  "'+'", "'-'", "'*'", "'/'"
};

/*
 * Llave de un no terminal dentro de syms. Los terminales usan su valor del enum
 * Token (de t_eof a t_div) y los no terminales van despues, a partir de t_error
 * que no es parte de la gramatica, para que las llaves no se repitan.
 */
static int N(NoTerm n)
{
  return t_error + n;
}

ParserLL::ParserLL(Lexer *lexer)
{
    this->lexer = lexer;
}

ParserLL::~ParserLL()
{
}


void ParserLL::loadSyms()
{
  // 4. agregar todos los simbolos (N' U T) de G'
  for(int t = t_eof; t < t_error; t++)
    grammar.addSymbol(t, Symbol((Token) t));

  // strEmpty tambien se agrega porque aparece en el cuerpo de las producciones ε
  for(int n = strEmpty; n <= nt_factor; n++)
    grammar.addSymbol(N((NoTerm) n), Symbol((NoTerm) n));

  grammar.setNum(t_error, nt_factor);
  grammar.start = N(nt_programa);
}

void ParserLL::loadProds()
{
  // 5. agregar todas las producciones (P') de G'
  grammar.addProd(1, Production(N(nt_programa), {N(nt_declaraciones), N(nt_sentencias)}));
  grammar.addProd(2, Production(N(nt_declaraciones), {N(nt_declaracion), N(nt_declaracionesP)}));
  grammar.addProd(3, Production(N(nt_declaracionesP), {N(nt_declaracion), N(nt_declaracionesP)}));
  grammar.addProd(4, Production(N(nt_declaracionesP), {N(strEmpty)}));
  grammar.addProd(5, Production(N(nt_declaracion), {N(nt_tipo), N(nt_lista_var), t_pyc}));
  grammar.addProd(6, Production(N(nt_tipo), {t_int}));
  grammar.addProd(7, Production(N(nt_tipo), {t_float}));
  grammar.addProd(8, Production(N(nt_lista_var), {t_id, N(nt_lista_varP)}));
  grammar.addProd(9, Production(N(nt_lista_varP), {t_coma, t_id, N(nt_lista_varP)}));
  grammar.addProd(10, Production(N(nt_lista_varP), {N(strEmpty)}));
  grammar.addProd(11, Production(N(nt_sentencias), {N(nt_asignacion), N(nt_sentenciasP)}));
  grammar.addProd(12, Production(N(nt_sentencias), {N(nt_control)}));
  grammar.addProd(13, Production(N(nt_sentenciasP), {N(nt_sentencias)}));
  grammar.addProd(14, Production(N(nt_sentenciasP), {N(strEmpty)}));
  grammar.addProd(15, Production(N(nt_asignacion), {t_id, t_asig, N(nt_expresion), t_pyc}));
  grammar.addProd(16, Production(N(nt_control), {t_if, t_lpar, N(nt_expresion), t_rpar, N(nt_sentencias), t_else, N(nt_sentencias)}));
  grammar.addProd(17, Production(N(nt_control), {t_while, t_lpar, N(nt_expresion), t_rpar, N(nt_sentencias)}));
  grammar.addProd(18, Production(N(nt_expresion), {N(nt_termino), N(nt_expresionP)}));
  grammar.addProd(19, Production(N(nt_expresionP), {t_mas, N(nt_termino), N(nt_expresionP)}));
  grammar.addProd(20, Production(N(nt_expresionP), {t_menos, N(nt_termino), N(nt_expresionP)}));
  grammar.addProd(21, Production(N(nt_expresionP), {N(strEmpty)}));
  grammar.addProd(22, Production(N(nt_termino), {N(nt_factor), N(nt_terminoP)}));
  grammar.addProd(23, Production(N(nt_terminoP), {t_mul, N(nt_factor), N(nt_terminoP)}));
  grammar.addProd(24, Production(N(nt_terminoP), {t_div, N(nt_factor), N(nt_terminoP)}));
  grammar.addProd(25, Production(N(nt_terminoP), {N(strEmpty)}));
  grammar.addProd(26, Production(N(nt_factor), {t_lpar, N(nt_expresion), t_rpar}));
  grammar.addProd(27, Production(N(nt_factor), {t_id}));
  grammar.addProd(28, Production(N(nt_factor), {t_num}));
}

void ParserLL::loadTable()
{

  // 6. cargar la tabla de AS predictivo
  // cada entrada es el numero de produccion, las que no estan son error
  table[nt_programa][t_int] = 1;
  table[nt_programa][t_float] = 1;

  table[nt_declaraciones][t_int] = 2;
  table[nt_declaraciones][t_float] = 2;

  table[nt_declaracionesP][t_int] = 3;
  table[nt_declaracionesP][t_float] = 3;
  table[nt_declaracionesP][t_id] = 4;
  table[nt_declaracionesP][t_if] = 4;
  table[nt_declaracionesP][t_while] = 4;

  table[nt_declaracion][t_int] = 5;
  table[nt_declaracion][t_float] = 5;

  table[nt_tipo][t_int] = 6;
  table[nt_tipo][t_float] = 7;

  table[nt_lista_var][t_id] = 8;

  table[nt_lista_varP][t_coma] = 9;
  table[nt_lista_varP][t_pyc] = 10;

  table[nt_sentencias][t_id] = 11;
  table[nt_sentencias][t_if] = 12;
  table[nt_sentencias][t_while] = 12;

  table[nt_sentenciasP][t_id] = 13;
  table[nt_sentenciasP][t_if] = 13;
  table[nt_sentenciasP][t_while] = 13;
  table[nt_sentenciasP][t_else] = 14;
  table[nt_sentenciasP][t_eof] = 14;

  table[nt_asignacion][t_id] = 15;

  table[nt_control][t_if] = 16;
  table[nt_control][t_while] = 17;

  table[nt_expresion][t_id] = 18;
  table[nt_expresion][t_num] = 18;
  table[nt_expresion][t_lpar] = 18;

  table[nt_expresionP][t_mas] = 19;
  table[nt_expresionP][t_menos] = 20;
  table[nt_expresionP][t_pyc] = 21;
  table[nt_expresionP][t_rpar] = 21;

  table[nt_termino][t_id] = 22;
  table[nt_termino][t_num] = 22;
  table[nt_termino][t_lpar] = 22;

  table[nt_terminoP][t_mul] = 23;
  table[nt_terminoP][t_div] = 24;
  table[nt_terminoP][t_mas] = 25;
  table[nt_terminoP][t_menos] = 25;
  table[nt_terminoP][t_pyc] = 25;
  table[nt_terminoP][t_rpar] = 25;

  table[nt_factor][t_lpar] = 26;
  table[nt_factor][t_id] = 27;
  table[nt_factor][t_num] = 28;
}

int ParserLL::parse()
{
  // 7. algoritmo de AS predictivo

  //Auxiliares
  stack<Symbol> pila;
  map<Token, int>::iterator accion;
  vector<int> body;
  Symbol X;

  loadSyms();
  loadProds();
  loadTable();

  // la pila empieza con $ hasta abajo y el simbolo inicial en el tope
  pila.push(grammar.getSym(t_eof));
  pila.push(grammar.getSym(grammar.start));
  token = eat();

  while(!pila.empty()){
    X = pila.top();

    if(X.getType() == terminal){
      if(X.getToken() != token)
        error("se esperaba " + nombres[X.getToken()]);
      pila.pop();
      // el ultimo en coincidir es $, despues de el ya no hay nada que leer
      if(!pila.empty())
        token = eat();
    }
    else{
      accion = table[X.getNoTerm()].find(token);
      if(accion == table[X.getNoTerm()].end()){
        // lo que si se podia leer son las columnas con entrada para este no terminal
        string esperados;
        for(auto &entrada : table[X.getNoTerm()])
          esperados += " " + nombres[entrada.first];
        error("se esperaba alguno de:" + esperados);
      }

      // se reemplaza X por el cuerpo al reves, para que el primer simbolo quede en el tope
      pila.pop();
      body = grammar.getProd(accion->second).getBody();
      for(int i = body.size() - 1; i >= 0; i--){
        Symbol s = grammar.getSym(body[i]);
        // en las producciones ε no se apila nada
        if(s.getType() == terminal || s.getNoTerm() != strEmpty)
          pila.push(s);
      }
    }
  }

  aceptar("La cadena es aceptada");
  return 0;
}

// al primer error se detiene el analisis, ya no tiene caso seguir
void ParserLL::error(string msg)
{
    cout<<"ERROR DE SINTAXIS "<<msg<<" en la línea "<<lexer->getLine()<<endl;
    cout<<"La cadena no pertenece al lenguaje generado por la gramática"<<endl;
    exit(EXIT_FAILURE);
}

Token ParserLL::eat()
{
    return (Token) lexer->yylex();
}

void ParserLL::aceptar(string msg)
{
    cout<<msg<<endl;
}
