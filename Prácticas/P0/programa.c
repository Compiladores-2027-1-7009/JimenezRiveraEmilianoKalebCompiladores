#include <stdio.h>
#include <stdlib.h>
// define PI 3.141592


# ifdef PI
# define area (r) (PI * r * r)
# else
# define area(r) (3.1416 * r * r)
# endif


/***
 * Compiladores 2027-1
 ***/
int main (void) { 
    printf("Hola Mundo !\n"); // funcion para imprimir hola mundo
    float mi_area = area (3); //soy un comentario... hasta donde llegare?
    printf("Restulado : %f\n", mi_area);
    return 0;
}
