#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "TS.h"


/*
void initTS(ComponenteLexico** TS){
    // Reservamos memoria para los 35 componentes lexicos de las
    // palabras reservadas
    (*TS) = (ComponenteLexico*)malloc(47 * sizeof(ComponenteLexico));
    if ((*TS) == NULL){
        printf("Error al reservar memoria\n");
        exit(1);
    }

    // Definimos las palabras reservadas de Python
    char* palabrasReservadas[35] = { 
        "False", "None", "True", "and", "as", "assert", "async", "await", "break", 
        "class", "continue", "def", "del", "elif", "else", "except", "finally", 
        "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", 
        "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"
    };

    char* delimitadores[12] = {
        "(", ")", "[", "]", "{", "}", ",", ":", ".", " ", "\n", "EOF"
    };
    
    int token[35] = {
        FALSE, NONE, TRUE, AND, AS, ASSERT, ASYNC, AWAIT, BREAK, 
        CLASS, CONTINUE, DEF, DEL, ELIF, ELSE, EXCEPT, FINALLY, 
        FOR, FROM, GLOBAL, IF, IMPORT, IN, IS, LAMBDA, NONLOCAL, 
        NOT, OR, PASS, RAISE, RETURN, TRY, WHILE, WITH, YIELD
    };

    // Asignamos y reservamos la memoria para los delimitadores
    for (int i = 0; i < 12; i++){
        // Reservamos memoria para cada palabra reservada
        (*TS)[i].cadena = (char*)malloc(10 * sizeof(char));
        if ((*TS)[i].cadena == NULL){
            printf("Error al reservar memoria\n");
            exit(1);
        }
        // Asignamos la palabra reservada y su token
        (*TS)[i].cadena = delimitadores[i];
        (*TS)[i].token = delimitadores[i][0];
    }
    // Asignamos y reservamos la memoria para las palabras 
    // reservadas a la tabla de simbolos
    for (int i = 12; i < 47; i++){
        // Reservamos memoria para cada palabra reservada
        (*TS)[i].cadena = (char*)malloc(10 * sizeof(char));
        if ((*TS)[i].cadena == NULL){
            printf("Error al reservar memoria\n");
            exit(1);
        }
        // Asignamos la palabra reservada y su token
        (*TS)[i].cadena = palabrasReservadas[i-12];
        (*TS)[i].token = token[i-12];
    }


}

void imprimirTS (ComponenteLexico* TS, int tamTS){
    for (int i = 0; i < tamTS; i++){
        printf("%s: %d\n", TS[i].cadena, TS[i].token);
    }
    
}
*/

void initTS(abin *TS){
    crear(TS);
    insertar(TS, DEF, "def");
    insertar(TS, IMPORT, "import");
    insertar(TS, AS, "as");
    insertar(TS, FOR, "for");
    insertar(TS, IN, "in");
    insertar(TS, IF, "if");
    insertar(TS, ELIF, "elif");
    insertar(TS, ELSE, "else");
    insertar(TS, RETURN, "return");
    insertar(TS, NOT, "not");
    insertar(TS, ID, "id");
}

void imprimirTS (abin TS){
    imprimir(TS);
}

void destruirTS(abin* TS){
    destruir(TS);
}