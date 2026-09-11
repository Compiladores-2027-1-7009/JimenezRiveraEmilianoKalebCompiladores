<p  align="center">
  <img  width="200"  src="https://www.fciencias.unam.mx/sites/default/files/logoFC_2.png"  alt="">  <br>Compiladores  2027-1 <br>
  Práctica 0: Sistema de procesamiento de Lenguaje <br> Profesora: Ariel Adara Mercado Martínez
</p>

### Sistema de procesamiento de Lenguaje
### Objetivo:
Comprender y analizar el funcionamiento de los diferentes programas que intervienen en el proceso de traducción de un programa fuente a un programa ejecutable.

### Introducción
Un compilador es un programa que traduce un programa fuente escrito en un lenguaje de alto nivel a un
programa en lenguaje objeto, usualmente de bajo nivel. Este proceso no se realiza de manera aislada; el
compilador colabora con otros programas como el preprocesador, ensamblador y enlazador.
El preprocesador recopila y expande macros y otros fragmentos de código abreviado en el programa
fuente. Luego, el compilador transforma el código preprocesado en un programa objeto en lenguaje ensamblador, que es posteriormente convertido en código máquina por el ensamblador. Finalmente, el enlazador combina los archivos de código máquina y bibliotecas necesarias para producir un programa ejecutable. El cargador lleva este programa ejecutable a la memoria para su ejecución.

```mermaid
flowchart TD
subgraph 1
id1([Programa fuente]) --> Preprocesador  -- Programa Fuente modificado -->  Compilador  -- Programa objeto en lenguaje ensamblador -->  Ensamblador  -- Código máquina relocalizable --> Enlazador/Cargador --> id2([Código máquina destino])
end
subgraph  2  
id3[/Archivos de biblioteca y archivos objeto/] --> Enlazador/Cargador  
end  
 ```

### Desarrollo:

1. Deberá tener instalado el compilador _gcc_ y trabajar en un ámbiente _Linux_.
2. Escriba el siguiente programa en lenguaje **_C_** (sin copiar y pegar) y nómbrelo *programa.c*
```c
#include <stdio .h>
#include <stdlib.h>
//# define PI 3.1415926535897

# ifdef PI
# define area (r) (PI * r * r)
# else
# define area (r) (3.1416 * r * r)
# endif


/**
* Compiladores 2025-2
*
*/
int main ( void ) {
printf ("Hola Mundo !\n"); //Función para imprimir hola mundo
float mi_area = area (3) ; //soy un comentario... hasta donde llegaré ?
printf ("Resultado : %f\n", mi_area);
return 0;
}
```

