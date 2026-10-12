<p  align="center">
  <img  width="200"  src="https://www.fciencias.unam.mx/sites/default/files/logoFC_2.png"  alt="">  <br>Compiladores  2027-1 <br>
  Práctica 2: Análisis sintáctico descendente <br> Profesora: Ariel Adara Mercado Martínez
</p>


# Análisis sintáctico descendente

## Objetivo:
Que el alumno conozca e implemente los principios del análisis sintáctico descendente recursivo, aplicando técnicas de transformación de gramáticas —eliminación de ambigüedad, eliminación de recursividad izquierda y 
factorización izquierda— para construir un analizador sintáctico predictivo que, junto con el analizador léxico desarrollado con Flex, permita reconocer programas válidos del lenguaje C_1.


### Estructura del directorio
```c++
p2
├── README.md
└── src //carpeta de código
    ├── headers
    │   ├── Lexer.hpp   //header de la clase Lexer
    │   ├── Parser.hpp  //header de la clase Parser
    │   └── Symbols.hpp //definición de símbolos y de la gramática
    ├── lexer.ll        //definición del an. léxico
    ├── main.cpp        //contiene la función principal del programa
    ├── Makefile        //compila y ejecuta las pruebas
    ├── Parser.cpp      //código fuente de la clase Parser
    ├── prueba          //archivo de entrada para el analizador (léxico y sintáctico)
    ├── prueba_valida1  //pruebas extra (ejercicio 9)
    ├── prueba_valida2
    ├── prueba_invalida1
    └── prueba_invalida2
```

### Uso

#### Compilación

```bash
$ cd src/
$ flex++ lexer.ll
$ g++ Lexer.cpp Parser.cpp main.cpp -o analyzer
```

#### Ejecución

```bash
$ ./analyzer prueba
```

O con el Makefile (ver ejercicio 10):

```bash
$ make          # genera Lexer.cpp y compila analyzer
$ make run      # ejecuta ./analyzer prueba
$ make pruebas  # ejecuta todos los archivos de prueba
$ make clean    # borra Lexer.cpp y analyzer
```

#### Salida esperada
```
La cadena es aceptada
```

#### Ejercicios
Para la gramática G = ( N, Σ, P, S), descrita por las siguientes producciones: 
> P = {
>> programa → declaraciones sentencias <br>
>> declaraciones → declaraciones declaracion | declaracion <br>
>> declaracion → tipo lista-var **;** <br>
>> tipo → **int** | **float** <br>
>> lista_var → lista_var **,** _**identificador**_ | _**identificador**_ <br>
>> sentencias → sentencias sentencia | sentencia <br>
>> sentencia → _**identificador**_ **=** expresion **;** | **if** **(** expresion **)** sentencias **else** sentencias | **while** **(** expresión **)** sentencias <br>
>> expresion → expresion **+** expresion | expresion **-** expresion | expresion __\*__ expresion | expresion **\/** expresión | _**identificador**_ | **_numero_** <br>
>> expresion → **(** expresion **)** <br>
}


1. Determinar en un archivo Readme, en formato Markdown (.md) o LaTeX (.tex) -- con su respectivo PDF, para este último -- , los conjuntos _N_, _Σ_ y el símbolo inicial _S_.  (0.5 pts.)

    Las variables o no terminales son los que aparecen del lado izquierdo de las producciones, los terminales son los símbolos que aparecen en negritas (lo que regresa el analizador léxico) y el símbolo inicial es el de la primera producción:

    * _N_ = { programa, declaraciones, declaracion, tipo, lista_var, sentencias, sentencia, expresion }
    * _Σ_ = { **int**, **float**, **identificador**, **numero**, **,**, **;**, **=**, **if**, **else**, **while**, **(**, **)**, **+**, **-**, __\*__, **/** }
    * _S_ = programa

    En la producción de `declaracion` aparece escrito `lista-var` y en la de `lista_var` con guion bajo; lo tomé como el mismo no terminal, ya que si no `lista-var` no tendría ninguna producción y no se podría derivar nada. Lo mismo con `expresión` con acento en las producciones de `while` y de la división, es el mismo no terminal `expresion`.

