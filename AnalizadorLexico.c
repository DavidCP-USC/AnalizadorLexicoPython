#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "definiciones.h"
#include "AnalizadorLexico.h"
#include "SistemaEntrada.h"
#include "TS.h"

char lexema[TAM_MAX];
char buffer[TAM_MAX];
int indiceBuffer = 0;
char lexemasIndividuales[TAM_LEXEMAS_UNICARACTER] = "()[]{}.=,:;!&|";
tipoelem returnValue;

void _saltarComentario(char *caracter);
int  _identificarSiComentarioMultilinea(char *caracter) ;
void _saltarComentarioMultilinea(char *caracter);
int _esLexemaUnicaracter(char cadena);
void _recuperarLexema(int tipo, int retroceder);
void _identificarCadenasAlfanumericas(char *caracter);


tipoelem siguienteComponenteLexico(){
    short error = 0;
    short estado = 0;
    short terminado = 0;
    char caracter;


    while (!(terminado || error)){
        printf("\tEstado: %d\n", estado);
        switch (estado){
            case 0:
                caracter = siguienteCaracter();
                if (caracter == '#'){
                    estado = 1; // Automata de comentarios de una linea
                }
                else if(caracter == '"'){
                    if (_identificarSiComentarioMultilinea(&caracter)){
                        estado = 2; // Automata de comentarios multilinea
                    }
                    else{
                        estado = 5; // Automata de strings
                    }
                }
                else if (isalpha(caracter) || caracter == '_'){
                    estado = 3; // Automata de cadenas alfanumericas
                }
                else if (isdigit(caracter)){
                    estado = 4; // Automata de numeros
                }
                else if (caracter == '`'){ // Las " ya se han comprobado antes
                    estado = 5; // Automata de strings
                }
                else if(caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '<' || caracter == '>'){
                    estado = 6; // Automata de operadores
                }
                else if (_esLexemaUnicaracter(caracter)){
                    estado = 7; // Automata de lexemas de un caracter que no sean operadores ni letras
                }
                else if (caracter == EOF) { //si encontramos un EOF (esto funciona solo para el sistema de entrada simple)
                    returnValue.valor = EOF;
                    returnValue.lexema = "EOF";
                    terminado = 1;
                }
                else if(isblank(caracter) || caracter == '\n'){
                    // Puede haber multiples espacios en blanco o saltos de linea
                    // entre componentes lexicos, por lo que nos los saltamos
                    do{
                        caracter = siguienteCaracter();
                    }while (isblank(caracter) || caracter == '\n');
                    retrocederCaracter();
                    aceptarLexema();
                
                }
                else{
                    // GESTIONAR ERROR
                    // -- TODO --
                    printf("Error: Caracter no reconocido\n");
                    error = 1;
                }
                break;
            case 1: // Identificar comentarios
                _saltarComentario(&caracter);
                estado = 0;
                break;
            case 2: // Identificar comentarios multilinea
                _saltarComentarioMultilinea(&caracter);
                estado = 0;
                break;
            case 3: // Identificar cadenas
                _identificarCadenasAlfanumericas(&caracter);
                // Hemos identificado una cadena, terminamos
                terminado = 1;
                break;
            case 4: // Identificar numeros
            break;
            case 5: // Identificar strings
            break;
            case 6: // Identificar operadores
            break;
            case 7: // Identificar delimitadores
                _recuperarLexema(caracter, 0);
                aceptarLexema();
                terminado = 1;
            break;

        }
    }
               

    return returnValue;
}


void _saltarComentario(char *caracter){
    while (*caracter != '\n'){
        *caracter = siguienteCaracter();
    }
    // Hemos leido un comentario entero. No es un componente
    // lexico, pero la funcion aceptarLexema 
    // prepara el buffer para el siguiente componente lexico,
    // es decir, estamos descartando el comentario
    aceptarLexema();
}


int  _identificarSiComentarioMultilinea(char *caracter){    
    if (siguienteCaracter() == '"'){ // Posible comentario multilinea
        if (siguienteCaracter() == '"'){ // ES un comentario multilinea
            return 1;
        }
        else{
            retrocederCaracter();
            retrocederCaracter();
        }
    }
    else{
        retrocederCaracter();
    }
    

    return 0;
}


void _saltarComentarioMultilinea(char *caracter){
    while (1){
        *caracter = siguienteCaracter();        
        if (*caracter == '"'){
            *caracter = siguienteCaracter();
            if (*caracter == '"'){
                *caracter = siguienteCaracter();
                if (*caracter == '"'){
                    *caracter = siguienteCaracter();
                    if (*caracter == '\n'){ // Saltamos el \n final
                        // Hemos leido un comentario entero. No es un componente
                        // lexico, pero la funcion aceptarLexema 
                        // prepara el buffer para el siguiente componente lexico,
                        // es decir, estamos descartando el comentario
                        aceptarLexema();
                        break;
                    } 
                }
            }
        }
        if (*caracter == EOF){
            printf("Error: comentario multilinea no cerrado\n");
            break;
        }
    }
    
}


void _identificarCadenasAlfanumericas(char *caracter){
    do{
        printf("Caracter: %c\n", *caracter);
        *caracter = siguienteCaracter();
    }while(isalpha(*caracter) || isdigit(*caracter) || *caracter == '_');
    printf("Caracter: %c\n", *caracter);
    if (_esLexemaUnicaracter(*caracter) || isblank(*caracter) || *caracter == '\n' || *caracter == EOF){
        printf("Salto\n");
        _recuperarLexema(ID, 1);
    }
    else{
        _recuperarLexema(ID, 0);
    }
    printf("FIN\n");
}


void _recuperarLexema(int tipo, int retroceder){
    if (retroceder){
        printf("Retroceder\n");
        retrocederCaracter();
    }
    if (tipo == ID){
        printf("ID\n");
        obtenerLexema(&returnValue);
        returnValue.valor = obtenerValorTS(returnValue.lexema);
    }
    else{ // Para lexemas que solo tienen un caracter
        returnValue.valor = tipo;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(2 * sizeof(char));
        returnValue.lexema[0] = tipo;
        returnValue.lexema[1] = '\0';
    }
    aceptarLexema();
}


int _esLexemaUnicaracter(char cadena){
    for (int i = 0; i < TAM_LEXEMAS_UNICARACTER; i++){
        if (cadena == lexemasIndividuales[i]){
            return 1;
        }
    }
    return 0;
}