3. Use el siguiente comando: `cpp programa.c programa.i`
Revise el contenido de _programa.i_ y conteste lo siguiente:
<ol type="a">
  <li>¿Qué ocurre cuando se invoca el comando <i>cpp</i> con esos argumentos?
  <p>Primero en mi mac no se pudo ejecutar el comando cpp programa.c programa.i en mi mac por el tipo de arquitectura, 
  y tuve que usar el comando clang -E programa.c -o programa.i y tras ejecutarlo se generanon dos archivos: programa-7f45e34b.i.tmp y 
  programa.i, tras investigar el comportamiento es muy similar y no debería presentar problemas con el desarrollo de la práctica<p/>
  </li>
  
  <li>¿Qué similitudes encuentra entre los archivos <i>programa.c</i> y <i>programa.i</i>?
  <p>Que ambos parecen estar escritos en lenguaje c, pues se manejan los mismos tipos de datos y sintaxis para declarar funcinoes, 
  variables y por otro lado que el prámbulo es mucho más grande, es como si hubiese inyectado el código de las primeras insturcciones
  con las librerías en el programa.c<p/>
  </li>

  <li>¿Qué pasa con las macros y los comentarios del código fuente original en <i>programa.i</i>?
  <p>Las macros desaparecen como tales: no queda ni rastro de <code>#define area(r) ...</code>, sino que cada lugar donde se usaba <code>area(3)</code>
  fue sustituido directamente por su expresión expandida, en mi caso <code>(3.1416 * 3 * 3)</code> porque <code>PI</code> estaba comentado y se tomó
  la rama del <code>#else</code>. Es decir, el preprocesador resuelve las macros en tiempo de preprocesamiento y el compilador ya nunca ve esas primeras líneas de código tal cual.
  Los comentarios, tanto los de bloque <code>/* ... */</code> como los de línea <code>//</code>, también se eliminan por completo del archivo
  de salida; no aparecen ni siquiera como líneas en blanco en el lugar donde estaban, por lo que a partir de este paso el código pierde toda
  la documentación que teniamso en el fuente original.<p/>
  </li>

  <li>Compare el contenido de <i>programa.i</i> con el de <i>stdio.h</i> e indique de forma general las similitudes entre ambos
  archivos.
  <p>Al revisar <i>programa.i</i> encontré que gran parte de su contenido (en mi caso casi todo el archivo, ya que <i>programa.i</i> terminó
  con 1742 líneas contra apenas 65 líneas del <i>stdio.h</i>) es prácticamente una copia del contenido de <i>stdio.h</i> y de
  los demás encabezados que este incluye de forma anidada (<i>_stdio.h</i>, <i>_types.h</i>, <i>cdefs.h</i>,  etc). Esto se debe
  a que <i>stdio.h</i> no es un archivo aislado, sino que hace <code># include</code> de otros archivos del sistema, y el
  preprocesador va insertando el contenido de cada uno de ellos de forma recursiva. Ambos archivos comparten la misma sintaxis de C (declaraciones
  de funciones, tipos, macros de disponibilidad de Apple como <code>__attribute__((availability(...)))</code>), y en <i>programa.i</i> se pueden
  reconocer literalmente las firmas de funciones como <code>printf</code>, <code>scanf</code>, <code>fopen</code>, etc., tal cual aparecen declaradas
  en <i>stdio.h</i>. La diferencia es que <i>programa.i</i> además incluye, al final, mi propio código fuente ya sin macros ni comentarios, y unas
  líneas que empiezan con <code>#</code> seguidas de un número y una ruta de archivo (p. ej. <code># 1 "programa.c"</code>), que son marcas de línea
  que usa el preprocesador para indicarle al compilador de qué archivo y de qué línea original proviene cada fragmento de código, útil para que los
  mensajes de error señalen el archivo correcto y no el <i>.i</i> generado.<p/>

  </li>
  <li>¿A qué etapa corresponde este proceso?
  <p>Corresonde a la etapa de preprocesamiento del programa fuente para transformarlo en programa fuente modificado, y es donde se inyecta código.
  <p/>
  </li>
</ol>

---

1. Ejecute la siguiente instrucción: ``gcc -Wall -S programa.i``
<ol type="a">
  <li>¿Para qué sirve la opción <i>-Wall</i>?
  <p><i>-Wall</i> activa la mayoría de los warnings (advertencias) del compilador, es decir, le pide que además de detectar errores que impiden
  compilar también avise sobre construcciones que son válidas mas sospechosas o propensas a bugs, como variables sin usar, comparaciones raras entre
  signed y unsigned, funciones sin prototipo, etc. En mi caso al ejecutar <code>gcc -Wall -S programa.i</code> no se mostró ningún warning, lo cual
  tiene sentido porque el código ya venía preprocesado y según yo es bastante simple.<p/>
  </li>
  <li>¿Qué le indica a gcc la opción <i>-S</i>?
  <p>Le indica al compilador que se detenga justo después de la fase de compilación propiamente dicha, es decir, que genere el código en lenguaje
  ensamblador correspondiente al programa fuente pero que no continúe con el ensamblado ni con el enlazado. Sin <i>-S</i>, gcc seguiría invocando
  automáticamente al ensamblador y al enlazador hasta producir un ejecutable.<p/>
  </li>
  <li>¿Qué contiene el archivo de salida y cuál es su extensión?
  <p>El archivo de salida es <i>programa.s</i>, con extensión <i>.s</i>, y contiene el código en lenguaje ensamblador equivalente al programa hecho en C.
  En mi caso, como estoy en una Mac con procesador Apple Silicon  i.e. arquitectura arm64 y no en Linux x86-64, el ensamblador generado usa instrucciones
  ARM64 (por ejemplo <code>stp</code>, <code>ldp</code>, <code>adrp</code>, <code>bl _printf</code>) y directivas comúnes como Mach-O
  (<code>.build_version macos</code>, <code>.section __TEXT,__text</code>), en lugar del ensamblador AT&T/x86-64 y formato ELF que se vería en una máquina Linux; el contenido conceptual (instrucciones para imprimir el string y calcular el área) es el mismo, solo cambia la arquitectura de
  destino.<p/>
  </li>
  <li>¿A qué etapa corresponde este comando?
  <p>Corresponde a la etapa de compilación propiamente dicha: es el paso en el que el compilador toma el programa fuente ya preprocesado y lo
  traduce a un programa objeto en lenguaje ensamblador.<p/>
  </li>
