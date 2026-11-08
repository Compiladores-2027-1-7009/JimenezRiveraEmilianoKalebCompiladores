<p  align="center">
  <img  width="200"  src="https://www.fciencias.unam.mx/sites/default/files/logoFC_2.png"  alt="">  <br>Compiladores  2027-1 <br>
  Práctica 3: Analizador sintáctico de descenso predictivo <br> Profesora: Ariel Adara Mercado Martínez
</p>

# Análisis sintáctico descendente predictivo

## Objetivo:
Que el alumno conozca e implemente los principios del análisis sintáctico descendente predictivo, aplicando la construcción de los conjuntos FIRST y FOLLOW, así como la tabla de análisis sintáctico predictivo, para construir un analizador sintáctico LL(1) que, junto con el analizador léxico desarrollado con Flex, permita reconocer programas válidos del lenguaje C_1.


### Estructura del directorio
```c++
p3
├── README.md
└── src //carpeta de código
    ├── prueba //archivo para I/O test
    ├── prueba_valida1 //pruebas extra (ejercicio 14)
    ├── prueba_valida2
    ├── prueba_invalida1
    ├── prueba_invalida2
    ├── lib //carpeta para bibliotecas
    │   └── FlexLexer.h //archivo de cabecera (ignorar)
    ├── Makefile //archivo de reglas de compilación
    ├── main.cpp //contiene la función principal del programa
    ├── Production.cpp //archivo para la clase producción
    ├── lexer.ll //archivo de Flex para generación de `scanner`
    ├── Symbol.cpp //archivo para la clase de símbolo
    ├── Grammar.cpp //archivo para la clase de gramática
    ├── ParserLL.cpp //archivo para parser LL(1)
    └── headers //carpeta de archivos de código
        ├── Lexer.hpp //archivo de cabecera para el scanner
        ├── Symbol.h //archivo de cabecera de símbolo
        ├── Production.h //archivo de cabecera de producción
        ├── Symbols.hpp //archivo de cabecera definición de los conjuntos N y Σ
        ├── Grammar.h //archivo de cabecera de gramática
        └── ParserLL.hpp //archivo de cabecera del parser LL(1)

```

### Uso

#### Compilacion

```bash
$ cd src/
$ make
```

#### Ejecucion

```bash
$ ./compiler prueba
```

O con el Makefile:

