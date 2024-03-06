#include <stdio.h>
#include "AnalizadorLexico.h"
#include "definiciones.h"
#include <unistd.h>

void analisisSintactico(FILE *archivo, abin TS){
    tipoelem componenteLexico;
    do{
        componenteLexico = siguienteComponenteLexico(archivo, TS);
        printf("Componente lexico - Lexema %d - Valor %s\n", componenteLexico.valor, componenteLexico.lexema);
        sleep(1);
    }while (componenteLexico.valor != EOF);
    printf("--- FIN DEL ANALISIS SINTATICO ---\n");
}