#include <stdio.h>
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "definiciones.h"
#include "TS.h"

int main() {
    // Inicializar tabla de simbolos
    ComponenteLexico* TS = NULL;
    int tamTS = 35;
    initTS(TS);
    printf("Iniciado\n");
    printf("%s\n", TS[0].cadena);
    printf("Imprimiendo...\n");
    imprimirTS(TS, 35);

    return 0;
}