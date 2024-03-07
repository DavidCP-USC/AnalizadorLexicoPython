#include "SistemaEntrada.h"
#include "abin.h"
#include <string.h>



// Aplicamos el Patron Singleton
FILE* archivo = NULL;
dobleBuffer db;

// Funcion privada para hacer pruevas e imprimir el doble buffer
void _imprimirDobleBuffer();

/*
Inicializa el sistema de entrada
Parametros:
    char* nombreArchivo: Nombre del archivo que se va a leer
*/
void inicializarSistemaEntrada(char* nombreArchivo){
    // Se hace una primera comprobación para ver si el archivo
    // se abrio anteriormente
    if (archivo == NULL){
        archivo = fopen(nombreArchivo, "r");
        if(archivo != NULL){
            // Leemos el primer bloque
            fread(db.buffer, 1, TAM_MAX_LEXEMA, archivo); 
            // Ponemos los caracteres EOF en sus respectivos lugares
            db.buffer[CENTINELA1]=EOF;
            db.buffer[CENTINELA2]=EOF;
        }
        else{
            // PASAR A GESTION DE ERRORES
            // TODO
            exit(1);
        }
    }    
}

char siguienteCaracter(){
    // Comprobamos si tenemos un EOF
    if (db.buffer[db.delantero] == EOF){
        // El caso de tener un EOF comprobamos si es el de algun centinela
        // o el del propio archivo
        unsigned int bytesLeidos = 0;
        // Si es el EOF del centinela 1 actualizamos el segundo bloque
        if (db.delantero == CENTINELA1){
            bytesLeidos = fread(&db.buffer[INICIO_BLOQUE2], 1, TAM_MAX_LEXEMA, archivo);
            // Comprobamos si hemos llegado al final del archivo
            if (feof(archivo)){
                db.buffer[bytesLeidos + INICIO_BLOQUE2] = EOF; // fread no guarda el EOF, lo anadimos manualmente
            }
            db.delantero++;
        }

        // Si es el EOF del centinela 2 actualizamos el primer bloque
        else if (db.delantero == CENTINELA2){
            bytesLeidos = fread(db.buffer, 1, TAM_MAX_LEXEMA, archivo);
            if (feof(archivo)){
                db.buffer[bytesLeidos] = EOF; // fread no guarda el EOF, lo anadimos manualmente
            }
            db.delantero = 0; // Volvemos al principio del buffer
        }

        else{ // Fin de la lectura del fichero
            db.delantero++;
            return EOF;
        }
    }

    // Devolvemos el caracter actual y avanzamos el puntero
    char siguiente = db.buffer[db.delantero];
    db.delantero++;

    return siguiente;
}


void retrocederCaracter(){
    db.delantero--;
    // Si el delantero es menor que 0, es que estamos en el primer
    // bloque y aca
    if(db.delantero < 0){
        db.delantero = CENTINELA2-1;
    }
    else if (db.delantero == CENTINELA1){
        db.delantero--;
    }
}

void aceptarLexema(){
    db.inicio = db.delantero;
}





/*
Funcion que obtiene el lexema que se encuentra en el buffer
Parametros:
    tipoelem *returnValue: Puntero al tipo de dato que se va a devolver

*/
void obtenerLexema(tipoelem *returnValue){
    // Reservamos memoria para el lexema
    if (returnValue->lexema != NULL){
        free(returnValue->lexema);
    }
    // Pedimos memoria para el lexema (el +1 es para el caracter de fin de cadena '\0') 
    returnValue->lexema = (char*) malloc(db.delantero - db.inicio+1);
    // Copiamos el lexema en el puntero y añadimos el caracter de fin de cadena
    printf("Obteniendo lexema\n");
    strncpy(returnValue->lexema, &db.buffer[db.inicio], db.delantero - db.inicio);
    returnValue->lexema[db.delantero - db.inicio] = '\0';
}

void finalizarSistemaEntrada(){
    fclose(archivo);
}





// Funcion privada para hacer pruevas e imprimir el doble buffer
void _imprimirDobleBuffer(){
    printf("Buffer: ");
    for (int i = 0; i < TAMANO_DOBLE_BUFFER; i++){
        if (i == CENTINELA1 || i == CENTINELA2){
            printf("|");
        }
        else{
            printf("%c", db.buffer[i]);
        }
    }
    printf("\n");
    printf("Inicio: %d\n", db.inicio);
    printf("Delantero: %d\n", db.delantero);
}