2. Mostrar en el archivo el proceso de eliminación de ambigüedad o justificar, en caso de no ser necesario. (1 pts.).

    Sí es necesario, encontré dos fuentes de ambigüedad:

    **a) Las expresiones.** La producción `expresion → expresion + expresion | expresion * expresion | ...` no dice nada de precedencia ni de asociatividad, entonces una cadena como `id + id * id` tiene dos árboles de derivación distintos:

    ```
           expresion                          expresion
         /     |     \                       /     |     \
       exp     +     exp                   exp     *     exp
        |          /  |  \              /  |  \          |
        id      exp   *  exp           exp  +  exp        id
                 |        |             |        |
                 id       id            id       id
    ```

    El de la izquierda es `id + (id * id)` y el de la derecha `(id + id) * id`. Lo mismo pasa con `id - id - id`, que se puede agrupar como `(id - id) - id` o `id - (id - id)`, y ahí ya cambia el resultado. Para eliminarla introduje un no terminal por cada nivel de precedencia: `expresion` para `+` y `-` (menor precedencia), `termino` para `*` y `/`, y `factor` para lo que no se puede partir (paréntesis, identificadores y números). La recursión queda del lado izquierdo para que los operadores asocien a la izquierda, como en C:

    > expresion → expresion **+** termino | expresion **-** termino | termino <br>
    > termino → termino __\*__ factor | termino **/** factor | factor <br>
    > factor → **(** expresion **)** | **identificador** | **numero**

    Así `id + id * id` solo se puede derivar poniendo el `*` más abajo en el árbol, es decir, con mayor precedencia.

    **b) Las sentencias dentro de `while` y del `else`.** Esta me costó más trabajo verla. Como `while ( expresion ) sentencias` y `... else sentencias` terminan en `sentencias`, que es una **lista** de sentencias sin ningún delimitador de cierre (no hay llaves), no se sabe dónde termina el cuerpo. Por ejemplo, en

    ```c
    while (a) x = 1; y = 2;
    ```

    `y = 2;` puede ser parte del cuerpo del `while` o ser la siguiente sentencia después del `while`, y las dos opciones se derivan con la gramática original. Es un problema parecido al del _else colgante_. El del `if` no tiene este problema en la parte del _then_ porque ahí el `else` funciona como cierre.

    Para eliminarla sin cambiar el lenguaje decidí la misma convención que se usa con el else colgante: el cuerpo del `while` (y la parte del `else`) se lleva **todas** las sentencias que le siguen dentro de su lista. Eso quiere decir que una sentencia `if` o `while` siempre es la última de su lista, y antes de ella solo puede haber asignaciones. Entonces separé `sentencia` en dos no terminales, `asignacion` y `control`, y reescribí `sentencias` como "cero o más asignaciones seguidas de una asignación o de un `control`":

    > sentencias → asignacion sentencias | asignacion | control <br>
    > asignacion → **identificador** **=** expresion **;** <br>
    > control → **if** **(** expresion **)** sentencias **else** sentencias | **while** **(** expresion **)** sentencias

    El lenguaje es el mismo, porque cualquier secuencia de sentencias de la gramática original se puede derivar así (agrupando con la convención), pero ahora `while (a) x = 1; y = 2;` tiene un solo árbol, con `y = 2;` dentro del `while`. Que ya no hay ambigüedad se comprueba al final del ejercicio 5, donde verifiqué que la gramática resultante es LL(1), y toda gramática LL(1) es no ambigua.

    Las demás producciones (`declaraciones`, `declaracion`, `tipo`, `lista_var`) no son ambiguas: las listas son recursivas solo de un lado y cada declaración termina con `;`, así que cada cadena tiene una sola forma de derivarse.

3. Mostrar en el archivo el proceso de eliminación de la recursividad izquierda o justificar, en caso de no ser necesario. (1 pts.)

    Sí es necesario, porque un analizador de descenso recursivo con recursividad izquierda se cicla: por ejemplo la función de `declaraciones` lo primero que haría es volver a llamar a `declaraciones` sin haber consumido ningún token, y así infinitamente. Después del ejercicio 2 quedaron con recursividad izquierda inmediata `declaraciones`, `lista_var`, `expresion` y `termino` (no hay recursividad indirecta, ningún no terminal empieza con otro que a su vez regrese a él). Usé la regla que vimos en clase:

    > A → A α | β &nbsp;&nbsp; se reemplaza por &nbsp;&nbsp; A → β A' &nbsp;&nbsp; A' → α A' | ε

    * `declaraciones → declaraciones declaracion | declaracion` con α = `declaracion` y β = `declaracion`:
      > declaraciones → declaracion declaraciones' <br>
      > declaraciones' → declaracion declaraciones' | ε
    * `lista_var → lista_var , identificador | identificador` con α = `, identificador` y β = `identificador`:
      > lista_var → **identificador** lista_var' <br>
      > lista_var' → **,** **identificador** lista_var' | ε
    * `expresion → expresion + termino | expresion - termino | termino` con α₁ = `+ termino`, α₂ = `- termino` y β = `termino`:
      > expresion → termino expresion' <br>
      > expresion' → **+** termino expresion' | **-** termino expresion' | ε
    * `termino → termino * factor | termino / factor | factor`, igual que la anterior:
      > termino → factor termino' <br>
      > termino' → __\*__ factor termino' | **/** factor termino' | ε

    La original `sentencias → sentencias sentencia | sentencia` también tenía recursividad izquierda, pero ya no después de reescribirla en el ejercicio 2: ahora `sentencias` empieza con `asignacion` o con `control` y la recursión queda a la derecha.

    Algo que noté es que con `expresion'` y `termino'` el árbol ya no se "inclina" a la izquierda, pero la asociatividad a la izquierda se conserva en el parser porque los operadores se van consumiendo de izquierda a derecha.

