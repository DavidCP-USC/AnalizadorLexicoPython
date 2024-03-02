#include "abin.h"
/*
// Definimos un struct
typedef struct {
    char* cadena;
    int token;
}ComponenteLexico;

void initTS(ComponenteLexico** TS);

void imprimirTS (ComponenteLexico* TS, int tamTS);
*/

void initTS(abin* TS);

void imprimirTS (abin TS);

void destruirTS(abin* TS);