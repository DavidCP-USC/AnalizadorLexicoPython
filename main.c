#include <stdio.h>
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "definiciones.h"
#include "TS.h"


int main() {
    // Abrimos el archivo
    FILE *archivo;
    archivo = fopen("wilcoxon.py", "r");
    if (archivo == NULL){
        printf("Error al abrir el archivo\n");
        return 1;
    }

    // Inicializar tabla de simbolos
    int tamTS = 47;
    ComponenteLexico* TS = NULL;

    // Inicializar tabla de simbolos
    initTS(&TS);
    // Imprimir tabla de simbolos
    imprimirTS(TS, tamTS);

    printf("Componentes lexicos:\n");
    char componenteLexicoActual[TAM_MAX];

    siguienteComponenteLexico(archivo, componenteLexicoActual);
    printf("Componente lexico: %s\n", componenteLexicoActual);
    


    return 0;
}