#include "Errores.h"

void printTipoError(int tipo, char *error){
    switch(tipo){
        case ERROR_TS:
            printf("Error con la TS\n");
            break;
        case ERROR_ANALIZADOR_LEXICO:
            printf("Error en el analizador lexico\n");
            break;
        case ERROR_ANALIZADOR_SINTACTICO:
            printf("Error en el analizador sintactico\n");
            break;
        case ERROR_SISTEMA_DE_ENTRADA:
            printf("Error en el sistema de entrada\n");
            break;
        default:
            printf("Error desconocido\n");
            break;
    }
    if (error != NULL){
        printf("Error: %s\n", error);
    }    
}
