#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"

#define CENTINELA1 TAM_MAX_LEXEMA
#define CENTINELA2 TAM_MAX_LEXEMA*2+1
#define TAMANO_DOBLE_BUFFER TAM_MAX_LEXEMA*2+2
#define INICIO_BLOQUE2 TAM_MAX_LEXEMA+1 

/*
Se va a utilizar el método del doble buffer con un unico array
pero se trabajara como dos bloques logicos
*/

typedef struct{
    char buffer[TAMANO_DOBLE_BUFFER];
    short inicio; // Indice de la posicion del primer caracter del lexema
    short delantero; // Indice de la posición del caracter que se está procesando
}dobleBuffer;


void inicializarSistemaEntrada(char* nombreArchivo);

char siguienteCaracter();

void retrocederCaracter();

void finalizarSistemaEntrada();