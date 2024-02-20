#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "TS.h"



void initTS(ComponenteLexico* TS){
    // Reservamos memoria para los 35 componentes lexicos de las
    // palabras reservadas
    TS = (ComponenteLexico*)malloc(35 * sizeof(ComponenteLexico));
    
    // Reservamos memoria para cada una de las cadenas de las palabras reservadas
    for (int i = 0; i < 35; i++){
        TS[i].cadena = (char*)malloc(10 * sizeof(char));
    }

    // Definimos las palabras reservadas de Python
    char* palabrasReservadas[35] = { 
        "False", "None", "True", "and", "as", "assert", "async", "await", "break", 
        "class", "continue", "def", "del", "elif", "else", "except", "finally", 
        "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", 
        "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"
    };
    
    int token[35] = {
        FALSE, NONE, TRUE, AND, AS, ASSERT, ASYNC, AWAIT, BREAK, 
        CLASS, CONTINUE, DEF, DEL, ELIF, ELSE, EXCEPT, FINALLY, 
        FOR, FROM, GLOBAL, IF, IMPORT, IN, IS, LAMBDA, NONLOCAL, 
        NOT, OR, PASS, RAISE, RETURN, TRY, WHILE, WITH, YIELD
    };

    // Asignamos las palabras reservadas a la tabla de simbolos
    for (int i = 0; i < 35; i++){
        TS[i].cadena = palabrasReservadas[i];
        printf("%s ", TS[i].cadena);
        TS[i].token = token[i];
        printf("%d\n", TS[i].token);
    }

}

void imprimirTS (ComponenteLexico* TS, int tamTS){
    for (int i = 0; i < tamTS; i++){
        printf("a: \n");
        printf("%s: %d\n", TS[i].cadena, TS[i].token);
    }
    
}