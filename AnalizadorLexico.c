#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "TS.h"
#include "AnalizadorLexico.h"
#include <ctype.h>

/*
void siguienteComponenteLexico(FILE *archivo, char* componenteLexico){
    int indiceComponenteLexico = 0;
    char caracter;
    int delimitadorEncontrado = 0;
    caracter = fgetc(archivo);
    int delimitadores[11] = {
        ' ', '\n', '(', ')', '[', ']', '{', '}', ',', ':', '.'
    };
    while (caracter != EOF){
        for (int i = 0; i < 11; i++){
            // Si el caracter es un comentario, ignoramos el resto de la linea
            if (caracter == '#'){
                while (fgetc(archivo) != '\n'){
                    caracter = fgetc(archivo);
                }
                break;
            }
            // Hacemos lo mismo para los comentarios multilinea
            // TODO
            if (caracter == delimitadores[i]){
                delimitadorEncontrado = 1;
                break;
            }
        }
        if (delimitadorEncontrado){
            break;
        }
        else{
            // Agregamos el caracter al componente lexico
            componenteLexico[indiceComponenteLexico] = caracter;
            indiceComponenteLexico++;
            caracter = fgetc(archivo);
        }
    }
    // Agregamos el caracter nulo al final de la cadena
    componenteLexico[indiceComponenteLexico] = '\0';
}
*/

void _saltarComentario(FILE *archivo, char *caracter);
int  identificarSiComentarioMultilinea(FILE *archivo, char *caracter);
void _saltarComentarioMultilinea(FILE *archivo, char *caracter);

int siguienteComponenteLexico(FILE *archivo, abin TS){
    int error = 0;
    int estado = 0;
    int terminado = 0;
    char caracter;
    int componenteLexico = 0; 

    while (!(terminado || error)){
        caracter = fgetc(archivo);
        switch (estado){
            case 0:
                if (caracter == '#'){
                    estado = 1; // Identificar comentarios
                }
                else if(caracter == '"'){ // Posible comentario multilinea
                    if (_identificarSiComentarioMultilinea(archivo, &caracter)){
                        estado = 2; // Saltar comentario multilinea
                    }
                    else{ // Leemos la cadena
                        estado = 3;
                    }
                }
                break;
            case 1: // Identificar comentarios
                _saltarComentario(archivo, &caracter);
                return 1;
                break;
            case 2: // Identificar comentarios multilinea
                _saltarComentarioMultilinea(archivo, &caracter);
                return 1;
                break;
            case 3: // Identificar cadenas
                _identificarCadena(archivo, &caracter);
            break;
        }
    }
    return componenteLexico;
}


void _saltarComentario(FILE *archivo, char *caracter){
    printf("Se encontro un comentario de una linea\n");
    while (*caracter != '\n'){
        *caracter = fgetc(archivo);
    }
}

int  identificarSiComentarioMultilinea(FILE *archivo, char *caracter){
    int posicionArchivo;   
    // Guardamos la posicion del archivo
    posicionArchivo = ftell(archivo);
    if (fgetc(archivo) == '"'){ // Posible comentario multilinea
        if (fgetc(archivo) == '"'){ // ES un comentario multilinea
            return 1;
        }
        else{ // Si no es comentario multilinea, retrocedemos el puntero
            fseek(archivo, posicionArchivo, SEEK_SET);
            return 0;
        }
    }
}
void _saltarComentarioMultilinea(FILE *archivo, char *caracter){
    printf("Se encontro un comentario multilinea\n");
    while (1){
        *caracter = fgetc(archivo);
        if (*caracter == '"'){
            if (fgetc(archivo) == '"'){
                if (fgetc(archivo) == '"'){
                    break;
                }
            }
        }
        if (*caracter == EOF){
            printf("Error: comentario multilinea no cerrado\n");
            break;
        }
    }
}