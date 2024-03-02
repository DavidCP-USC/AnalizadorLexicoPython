#include <stdio.h>
#include "AnalizadorLexico.h"
#include "definiciones.h"
#include "TS.h"
#include <unistd.h>

void analisisSintactico(FILE *archivo, abin TS){
    int componenteLexico = 0;
    while (componenteLexico != EOF){
        componenteLexico = siguienteComponenteLexico(archivo, TS);
        printf("Componente lexico: %d\n", componenteLexico);
        sleep(3);
    }
    printf("--- FIN DEL ANALISIS SINTATICO ---\n");
}