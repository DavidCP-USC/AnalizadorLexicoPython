#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "definiciones.h"
#include "AnalizadorLexico.h"
#include "SistemaEntrada.h"

char lexema[TAM_MAX];
char buffer[TAM_MAX];
int indiceBuffer = 0;
char delimitadores[TAM_DELIMITERS] = "()[]{},:. \n\0";
// EOF TODO

void _saltarComentario(char *caracter);
//int  _identificarSiComentarioMultilinea(char *caracter);
void _saltarComentarioMultilinea(char *caracter);
//void _identificarID(char *caracter);
int _esDelimitador(char cadena);


tipoelem siguienteComponenteLexico(abin TS){
    short error = 0;
    short estado = 0;
    short terminado = 0;
    char caracter;
    tipoelem returnValue;


    while (!(terminado || error)){
        caracter = siguienteCaracter();
        switch (estado){
            case 0:
                if (caracter == '#'){
                    estado = 1; // Automata de comentarios de una linea
                }
                else if(caracter == '"'){
                     if (_identificarSiComentarioMultilinea(&caracter)){
                        estado = 2; // Automata de comentarios multilinea
                    }
                }
                else if (isalpha(caracter) || caracter == '_'){
                    estado = 3; // Automata de cadenas alfanumericas
                }
                else if (isdigit(caracter)){
                    estado = 4; // Automata de numeros
                }
                else if (caracter == '"' || caracter == '`'){
                    estado = 5; // Automata de strings
                }
                else if(caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '=' || caracter == '<' || caracter == '>'){
                    estado = 6; // Automata de operadores
                }
                else if (caracter == '(' || caracter == ')' || caracter == '[' || caracter == ']' || caracter == '{' || caracter == '}'){
                    estado = 7; // Automata de delimitadores
                }
                else if (caracter == EOF) { //si encontramos un EOF (esto funciona solo para el sistema de entrada simple)
                    returnValue.valor = EOF;
                    returnValue.lexema = "EOF";
                    terminado = 1;
                }
                else if(isblank(caracter) || caracter == '\n'){
                    terminado = 1;
                    recuperarLexema();
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
                _identificarID(&caracter);
                // Hemos identificado una cadena, terminamos
                terminado = 1;
            break;
        }
    }
    if (!strncmp(lexema, ".", 1)){
        returnValue.valor = '.';
        returnValue.lexema = (char*) malloc(1 * sizeof(char));
        returnValue.lexema = ".";
        return returnValue;
    }
    // Buscamos el entero correspondiente al lexema en la TS
    returnValue = buscar_lexema(&TS, lexema);
                

    return returnValue;
}


void cadenaAlfanumerica(char caracter);

/*
tipoelem siguienteComponenteLexico(abin TS){
    int terminado = 0;
    int error = 0;
    tipoelem returnValue;
    int estado = 0;
    
    while (!(terminado || error)){
        char caracter = siguienteCaracter();
        switch (estado){
            case 0:
             

        }
        

    }

    return returnValue;
}
*/


void _saltarComentario(char *caracter){
    printf("Se encontro un comentario de una linea\n");
    while (*caracter != '\n'){
        *caracter = siguienteCaracter();
    }
}

// ADAPTAR AL SISTEMA DE ENTRADA
// -- TODO --
//
// -- TODO --
//
/*
int  _identificarSiComentarioMultilinea(char *caracter){
    int posicionArchivo;   
    // Guardamos la posicion del archivo
    // TODO
    posicionArchivo = ftell(archivo);
    if (fgetc(archivo) == '"'){ // Posible comentario multilinea
        if (fgetc(archivo) == '"'){ // ES un comentario multilinea
            return 1;
        }
    }
    // Volvemos a la posicion original del archivo si no es un comentario multilinea
    fseek(archivo, posicionArchivo, SEEK_SET);
    // ungetc() // TODO
    return 0;
}
*/

void _saltarComentarioMultilinea(char *caracter){
    while (1){
        *caracter = siguienteCaracter();
        if (*caracter == '"'){
            if (siguienteCaracter() == '"'){
                if (siguienteCaracter() == '"'){
                    if (siguienteCaracter() == '\n'){ // Saltamos el \n final
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

/*
void _identificarID(char *caracter){
    printf("Se encontro un ID\n");
    // Leemos la cadena y la guardamos en el buffer
    indiceBuffer = 0;
    while (!_esDelimitador(*caracter) && *caracter != EOF){
        buffer[indiceBuffer] = *caracter;
        indiceBuffer++;
        *caracter = siguienteCaracter();
    }
    // Añadimos el caracter de fin de cadena
    buffer[indiceBuffer] = '\0';
    // Copiamos el buffer en el lexema
    strncpy(lexema, buffer, indiceBuffer);
    // Vaciamos el buffer
    _vaciarBuffer(buffer);
    printf("Buffer: %s\n", buffer);
}

*/

int _esDelimitador(char cadena){
    for (int i = 0; i < TAM_DELIMITERS; i++){
        if (cadena == delimitadores[i]){
            return 1;
        }
    }
    return 0;
}