4. Mostrar en el archivo el proceso de factorización izquierda o justificar, en caso de no ser necesario. (1 pts.)

    Hay que factorizar cuando dos alternativas de un mismo no terminal empiezan con el mismo prefijo, porque con un solo token de preanálisis el parser no sabría cuál elegir. Revisé todas las producciones y la única que lo necesita es la de `sentencias` que salió del ejercicio 2, donde las dos primeras alternativas empiezan con `asignacion`:

    > sentencias → asignacion sentencias | asignacion | control

    Con la regla A → α β₁ | α β₂ &nbsp;⇒&nbsp; A → α A' &nbsp; A' → β₁ | β₂, con α = `asignacion`, β₁ = `sentencias` y β₂ = ε:

    > sentencias → asignacion sentencias' | control <br>
    > sentencias' → sentencias | ε

    En las demás no hace falta: en `tipo` (`int` / `float`), `factor` (`(` / `identificador` / `numero`), `control` (`if` / `while`) y las primas del ejercicio 3 (`+` / `-` / ε, `*` / `/` / ε, `,` / ε, y `declaracion` / ε) cada alternativa empieza con un terminal distinto. En la gramática original, las alternativas de `expresion` sí compartían el prefijo `expresion`, pero eso ya lo resolvieron los ejercicios 2 y 3.