</ol>

---

1. Ejecute la siguiente instrucción: `as programa.s -o programa.o`
<ol type="a">
  <li> Antes de revisarlo, indique cuál es su hipótesis sobre lo que debe contener el archivo con extensión  <i>.o</i>.
  <p>Mi hipótesis, antes de revisarlo, es que <i>programa.o</i> debe contener código máquina, es decir, las instrucciones del ensamblador
  <i>programa.s</i> ya traducidas a binario (ceros y unos que el procesador puede ejecutar directamente), pero que todavía no debe ser un
  ejecutable completo, i.e. le deben faltar cosas como las direcciones finales en memoria y la definición
  de funciones externas que uso pero no defino, como <code>printf</code>, ya que esas se resuelven hasta el enlazado.<p/>
  </li>
  <li> Diga de forma general qué contiene el archivo <i>programa.o</i> y por qué se visualiza de esa manera.
  <p>Al ejecutar el comando y revisar <i>programa.o</i> con <code>file</code> y <code>xxd</code> confirmamos que es un archivo binario, no texto
  legible: contiene código máquina (en mi caso arm64, ya que estoy en Mac) más una tabla de símbolos y metadatos para el enlazador, todo
  empaquetado en un formato de archivo objeto. En mi sistema el formato es Mach-O (así lo reporta <code>file programa.o</code>: "Mach-O 64-bit
  object arm64", y empieza con unoa bytes <code>cffa edfe</code>), que es el formato de archivos objeto y ejecutables de macOS, análogo
  al formato ELF que se usaría en Linux como ya se había mencionado. Se visualiza como datos binarios sin sentido al abrirlo como texto porque justamente ya no es código
  fuente ni ensamblador legible por humanos, sino instrucciones de máquina codificadas en binario junto con una cabecera y secciones
  (<code>__TEXT</code>, símbolos, etc.) que describen esas instrucciones para que el ensamblador y el enlazador sepan interpretarlas.<p/>
  </li>
  <li> ¿Qué programa se invoca con  <i>as</i>?
  <p>Se invoca al ensamblador del sistema. Para
  traducir el código en lenguaje ensamblador a código máquina.<p/>
  </li>
  <li> ¿A qué etapa corresponde la llamada a este programa?
  <p>Corresponde a la etapa de ensamblado: es el paso en el que el programa objeto en lenguaje ensamblador se convierte en código máquina
  relocalizable, tal como se muestra en el diagrama de la introducción entre el Compilador y el Enlazador/Cargador.<p/>
  </li>
</ol>

---

1. Encuentre la ruta de los siguientes archivos en el equipo de trabajo:
* ld-linux-x86-64.so.2
* Scrt1.o (o bien, crt1.o)
* crti.o
* crtbeginS.o
* crtendS.o
* crtn.o
1a
<p>Como ya venía advirtiendo desde la primer pregunta, estoy trabajando en una Mac con Apple Silicon (arm64) y no en Linux x86-64, así que hice una
búsqueda completa en todo el sistema de archivos (<code>find / -iname "crt1.o"</code>, etc.) y ninguno de estos archivos
existe en mi equipo: ni <i>ld-linux-x86-64.so.2</i> ni los distintos <i>crt*.o</i>. Estos archivos son específicos de la cadena de herramientas
GNU/Linux (glibc + binutils): en otras palabras<i>ld-linux-x86-64.so.2</i> es el enlazador dinámico en tiempo de ejecución de Linux para x86-64 (el programa que
carga las bibliotecas compartidas cuando arranca un ejecutable), y los <i>crt*.o</i> (C RunTime) son pequeños objetos que glibc antepone y
agrega al final de cada programa para inicializar y finalizar correctamente el entorno de ejecución en C antes de llamar a <code>main</code>
y después de que este retorna. macOS no expone estos archivos como objetos sueltos en <i>/usr/lib</i>; tras una búsquedo sobre las especificaciones en la máquina, en su lugar usa su propio formato
Mach-O y su propio enlazador dinámico, que es el binario <i>/usr/lib/dyld</i> (confirmé con <code>file /usr/lib/dyld</code> que es justamente
un "Mach-O 64-bit dynamic linker"), el cual cumple un rol equivalente al de <i>ld-linux-x86-64.so.2</i> pero para el ecosistema de Apple. La
inicialización que en Linux hacen los <i>crt*.o</i> por separado, en macOS la resuelve internamente el propio <code>ld</code> del sistema al
enlazar contra la biblioteca <i>libSystem</i>, sin que el usuario tenga que indicar esos objetos de forma manual.<p/>

---

7. Ejecute el siguiente comando, sustituyendo las rutas que encontró en el paso anterior:
```bash
ld -o ejecutable -dynamic-linker /lib/ld-linux-x86-64.so.2 /usr/lib/Scrt1.o /usr/lib/crti.o programa.o -lc /usr/lib/crtn.o
```

<ol type="a">
  <li> En caso de que el comando ld mande errores, investigue como enlazar un programa utilizando el comando <i>ld</i>. Y proponga una posible solución para llevar a cabo este proceso con éxito.
  <p>Al ejecutar el comando tal cual lo indica la práctica obtuve el error <code>ld: unknown options: -dynamic-linker</code>,
  porque el <code>ld</code> de mi Mac es el enlazador de Apple (versión <i>ld-1267</i>, que soporta arquitecturas como arm64, x86_64, etc.) y no
  el enlazador GNU de binutils que se usa en Linux, por lo que ni siquiera reconoce esa bandera ni tiene sentido pasarle rutas que no existen en
  el sistema (como ya mencioné en la pregunta aterior). Investigando encontré que en macOS la forma equivalente de enlazar un objeto relocalizable en
  Mach-O a mano con <code>ld</code> es indicarle la biblioteca del sistema (<i>libSystem</i>, que en macOS agrupa lo que en Linux vendría en
  <i>libc</i> más otras bibliotecas del sistema), la raíz del SDK con <i>-syslibroot</i>, el punto de entrada y la arquitectura/versión de
  plataforma de destino. El comando que usé y que sí funcionó fue:
  <pre><code>SDK=$(xcrun --show-sdk-path)
ld -o ejecutable programa.o -lSystem -syslibroot "$SDK" -e _main -arch arm64 -platform_version macos 26.0 26.0</code></pre>
  Aquí <i>-lSystem</i> enlaza contra la biblioteca del sistema (equivalente a <i>-lc</i> en Linux), <i>-syslibroot</i> le indica a <code>ld</code>
  dónde buscar las bibliotecas del SDK de macOS, <i>-e _main</i> especifica que el punto de entrada es el símbolo <code>_main</code> (en Mach-O
  los símbolos de C llevan un guion bajo antepuesto) y <i>-platform_version</i> evita un warning.<p/>
  </li>
  <li> Describa el resultado obtenido al ejecutar el comando anterior.
  <p>Con el comando original del enunciado el resultado fue un error y no se generó ningún archivo <i>ejecutable</i>, tal como se explicó en el
  inciso anterior. Con el comando adaptado a macOS que propuse sí se generó exitosamente el archivo <i>ejecutable</i>, un binario Mach-O de 64
  bits para arm64 (confirmé con <code>file ejecutable</code>) con permisos de ejecución, listo para correr en la terminal.<p/>
  </li>
</ol>

---
8. Una vez que se enlazó el código máquina relocalizable, podemos ejecutar el programa con la siguiente
instrucción en la terminal: ```./ejecutable```

<p>Al ejecutar <code>./ejecutable</code> en mi terminal obtuve la siguiente salida:</p>
<pre><code>Hola Mundo !
Restulado : 28.274401</code></pre>
<p>El valor 28.274401 corresponde al área de un círculo de radio 3 usando el valor de la rama <code>#else</code> de la macro, es decir
<code>3.1416 * 3 * 3</code>, ya que en el archivo fuente original la línea <code>#define PI</code> estaba comentada y por lo tanto la
condición <code>#ifdef PI</code> era falsa. Además, el código de salida del programa fue 0 (<code>return 0;</code>), indicando que terminó sin
errores.<p/>

---

9. Quite el comentario de la macro _#define PI_ en el código fuente original y conteste lo siguiente:
<ol type="a">
  <li> Genere nuevamente el archivo.i. De preferencia asigne un nuevo nombre.
  <p>Quité el comentario dejando <code>#define PI 3.141592</code> activo y generé un nuevo archivo con el comando
  <code>clang -E programa.c -o programa_pi.i</code> (equivalente en mi Mac al <code>cpp programa.c programa_pi.i</code> original), guardándolo
  con un nombre distinto para no perder el <i>programa.i</i> del ejercicio anterior.<p/>
  </li>
  <li> ¿Cambia en algo la ejecución final?
  <p>Sí, y de una forma que no me esperaba: en lugar de solo cambiar el resultado numérico del área, <b>la compilación se rompe por completo</b>.
  Al revisar <i>programa_pi.i</i> encontré que la línea de <code>mi_area</code> quedó expandida como
  <code>float mi_area = (r) (3.141592 * r * r) (3);</code>, con <code>r</code> suelta y sin definir, en vez de sustituir <code>r</code> por
  <code>3</code> como uno esperaría de una macro de este estilo función. Lo que pasa es que
  <code># define area (r) (PI * r * r)</code> tiene un <b>espacio entre <code>area</code> y el paréntesis de apertura</b>. Para el preprocesador
  de C, si hay un espacio ahí, <code>area</code> deja de interpretarse como una macro tipo función con parámetro <code>r</code> y pasa a ser una
  macro-objeto simple cuyo cuerpo de reemplazo es literalmente el texto <code>(r) (PI * r * r)</code>; por eso al usar <code>area (3)</code> el
  preprocesador solo sustituye <code>area</code> por ese texto y dejó el <code>(3)</code> pegado al final tal cual, como si fuera una llamada a
  función sobre el resultado de esa expresión. En la rama <code>#else</code> (la que usé en las preguntas anteriores) la macro sí estaba bien
  definida como <code>#define area(r) (3.1416 * r * r)</code>, <b>sin espacio</b>, por eso ahí sí funcionaba como macro tipo función. Al intentar
  compilar <i>programa_pi.i</i> con <code>gcc -Wall -S</code> obtuve el error <code>use of undeclared identifier 'r'</code> en las tres apariciones
  de <code>r</code>, confirmando el problema. Esto me pareció un hallazgo interesante de la práctica y que me servirá para las futuras: demuestra por qué es tan importante no dejar
  espacio entre el nombre de una macro tipo función y su paréntesis de parámetros al definirla con <i>cpp</i>.<p/>
  </li>
</ol>

---

10. Escribe un segundo programa en lenguaje **_C_** en el que agregue 4 directivas del preprocesador
de _**C**_ (_cpp_)[^1]. Las directivas elegidas deben jugar algún papel en el significado del programa, ser distintas entre sí y
diferentes de las utilizadas en el primer programa (aunque no están prohibidas si las requieren). 
<ol type="a">
    <li>Explique su utilidad
general y su función en particular para su programa.

<p>Escribí <i>programa2.c</i> con el siguiente contenido:</p>

<pre><code>#include &lt;stdio.h&gt;

#define VERSION 2

#pragma message "Compilando programa2.c con directivas de preprocesador"

#if VERSION == 1
    #define SALUDO "Version 1: Hola"
#elif VERSION == 2
    #define SALUDO "Version 2: Hola de nuevo"
#else
    #error "VERSION no soportada"
#endif

#undef VERSION

int main(void) {
    printf("%s\n", SALUDO);

#ifdef VERSION
    printf("VERSION sigue definida\n");
#else
    printf("VERSION ya fue eliminada con #undef\n");
#endif

    return 0;
}
</code></pre>

<p>Lo compilé y ejecuté con <code>gcc -Wall programa2.c -o ejecutable2</code> y <code>./ejecutable2</code>, obteniendo:</p>
<pre><code>Version 2: Hola de nuevo
VERSION ya fue eliminada con #undef</code></pre>

<p>Las 4 directivas que utilicé, distintas entre sí y distintas de las usadas en <i>programa.c</i> (que solo usaba <code>#include</code>,
<code>#define</code> e <code>#ifdef</code>/<code>#else</code>/<code>#endif</code>), fueron:</p>

<ul>
<li><b><code>#if</code> / <code>#elif</code> / <code>#else</code> / <code>#endif</code></b>: en general sirven para incluir o excluir bloques
de código de forma condicional según el valor de una expresión constante evaluada en tiempo de preprocesamiento (a diferencia de
<code>#ifdef</code>, que solo revisa si un identificador está definido, <code>#if</code> puede evaluar expresiones como comparaciones
numéricas). En mi programa las usé para seleccionar, según el valor numérico de la macro <code>VERSION</code>, cuál de dos posibles mensajes
de saludo se define en <code>SALUDO</code>, simulando algo parecido a una compilación condicional por versiones.</li>

<li><b><code>#error</code></b>: sirve para forzar que la compilación se detenga inmediatamente con un mensaje de error personalizado cuando se
cumple una condición que el programador considera inválida, muy útil para detectar configuraciones no soportadas antes de que el compilador
intente generar código con datos incoherentes. En mi programa la coloqué en la rama final del <code>#if/#elif/#else</code>, de forma que si
<code>VERSION</code> tomara un valor distinto de 1 o 2 la compilación fallaría de inmediato con el mensaje "VERSION no soportada" en vez de
compilar silenciosamente con un comportamiento indefinido.</li>

<li><b><code>#undef</code></b>: sirve para eliminar la definición de una macro previamente definida con <code>#define</code>, de modo que a
partir de ese punto el preprocesador ya no la reconozca como tal (y por ejemplo un <code>#ifdef</code> posterior sobre ella sea falso). En mi
programa usé <code>#undef VERSION</code> justo después de haberla usado para decidir el saludo, para "liberar" ese nombre y demostrar
su efecto: el <code>#ifdef VERSION</code> dentro de <code>main</code> toma la rama <code>#else</code> porque la macro ya no existe, y el
programa imprime "VERSION ya fue eliminada con #undef".</li>

<li><b><code>#pragma</code></b>: sirve para dar instrucciones especiales, específicas del compilador, que no forman parte del estándar
general de C pero que controlan su comportamiento (por ejemplo, deshabilitar warnings, cambiar el alineamiento de estructuras, etc). En mi
programa usé <code>#pragma message "..."</code> para que, al compilar, gcc/clang impriman un mensaje informativo en la terminal (yo lo vi como
un warning con la bandera <code>-W#pragma-messages</code>), simplemente para dejar constancia de que ese archivo se está compilando con
soporte de estas directivas; no afecta el comportamiento en tiempo de ejecución del programa, solo el proceso de compilación.</li>
</ul>
<p/>
    </li>
</ol>

---

11. Redacte un informe detallado con sus resultados y conclusiones.

<p>A lo largo de esta práctica reconstruí, paso a paso y de forma manual, todo el proceso que normalmente <code>gcc</code> realiza de forma
transparente con un solo comando: preprocesamiento (<i>cpp</i>/<code>clang -E</code>), compilación a ensamblador (<code>gcc -S</code>),
ensamblado a código máquina relocalizable (<code>as</code>) y enlazado (<code>ld</code>) hasta obtener un ejecutable que corrí con éxito y que
produjo la salida esperada ("Hola Mundo !" y el área calculada). Un punto importante a señalar es que desarrollé la práctica en una Mac con
procesador Apple Silicon (arm64) en vez de en un Linux x86-64 porque no tengo y como sugieren las instrucciones, así que en varios pasos las herramientas involucradas
(<code>cpp</code>, <code>as</code>, <code>ld</code>) resultaron ser en realidad las de Apple (LLVM/clang y el enlazador propio de Apple) en vez
de las de GNU/binutils y glibc, y el formato de los archivos objeto y ejecutables fue Mach-O en vez de ELF. Esto me permitió, de hecho, comparar
directamente ambos ecosistemas: entendí que aunque las herramientas y nombres de archivo cambien (<i>ld-linux-x86-64.so.2</i> vs
<i>/usr/lib/dyld</i>, los <i>crt*.o</i> de glibc vs la resolución interna de <code>-lSystem</code> en macOS), el proceso conceptual de
traducción de código fuente a ejecutable es el mismo en cualquier sistema: preprocesar, compilar, ensamblar y enlazar.</p>

<p>Del análisis de <i>programa.i</i> concluí que el preprocesador es, en esencia, un motor de sustitución de texto: expande macros, resuelve
directivas condicionales, elimina comentarios e inserta literalmente el contenido de los archivos incluidos con <code>#include</code>, lo cual
explica por qué un archivo tan pequeño como <i>programa.c</i> (apenas 22 líneas) se convirtió en un archivo de más de 1700 líneas una vez
preprocesado: la enorme mayoría de ese contenido proviene de <i>stdio.h</i>, <i>stdlib.h</i> y los encabezados que estos incluyen a su vez de
forma anidada.</p>

<p>El hallazgo más valioso de la práctica desde mi punto de vista fue el del paso 9: al activar la macro <code>PI</code>, la definición
<code>#define area (r) (PI * r * r)</code> (con un espacio antes del paréntesis) dejó de comportarse como una macro tipo función y pasó a ser
una macro-objeto cuyo texto de reemplazo literal es <code>(r) (PI * r * r)</code>, lo cual rompió la compilación. Esto me dejó muy claro que en
<i>cpp</i> la presencia o ausencia de un espacio entre el identificador de una macro y su paréntesis de apertura cambia por completo el
significado de la definición, un detalle sintáctico fácil de pasar por alto pero con consecuencias importantes.</p>

<p>Finalmente, al escribir un segundo programa con las directivas <code>#if/#elif/#else/#endif</code>, <code>#error</code>, <code>#undef</code>
y <code>#pragma</code> pude entender que el preprocesador de C ofrece herramientas bastante más ricas que un simple buscar-y-reemplazar: permite
compilación condicional basada en expresiones (no solo en si algo está definido), validación temprana de configuraciones inválidas, control
explícito del ciclo de vida de una macro y comunicación de instrucciones especiales al compilador, todo antes de que exista siquiera un
código máquina.</p>


[^1]: Pueden consultar la lista de directivas en su documentación en línea: [CPP - Index of directives](https://gcc.gnu.org/onlinedocs/cpp/Index-of-Directives.html##Index-of-Directives). O bien, revisar la entrada para este preprocesador en la herramienta man en Linux: `$ man cpp`
