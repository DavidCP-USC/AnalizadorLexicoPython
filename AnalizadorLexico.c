#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "TS.h"
#include "AnalizadorLexico.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>

char lexema[TAM_MAX];
char buffer[TAM_MAX];
int indiceBuffer = 0;
char delimitadores[TAM_DELIMITERS] = "()[]{},:. \n\0";
// EOF

void _vaciarBuffer(char *buffer);
void _saltarComentario(FILE *archivo, char *caracter);
int  _identificarSiComentarioMultilinea(FILE *archivo, char *caracter);
void _saltarComentarioMultilinea(FILE *archivo, char *caracter);
void _identificarID(FILE *archivo, char *caracter);
int _esDelimitador(char cadena);


int siguienteComponenteLexico(FILE *archivo, abin TS){
    int error = 0;
    int estado = 0;
    int terminado = 0;
    char caracter;
    int componenteLexico = 0; 


    while (!(terminado || error)){
        caracter = fgetc(archivo);
        //printf ("Caracter: %c - Estado: %d\n", caracter, estado);

        switch (estado){
            case 0:
                if (caracter == '#'){
                    estado = 1; // Identificar comentarios
                }
                else if(caracter == '"'){ // Posible comentario multilinea
                    if (_identificarSiComentarioMultilinea(archivo, &caracter)){
                        printf("Se encontro un comentario multilinea\n");
                        estado = 2; // Saltar comentario multilinea
                    }
                    else{ // Leemos la cadena "..."
                        // TODO
                    }
                }
                else{
                    estado = 3; // Identificar ID
                }
                break;
            case 1: // Identificar comentarios
                _saltarComentario(archivo, &caracter);
                estado = 0;
                break;
            case 2: // Identificar comentarios multilinea
                _saltarComentarioMultilinea(archivo, &caracter);
                estado = 0;
                break;
            case 3: // Identificar cadenas
                _identificarID(archivo, &caracter);
                // Hemos identificado una cadena, terminamos
                terminado = 1;
            break;
        }
    }

    // Buscamos el entero correspondiente al lexema en la TS
    componenteLexico = buscar_lexema(TS, lexema);

    return componenteLexico;
}

void _vaciarBuffer(char *buffer){
    for (int i = 0; i < TAM_MAX; i++){
        buffer[i] = '\0';
    }
    indiceBuffer = 0;
}

void _saltarComentario(FILE *archivo, char *caracter){
    printf("Se encontro un comentario de una linea\n");
    while (*caracter != '\n'){
        *caracter = fgetc(archivo);
    }
}

int  _identificarSiComentarioMultilinea(FILE *archivo, char *caracter){
    int posicionArchivo;   
    // Guardamos la posicion del archivo
    posicionArchivo = ftell(archivo);
    if (fgetc(archivo) == '"'){ // Posible comentario multilinea
        if (fgetc(archivo) == '"'){ // ES un comentario multilinea
            return 1;
        }
    }
    // Volvemos a la posicion original del archivo si no es un comentario multilinea
    fseek(archivo, posicionArchivo, SEEK_SET);
    return 0;
}

void _saltarComentarioMultilinea(FILE *archivo, char *caracter){
    while (1){
        *caracter = fgetc(archivo);
        if (*caracter == '"'){
            if (fgetc(archivo) == '"'){
                if (fgetc(archivo) == '"'){
                    if (fgetc(archivo) == '\n'){ // Saltamos el \n final
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

void _identificarID(FILE *archivo, char *caracter){
    printf("Se encontro un ID\n");
    // Leemos la cadena y la guardamos en el buffer
    indiceBuffer = 0;
    while (!_esDelimitador(*caracter) && *caracter != EOF){
        buffer[indiceBuffer] = *caracter;
        indiceBuffer++;
        *caracter = fgetc(archivo);
    }
    printf("Fin bucle\n");
    // Añadimos el caracter de fin de cadena
    buffer[indiceBuffer] = '\0';
    // Copiamos el buffer en el lexema
    strncpy(lexema, buffer, indiceBuffer);
    printf("Buffer: %s\n", buffer);
    // Vaciamos el buffer
    _vaciarBuffer(buffer);
}

int _esDelimitador(char cadena){
    for (int i = 0; i < TAM_DELIMITERS; i++){
        if (cadena == delimitadores[i]){
            return 1;
        }
    }
    return 0;
}
