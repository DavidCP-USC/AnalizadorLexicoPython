#include <stdio.h>
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "definiciones.h"
#include "TS.h"


int main() {
    // Inicializar tabla de simbolos
    abin TS;
    initTS(&TS);

    // Abrimos el archivo
    FILE *archivo;
    archivo = fopen("wilcoxon.py", "r");
    if (archivo == NULL){
        printf("Error al abrir el archivo\n");
        return 1;
    }

    // Empezamos el analisis lexico
    printf("--- ANALISIS SINTATICO ---\n");
    analisisSintactico(archivo, TS);

    // Cerramos el archivo y liberamos memoria
    printf("-- Liberando memoria... --\n");
    fclose(archivo);
    destruirTS(&TS);

    printf("-- Fin del programa --\n");
    return 0;
}