```bash
$ make pruebas  # ejecuta todos los archivos de prueba
$ make clean    # borra los archivos generados
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
>> expresion → expresion **+** expresion | expresion **-** expresion | expresion __\*__ expresion | expresion **/** expresión | _**identificador**_ | **_numero_** <br>
>> expresion → **(** expresion **)** <br>
}


1. Determinar en un archivo Readme, en formato Markdown (.md) o LaTeX (.tex) - con su respectivo PDF, para este último - , los conjuntos _N_, _Σ_ y el símbolo inicial _S_.  (0.5 pts.)

    Las variables o no terminales son los que aparecen del lado izquierdo de las producciones, los terminales son los símbolos que aparecen en negritas (lo que regresa el analizador léxico) y el símbolo inicial es el de la primera producción:

    * _N_ = { programa, declaraciones, declaracion, tipo, lista_var, sentencias, sentencia, expresion }
    * _Σ_ = { **int**, **float**, **identificador**, **numero**, **,**, **;**, **=**, **if**, **else**, **while**, **(**, **)**, **+**, **-**, __\*__, **/** }
    * _S_ = programa

    Igual que en la práctica 2, tomé `lista-var` y `lista_var` como el mismo no terminal, y `expresión` con acento como `expresion`, ya que si no esos símbolos no tendrían ninguna producción y no se podría derivar nada.

    a. Mostrar en el archivo el proceso de eliminación de ambigüedad o justificar, en caso de no ser necesario. (0.25 pts.)

    Sí es necesario. Es la misma gramática de la práctica 2, así que partí de ese análisis, donde encontré dos fuentes de ambigüedad:

    **Las expresiones.** `expresion → expresion + expresion | expresion * expresion | ...` no dice nada de precedencia ni de asociatividad, entonces `id + id * id` se puede derivar como `id + (id * id)` o como `(id + id) * id`, dos árboles distintos. Lo resolví con un no terminal por cada nivel de precedencia: `expresion` para `+` y `-`, `termino` para `*` y `/`, y `factor` para lo que no se puede partir. La recursión queda del lado izquierdo para que los operadores asocien a la izquierda, como en C:

    > expresion → expresion **+** termino | expresion **-** termino | termino <br>
    > termino → termino __\*__ factor | termino **/** factor | factor <br>
    > factor → **(** expresion **)** | **identificador** | **numero**

    **Las sentencias dentro de `while` y del `else`.** Como `while ( expresion ) sentencias` termina en una lista sin delimitador de cierre (no hay llaves), no se sabe dónde termina el cuerpo. En `while (a) x = 1; y = 2;` la sentencia `y = 2;` puede ser parte del cuerpo o ser la siguiente después del `while`. Usé la misma convención que con el else que cuelga, en otras palabras el cuerpo se lleva **todas** las sentencias que le siguen dentro de su lista. Eso significa que un `if` o `while` siempre es la última sentencia de su lista, y antes de ella solo puede haber asignaciones, así que separé `sentencia` en `asignacion` y `control`:

    > sentencias → asignacion sentencias | asignacion | control <br>
    > asignacion → **identificador** **=** expresion **;** <br>
    > control → **if** **(** expresion **)** sentencias **else** sentencias | **while** **(** expresion **)** sentencias

    Que ya no hay ambigüedad se comprueba en el ejercicio 4: la tabla no tiene ninguna celda con dos producciones, por lo que _G'_ es LL(1), y toda gramática LL(1) es no ambigua.

    b. Mostrar en el archivo el proceso de eliminación de la recursividad izquierda o justificar, en caso de no ser necesario. (0.25 pts.)

    Sí es necesario. En un parser predictivo con recursividad izquierda, al expandir `A → A α` se vuelve a tener `A` en el tope de la pila sin haber consumido ningún token, y tenemos algo iterativo que nunca pararía; además la tabla tendría conflictos. Por otro lado, como no hay recursividad indirecta y solo inmediata, tuve que usar la regla vista en clase:

    > A → A α | β &nbsp;&nbsp; se reemplaza por &nbsp;&nbsp; A → β A' &nbsp;&nbsp; A' → α A' | ε

    * `declaraciones → declaraciones declaracion | declaracion`:
      > declaraciones → declaracion declaraciones' <br>
      > declaraciones' → declaracion declaraciones' | ε
    * `lista_var → lista_var , identificador | identificador`:
      > lista_var → **identificador** lista_var' <br>
      > lista_var' → **,** **identificador** lista_var' | ε
    * `expresion → expresion + termino | expresion - termino | termino`:
      > expresion → termino expresion' <br>
      > expresion' → **+** termino expresion' | **-** termino expresion' | ε
    * `termino → termino * factor | termino / factor | factor`:
      > termino → factor termino' <br>
      > termino' → __\*__ factor termino' | **/** factor termino' | ε

    La original `sentencias → sentencias sentencia | sentencia` también era recursiva por la izquierda, pero dejó de serlo al reescribirla en el inciso a: ahora la recursión queda a la derecha.

    c. Mostrar en el archivo el proceso de factorización izquierda o justificar, en caso de no ser necesario. (0.25 pts.)

    Hay que factorizar cuando dos alternativas de un no terminal empiezan con el mismo prefijo. La única que lo necesita es `sentencias`, donde las dos primeras alternativas empiezan con `asignacion`. Con A → α β₁ | α β₂ &nbsp;⇒&nbsp; A → α A' &nbsp; A' → β₁ | β₂, tomando α = `asignacion`, β₁ = `sentencias` y β₂ = ε:

    > sentencias → asignacion sentencias' | control <br>
    > sentencias' → sentencias | ε

    En las demás no hace falta, porque cada alternativa empieza con un terminal distinto (`tipo`, `factor`, `control`, y las primas del inciso b).

    d. Mostrar en el archivo los nuevos conjuntos _N'_ y _P'_, que definen _G'_. (0.25 pts.)

    * _N'_ = { programa, declaraciones, declaraciones', declaracion, tipo, lista_var, lista_var', sentencias, sentencias', asignacion, control, expresion, expresion', termino, termino', factor }
    * _Σ_ y _S_ no cambian.
    * _P'_ = { (numeradas, porque así se cargan en _ParserLL.cpp_ y es lo que guarda la tabla del ejercicio 4)

      > 1. programa → declaraciones sentencias <br>
      > 2. declaraciones → declaracion declaraciones' <br>
      > 3. declaraciones' → declaracion declaraciones' <br>
      > 4. declaraciones' → ε <br>
      > 5. declaracion → tipo lista_var **;** <br>
      > 6. tipo → **int** <br>
      > 7. tipo → **float** <br>
      > 8. lista_var → **identificador** lista_var' <br>
      > 9. lista_var' → **,** **identificador** lista_var' <br>
      > 10. lista_var' → ε <br>
      > 11. sentencias → asignacion sentencias' <br>
      > 12. sentencias → control <br>
      > 13. sentencias' → sentencias <br>
      > 14. sentencias' → ε <br>
      > 15. asignacion → **identificador** **=** expresion **;** <br>
      > 16. control → **if** **(** expresion **)** sentencias **else** sentencias <br>
      > 17. control → **while** **(** expresion **)** sentencias <br>
      > 18. expresion → termino expresion' <br>
      > 19. expresion' → **+** termino expresion' <br>
      > 20. expresion' → **-** termino expresion' <br>
      > 21. expresion' → ε <br>
      > 22. termino → factor termino' <br>
      > 23. termino' → __\*__ factor termino' <br>
      > 24. termino' → **/** factor termino' <br>
      > 25. termino' → ε <br>
      > 26. factor → **(** expresion **)** <br>
      > 27. factor → **identificador** <br>
      > 28. factor → **numero** <br>

      }

2. Mostrar en el archivo la construcción de los conjuntos FIRST de la gramática _G'_. (1 pt.)

    Usé las reglas vistas en clase: si `X` es terminal, FIRST(X) = {X}; si hay una producción `X → ε`, ε está en FIRST(X); y si `X → Y₁ Y₂ ... Yₖ`, se agrega FIRST(Y₁) sin ε, y solo si Y₁ deriva ε se sigue con Y₂, etc. Conviene empezar por los no terminales cuyas producciones inician con terminal e ir subiendo:

    * FIRST(tipo) = { int, float }, por las producciones 6 y 7.
    * FIRST(declaracion) = FIRST(tipo) = { int, float }, y `tipo` no deriva ε.
    * FIRST(declaraciones) = FIRST(declaracion) = { int, float }.
    * FIRST(declaraciones') = FIRST(declaracion) ∪ { ε } = { int, float, ε }.
    * FIRST(lista_var) = { identificador }.
    * FIRST(lista_var') = { `,`, ε }.
    * FIRST(asignacion) = { identificador }.
    * FIRST(control) = { if, while }.
    * FIRST(sentencias) = FIRST(asignacion) ∪ FIRST(control) = { identificador, if, while }.
    * FIRST(sentencias') = FIRST(sentencias) ∪ { ε } = { identificador, if, while, ε }.
    * FIRST(programa) = FIRST(declaraciones) = { int, float }, y como `declaraciones` no deriva ε ya no se agrega FIRST(sentencias).
    * FIRST(factor) = { (, identificador, numero }.
    * FIRST(termino) = FIRST(factor) = { (, identificador, numero }.
    * FIRST(expresion) = FIRST(termino) = { (, identificador, numero }.
    * FIRST(expresion') = { +, -, ε }.
    * FIRST(termino') = { \*, /, ε }.

3. Mostrar en el archivo la construcción de los conjuntos FOLLOW de la gramática _G'_. (1 pt.)

    Las reglas son: $ está en FOLLOW(S); si hay `A → α B β`, todo FIRST(β) menos ε está en FOLLOW(B); y si hay `A → α B` o β deriva ε, todo FOLLOW(A) está en FOLLOW(B). La última regla se aplica hasta que ya no cambie ningún conjunto, con esto tenemos:

    * FOLLOW(programa) = { $ }, por ser el símbolo inicial.
    * FOLLOW(declaraciones) = FIRST(sentencias) = { identificador, if, while }, por la producción 1.
    * FOLLOW(declaraciones') = FOLLOW(declaraciones) = { identificador, if, while }, por las producciones 2 y 3.
    * FOLLOW(declaracion) = (FIRST(declaraciones') − ε) ∪ FOLLOW(declaraciones') = { int, float, identificador, if, while }; se agrega FOLLOW porque `declaraciones'` deriva ε.
    * FOLLOW(tipo) = FIRST(lista_var) = { identificador }, por la producción 5.
    * FOLLOW(lista_var) = { ; }, por la producción 5.
    * FOLLOW(lista_var') = FOLLOW(lista_var) = { ; }.
    * FOLLOW(sentencias): por la producción 1 tiene FOLLOW(programa) = { $ }, por la 16 tiene `else` (la del _then_), y las que terminan en `sentencias` agregan FOLLOW(control). También, por la 13, FOLLOW(sentencias') ⊆ FOLLOW(sentencias).
    * FOLLOW(sentencias') = FOLLOW(sentencias), por la producción 11.
    * FOLLOW(control) = FOLLOW(sentencias), por la producción 12. Estos tres dependen entre sí y no aportan nada nuevo, entonces los tres son { else, $ }.
    * FOLLOW(asignacion) = (FIRST(sentencias') − ε) ∪ FOLLOW(sentencias) = { identificador, if, while, else, $ }.
    * FOLLOW(expresion) = { ; } por la 15, { ) } por las 16, 17 y 26, entonces { ), ; }.
    * FOLLOW(expresion') = FOLLOW(expresion) = { ), ; }.
    * FOLLOW(termino) = (FIRST(expresion') − ε) ∪ FOLLOW(expresion') = { +, -, ), ; }.
    * FOLLOW(termino') = FOLLOW(termino) = { +, -, ), ; }.
    * FOLLOW(factor) = (FIRST(termino') − ε) ∪ FOLLOW(termino') = { \*, /, +, -, ), ; }.

    Creo que se ve mejor como tablita:

    | No terminal | FIRST | FOLLOW |
    |-------------|-------|--------|
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

4. Mostrar en el archivo la construcción de la tabla de análisis sintáctico predictivo para _G'_. (1 pt.)

    Para cada producción `A → α`: se agrega la producción en M[A, a] por cada terminal `a` de FIRST(α), y si ε está en FIRST(α), también en M[A, b] por cada `b` de FOLLOW(A) (incluyendo $). Un ejemplo para que se vea como fui construyendo la tabla:

    * Producción 3, `declaraciones' → declaracion declaraciones'`: FIRST = { int, float }, entonces va en las columnas `int` y `float`.
    * Producción 4, `declaraciones' → ε`: va en FOLLOW(declaraciones') = { identificador, if, while }.
    * Producción 14, `sentencias' → ε`: va en FOLLOW(sentencias') = { else, $ }.
    * Producción 25, `termino' → ε`: va en FOLLOW(termino') = { +, -, ), ; }.

    La tabla queda así, cada número es la producción del ejercicio 1.d y las celdas vacías son error sintáctico (`id` es **identificador** y `num` es **numero**):

    | | id | num | int | float | if | else | while | , | ; | = | ( | ) | + | - | \* | / | $ |
    |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
    | **programa** | | | 1 | 1 | | | | | | | | | | | | | |
    | **declaraciones** | | | 2 | 2 | | | | | | | | | | | | | |
    | **declaraciones'** | 4 | | 3 | 3 | 4 | | 4 | | | | | | | | | | |
    | **declaracion** | | | 5 | 5 | | | | | | | | | | | | | |
    | **tipo** | | | 6 | 7 | | | | | | | | | | | | | |
    | **lista_var** | 8 | | | | | | | | | | | | | | | | |
    | **lista_var'** | | | | | | | | 9 | 10 | | | | | | | | |
    | **sentencias** | 11 | | | | 12 | | 12 | | | | | | | | | | |
    | **sentencias'** | 13 | | | | 13 | 14 | 13 | | | | | | | | | | 14 |
    | **asignacion** | 15 | | | | | | | | | | | | | | | | |
    | **control** | | | | | 16 | | 17 | | | | | | | | | | |
    | **expresion** | 18 | 18 | | | | | | | | | 18 | | | | | | |
    | **expresion'** | | | | | | | | | 21 | | | 21 | 19 | 20 | | | |
    | **termino** | 22 | 22 | | | | | | | | | 22 | | | | | | |
    | **termino'** | | | | | | | | | 25 | | | 25 | 25 | 25 | 23 | 24 | |
    | **factor** | 27 | 28 | | | | | | | | | 26 | | | | | | |

    Ninguna celda tiene dos producciones, por lo tanto _G'_ es LL(1)

5. Sustituir el contenido del Analizador Léxico (lexer.ll) con el implementado en la segunda práctica. (0.5 pts.)

    Copié _lexer.ll_ de la práctica 2, con las mismas macros para números e identificadores y las palabras reservadas antes de `{ids}` para que ganen el empate de longitud. También copié los comentarios de _headers/Lexer.hpp_. Lo único que cambió es el comentario de la regla `<<EOF>>`, proque en la plantilla de esta práctica `t_eof` vale 0, que es justo lo que regresa `yylex()` al acabarse la entrada, así que ya no es indispensable como en la práctica 2, pero la dejé para que quede explícito que ahí sale el símbolo `$`.

    La regla `.` sigue regresando `t_error`, que no está en la gramática ni en la tabla, así que el parser lo reporta como error de sintaxis en esa misma línea.

6. Definir en un comentario de _Symbols.hpp_ la gramática _G'_. (0.05 pts.)

    Está en el comentario del inicio de _headers/Symbols.hpp_, con _N'_, _T_, _S_ y _P'_ con la misma numeración del ejercicio 1.d.

7. Definir _Σ_ en un _**enum**_ de _Symbols.hpp_. (0.10 pts.)

    En el `enum Token` hay un valor por terminal con el prefijo `t_`, además `t_eof` que hace de `$`. Al final está `t_error`, que no es parte de _Σ_, este solo lo usa el lexer para avisar de un carácter inválido.

8. Definir _N'_ en un _**enum**_ de _Symbols.hpp_. (0.10 pts.)

    En el `enum NoTerm` hay un valor por no terminal con el prefijo `nt_` (las primas terminan en `P`, igual que las funciones de la práctica 2). El primero es `strEmpty`, que venía en la plantilla y representa a ε en los cuerpos de las producciones.

9. Cargar _N' ∪ Σ_ en _ParserLL.cpp_. (0.25 pts.)

    En `loadSyms()` se agrega cada símbolo a `syms`. Como la llave es un solo entero y `Token` y `NoTerm` empiezan los dos en 0, las llaves chocarían, entonces los terminales usan su valor del enum y los no terminales van después, a partir de `t_error` (cabe aclarar que no es símbolo de la gramática). Para eso hice la función auxiliar `N(NoTerm)` en _ParserLL.cpp_, que regresa `t_error + n`. Ahí mismo se asignan el número de terminales y no terminales, y el símbolo inicial.

10. Cargar _P'_ en _ParserLL.cpp_. (0.25 pts.)

    En `loadProds()` se agrega cada producción con su número del ejercicio 1.d; el encabezado y el cuerpo son las llaves de `syms`. Las producciones ε tienen como cuerpo solamente a `N(strEmpty)`. Pongo un pequeño ejemplo:

    ```c++
    grammar.addProd(5, Production(N(nt_declaracion), {N(nt_tipo), N(nt_lista_var), t_pyc}));
    grammar.addProd(10, Production(N(nt_lista_varP), {N(strEmpty)}));
    ```

11. Cargar la tabla de análisis sintáctico predictivo en _ParserLL.cpp_. (0.25 pts.)

    En `loadTable()` cada celda no vacía de la tabla del ejercicio 4 es una asignación `table[no terminal][terminal] = producción`, por ejemplo `table[nt_sentenciasP][t_else] = 14;`. Las celdas vacías simplemente no existen en el `map`, y así se detecta el error.

12. Implementar el algoritmo de análisis sintáctico de descenso predictivo en _ParserLL.cpp_ de modo que el programa acepte el archivo _prueba_. (4 pts.)

    `parse()` carga los símbolos, las producciones y la tabla, ya que _main.cpp_ solamente llama a `parse()`. Después mete `$` y el símbolo inicial a la pila, pide el primer token, y mientras la pila no esté vacía revisa el tope `X`:

    * Si `X` es **terminal**, debe coincidir con el token actual: se saca de la pila y se pide el siguiente token. Si no coincide es error. Cuando coincide `$` la pila queda vacía, por eso ya no se pide otro token.
    * Si `X` es **no terminal**, se busca `M[X, token]`. Si no hay entrada es error; si la hay, se saca `X` y se mete el cuerpo de la producción **al revés**, para que el primer símbolo del cuerpo quede en el tope. Si el cuerpo es ε no se mete nada.

    Si se vacía la pila sin errores, se llama a `aceptar()`. A diferencia de la práctica 2, ya no hay una función por no terminal: todo el conocimiento de la gramática está en los datos (`prods` y `table`), y la pila explícita hace lo que antes hacía la pila de llamadas, que la verdad yo siento más fácil como esta última práctica.

    `error()` imprime `ERROR DE SINTAXIS`, lo que se esperaba y la línea, y termina el programa en el primer error, igual que en la práctica 2. Para decir qué se esperaba decide si el tope es un terminal, se imprime ese terminal; si es un no terminal, se imprimen todas las columnas que tienen entrada en su renglón de la tabla. Con el archivo _prueba_ la salida es:

    ```
    $ ./compiler prueba
    La cadena es aceptada
    ```

---
#### Extras

13. Documentar el código. (0.25pts)
    Lo hice

14. Proponer 4 archivos de prueba nuevos, 2 válidos y 2 inválidos. (0.25pts)

    Agregué cuatro archivos en _src/_, que se ejecutan con `./compiler <archivo>` o todos juntos con `make pruebas`:

    * **_prueba_valida1_**: varias variables por declaración, una expresión con paréntesis, un flotante con exponente y un `while` cuyo cuerpo son tres asignaciones (por la convención del ejercicio 1.a).
    * **_prueba_valida2_**: un `if-else` con paréntesis anidados en la condición, un `while` en el _then_ y otro `if-else` en el `else`. 

    * **_prueba_invalida1_**: un programa sin declaraciones, que la gramática no permite porque `declaraciones` no deriva ε. El tope es `declaraciones` y no hay entrada en la columna `identificador`.
    * **_prueba_invalida2_**: un `if` sin `else`. El error sale en la línea 6 porque el archivo termina con un salto de línea después de la línea 5.

    ```
    ===== prueba_invalida1 =====
    ERROR DE SINTAXIS se esperaba alguno de: 'int' 'float' en la línea 1
    La cadena no pertenece al lenguaje generado por la gramática
    ===== prueba_invalida2 =====
    ERROR DE SINTAXIS se esperaba 'else' en la línea 6
    La cadena no pertenece al lenguaje generado por la gramática
    ```

    Para que esto funcionara, en el _Makefile_ cambié el ejecutable de `comp` a `compiler`, que es el nombre que se usa en la sección de uso, y agregué el objetivo `pruebas`. Como las pruebas inválidas terminan con `EXIT_FAILURE`, le puse `|| true` para que `make` no marque error y siga con las demás.
