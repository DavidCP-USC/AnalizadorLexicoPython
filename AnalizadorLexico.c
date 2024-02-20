#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "TS.h"
#include "AnalizadorLexico.h"
#define TAM_MAX 255

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
