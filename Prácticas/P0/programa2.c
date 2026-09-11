#include <stdio.h>

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