5. Mostrar en el archivo los nuevos conjuntos _N_ y _P_. (0.5 pts.)

    * _N'_ = { programa, declaraciones, declaraciones', declaracion, tipo, lista_var, lista_var', sentencias, sentencias', asignacion, control, expresion, expresion', termino, termino', factor }
    * _Σ_ y _S_ no cambian.
    * _P'_ = {
      > programa → declaraciones sentencias <br>
      > declaraciones → declaracion declaraciones' <br>
      > declaraciones' → declaracion declaraciones' | ε <br>
      > declaracion → tipo lista_var **;** <br>
      > tipo → **int** | **float** <br>
      > lista_var → **identificador** lista_var' <br>
      > lista_var' → **,** **identificador** lista_var' | ε <br>
      > sentencias → asignacion sentencias' | control <br>
      > sentencias' → sentencias | ε <br>
      > asignacion → **identificador** **=** expresion **;** <br>
      > control → **if** **(** expresion **)** sentencias **else** sentencias | **while** **(** expresion **)** sentencias <br>
      > expresion → termino expresion' <br>
      > expresion' → **+** termino expresion' | **-** termino expresion' | ε <br>
      > termino → factor termino' <br>
      > termino' → __\*__ factor termino' | **/** factor termino' | ε <br>
      > factor → **(** expresion **)** | **identificador** | **numero** <br>

      }

    Esta misma gramática quedó escrita en el comentario de _headers/Symbols.hpp_. Para asegurarme de que el parser predictivo funciona (y de que ya no hay ambigüedad) calculé los conjuntos PRIMEROS y SIGUIENTES:

    | No terminal | PRIMEROS | SIGUIENTES |
    |-------------|----------|------------|
    | programa | int, float | $ |
    | declaraciones | int, float | identificador, if, while |
    | declaraciones' | int, float, ε | identificador, if, while |
    | declaracion | int, float | int, float, identificador, if, while |
    | tipo | int, float | identificador |
    | lista_var | identificador | ; |
    | lista_var' | `,`, ε | ; |
    | sentencias | identificador, if, while | else, $ |
    | sentencias' | identificador, if, while, ε | else, $ |
    | asignacion | identificador | identificador, if, while, else, $ |
    | control | if, while | else, $ |
    | expresion | (, identificador, numero | ), ; |
    | expresion' | +, -, ε | ), ; |
    | termino | (, identificador, numero | +, -, ), ; |
    | termino' | \*, /, ε | +, -, ), ; |
    | factor | (, identificador, numero | \*, /, +, -, ), ; |

    En cada no terminal con varias alternativas los PRIMEROS de cada una son disjuntos, y en los que derivan ε los PRIMEROS no se cruzan con los SIGUIENTES (p. ej. `sentencias'`: {identificador, if, while} contra {else, $}). Por lo tanto la gramática es LL(1). Con la gramática original esto no pasaba: `sentencias` aparece al final de `while`, así que los SIGUIENTES de la lista incluían a los PRIMEROS de `sentencia` y había conflicto, que es justamente la ambigüedad del ejercicio 2.

6. Definir el Analizador Léxico (lexer.ll) con las acciones léxicas correspondientes. (2 pts.)

    En _lexer.ll_ reutilicé las macros que hice en la práctica 1 para los números y los identificadores, y cada acción léxica regresa el token correspondiente de la enumeración `Token` de _headers/Symbols.hpp_:

    | Terminal | Token | Expresión regular |
    |----------|-------|-------------------|
    | Palabras reservadas | `t_int`, `t_float`, `t_if`, `t_else`, `t_while` | `"int"`, `"float"`, `"if"`, `"else"`, `"while"` |
    | Números enteros y flotantes | `t_num` | `numero {DIG}+(\.{DIG}+)?([eE][+-]?{DIG}+)?` |
    | Identificadores | `t_id` | `ids ({letra}\|_)({letra}\|{DIG}\|_)*` |
    | Coma, punto y coma, asignación | `t_coma`, `t_pyc`, `t_asig` | `","`, `";"`, `"="` |
    | Paréntesis | `t_lpar`, `t_rpar` | `"("`, `")"` |
    | Operadores aritméticos | `t_mas`, `t_menos`, `t_mul`, `t_div` | `"+"`, `"-"`, `"*"`, `"/"` |
    | Fin de archivo | `t_eof` | `<<EOF>>` |
    | Espacios y saltos de línea (se ignoran) | — | `[ \t\r]+` y `"\n"` (este además hace `line++`) |
    | Cualquier otro carácter | `t_error` | `.` |

    Igual que en la práctica 1, las palabras reservadas van antes de `{ids}` para que ganen el empate de longitud. Hay dos diferencias importantes con la práctica anterior:

    * En la plantilla, `t_eof` vale 1 porque `primero` ocupa el 0, pero `yylex()` regresa 0 cuando se acaba la entrada. Entonces si no se hace nada el parser nunca ve `t_eof` y nunca acepta la cadena. Por eso agregué la regla `<<EOF>> { return t_eof; }`.
    * En la práctica 1 la regla `.` solo imprimía `ERROR LEXICO` y seguía, pero aquí eso haría que el parser aceptara algo como `a = 1 $ 2;` sin `$` (o que no lo aceptara por una razón que no tiene que ver). Ahora además de imprimir el error regresa `t_error`, que no coincide con ningún terminal de la gramática, así que el parser lo reporta como error de sintaxis en esa misma línea.

    La variable global `line` lleva la cuenta de las líneas y `Lexer::getLine()` la regresa para que el parser la ponga en sus mensajes.

7. Implementar el Analizador Sintáctico (Parser.cpp) de descenso recursivo, documentando las funciones de cada No-Terminal, de forma que el programa descrito en el archivo _prueba_ sea reconocido y aceptado por el analizador resultante. (4 pts.)

    En _Parser.cpp_ hice una función por cada no terminal de la gramática del ejercicio 5 (las primas terminan en `P`, p. ej. `declaracionesP`), y cada una tiene arriba como comentario la producción que implementa. La idea de descenso recursivo es que el cuerpo de cada función es el lado derecho de su producción:

    * un **no terminal** se vuelve una llamada a su función,
    * un **terminal** se compara con el token de preanálisis (`token`) y si coincide se pide el siguiente con `eat()`; si no, se llama a `error()`. Para no repetir ese `if` en todos lados agregué el método privado `match(t, esperado)`.
    * cuando hay **varias alternativas**, se elige con un `if` según el token actual, usando los PRIMEROS de la tabla del ejercicio 5. Las alternativas ε son el caso en el que no entra a ningún `if` y simplemente se regresa sin consumir nada.

    Por ejemplo, para `expresion' → + termino expresion' | - termino expresion' | ε`:

    ```c++
    void Parser::expresionP(){
      if(token == t_mas || token == t_menos){
        token = eat();
        termino();
        expresionP();
      }
    }
    ```

    En `sentenciasP` además reviso que, si se toma ε, el token sí esté en SIGUIENTES (`else` o fin de archivo); así un `)` suelto da un mensaje con línea en vez de solo "no pertenece". Y en `parse()`, si después de `programa()` sobra algo, también se llama a `error()`.

    `error()` imprime `ERROR DE SINTAXIS`, lo que se esperaba y la línea, y termina el programa en el primer error, ya que sin recuperación de errores seguir analizando solo generaría mensajes en cascada. Con el archivo _prueba_ la salida es:

    ```
    $ ./analyzer prueba
    La cadena es aceptada
    ```


---
#### Extras

8. Documentar el código. (0.25pts)

    Documenté _lexer.ll_ (qué reconoce cada macro y por qué el orden de las reglas, `<<EOF>>` y `t_error`), _Parser.cpp_ (la producción que implementa cada función), _headers/Parser.hpp_, _headers/Lexer.hpp_, _main.cpp_ y el _Makefile_. En _headers/Symbols.hpp_ escribí la gramática original y la transformada, además de las enumeraciones de _T_ y _N_.

9. Proponer 4 archivos de prueba nuevos, 2 válidos y 2 inválidos. (0.25pts)

    Agregué cuatro archivos en _src/_, que se ejecutan con `./analyzer <archivo>` o todos juntos con `make pruebas`:

    * **_prueba_valida1_**: varias variables por declaración, una expresión con paréntesis y los cuatro operadores, un flotante con exponente negativo y un `while`. Aquí se ve la convención del ejercicio 2: la asignación de `promedio` también queda dentro del cuerpo del `while`, por ser la última de la lista (la sangría da igual, el lexer ignora los espacios).
    * **_prueba_valida2_**: tres declaraciones, un identificador con guion bajo y un `if-else` con otro `if-else` y un `while` anidados en el `else`, para probar la recursión entre `sentencias` y `control`.

    ```
    ===== prueba_valida1 =====
    La cadena es aceptada
    ===== prueba_valida2 =====
    La cadena es aceptada
    ```

    * **_prueba_invalida1_**: le falta el `;` a la primera declaración (`int a, b`). El error se reporta en la línea 2 aunque el `;` faltaba en la 1, porque el parser se da cuenta hasta que lee el siguiente token, que es `float` en la línea 2, y para entonces el lexer ya contó el salto de línea.
    * **_prueba_invalida2_**: un paréntesis sin cerrar dentro de la expresión de una asignación del `if` (`(2 + 3;`). `factor` lee `( expresion` y luego espera `)` pero encuentra `;`.

    ```
    ===== prueba_invalida1 =====
    ERROR DE SINTAXIS se esperaba ';' en la línea 2
    La cadena no pertenece al lenguaje generado por la gramática
    ===== prueba_invalida2 =====
    ERROR DE SINTAXIS se esperaba ')' en la línea 5
    La cadena no pertenece al lenguaje generado por la gramática
    ```

10. Modificar el Makefile para poder simplificar el proceso de compilación de la práctica. (1pt)

    El _Makefile_ de la plantilla no funcionaba (buscaba _./AL/Lexer.cpp_ y el objetivo `all` dependía de `wc`), así que lo rehice igual que el de la práctica 1:

    * `make` (o `make all`): ejecuta `flex++ lexer.ll` para generar _Lexer.cpp_ y luego `g++ Lexer.cpp Parser.cpp main.cpp -o analyzer`.
    * `make run`: compila si hace falta y ejecuta `./analyzer prueba`.
    * `make pruebas`: ejecuta el analizador con _prueba_ y los cuatro archivos del ejercicio 9, imprimiendo el nombre de cada uno. Como las pruebas inválidas terminan con `EXIT_FAILURE`, le puse `|| true` para que `make` no marque error y siga con las demás.
    * `make clean`: borra los archivos generados (_Lexer.cpp_ y _analyzer_).

    Como cada objetivo declara sus dependencias (incluyendo los headers), si se modifica _lexer.ll_ o _Symbols.hpp_ se vuelve a generar _Lexer.cpp_ y el ejecutable, y si todo está al día solo responde `Nothing to be done`.

