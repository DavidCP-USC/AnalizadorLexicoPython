#include <stdio.h>
#include "AnalizadorLexico.h"
#include "definiciones.h"
#include <unistd.h>
#include <stdlib.h>

void analisisSintactico(){
    tipoelem componenteLexico;
    do{
        componenteLexico = siguienteComponenteLexico();
        if (componenteLexico.valor == EOF){
            printf("Componente lexico -->\tLexema: %d\tValor: EOF\n", componenteLexico.valor);
            break;
        }
        else{
            printf("Componente lexico -->\tLexema: %d\tValor: %s\n", componenteLexico.valor, componenteLexico.lexema);
        }

    }while (componenteLexico.valor != EOF);
    liberarMemoria(&componenteLexico);

}