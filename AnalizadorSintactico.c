#include <stdio.h>
#include "AnalizadorLexico.h"
#include "definiciones.h"
#include <unistd.h>

void analisisSintactico(){
    tipoelem componenteLexico;
    do{
        componenteLexico = siguienteComponenteLexico();
        printf("Componente lexico -->\tLexema: %d\tValor: %s\n", componenteLexico.valor, componenteLexico.lexema);
        usleep(500);
    }while (componenteLexico.valor != EOF);
}