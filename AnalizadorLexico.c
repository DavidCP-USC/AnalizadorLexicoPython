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

char lexemasIndividuales[TAM_LEXEMAS_UNICARACTER] = "()[]{}.,:;!&|";
tipoelem returnValue;

void _saltarComentario(char *caracter);
int  _identificarSiComentarioMultilinea() ;
void _saltarComentarioMultilinea(char *caracter);
int _comprobarSiPuntoEsParteDeNumero();
int _esLexemaUnicaracter(char cadena);
void _recuperarLexema(int tipo, int retroceder);
void _identificarCadenasAlfanumericas(char *caracter);
void _identificarNumeros(char *caracter);
void _identificarNumerosHexadecimales(char *caracter);
void _identificarStrings(char *caracter, char tipoDeComillas);
void _identificarOperadores(char *caracter);



tipoelem siguienteComponenteLexico(){
    short error = 0;
    short terminado = 0;
    short estado = 0;
    char caracter;
    char tipoDeComillas;


    while (!(terminado || error)){
        //printf("\tEstado: %d\n", estado);
        switch (estado){
            case 0:
                caracter = siguienteCaracter();
                if (caracter == '#'){
                    estado = 1; // Automata de comentarios de una linea
                }
                else if(caracter == '"'){
                    if (_identificarSiComentarioMultilinea()){
                        estado = 2; // Automata de comentarios multilinea
                    }
                    else{
                        estado = 5; // Automata de strings
                        tipoDeComillas = '"';
                    }
                }
                else if(caracter == '.'){
                    if (_comprobarSiPuntoEsParteDeNumero()){
                        estado = 4; // Automata de numeros
                    }
                    else{
                        estado = 7; // Automata de lexemas de un caracter que no sean operadores ni letras
                    }
                }
                else if (isalpha(caracter) || caracter == '_'){
                    estado = 3; // Automata de cadenas alfanumericas
                }
                else if (isdigit(caracter)){
                    estado = 4; // Automata de numeros
                }
                else if (caracter == '\''){ // Las " ya se han comprobado antes
                    tipoDeComillas = '\'';
                    estado = 5; // Automata de strings
                }
                else if(caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '<' || caracter == '>' || caracter == '='){
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
                _identificarNumeros(&caracter);
                terminado = 1;
                break;
            case 5: // Identificar strings
                _identificarStrings(&caracter, tipoDeComillas);
                terminado = 1;
                break;
            case 6: // Identificar operadores
                _identificarOperadores(&caracter);
                terminado = 1;
                break;
            case 7: // Identificar delimitadores
                _recuperarLexema(caracter, 0);
                terminado = 1;
                break;
            default:
                printf("Error: Estado no reconocido\n");
                error = 1;
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


int  _identificarSiComentarioMultilinea(){    
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


int _comprobarSiPuntoEsParteDeNumero(){
    if (isdigit(siguienteCaracter())){
        return 1;
    }
    else{
        retrocederCaracter();
        return 0;
    }
}


void _identificarCadenasAlfanumericas(char *caracter){
    do{
        *caracter = siguienteCaracter();
    }while(isalpha(*caracter) || isdigit(*caracter) || *caracter == '_');
    if (_esLexemaUnicaracter(*caracter) || isblank(*caracter) || *caracter == '\n' || *caracter == EOF){
        _recuperarLexema(ID, 1);
    }
    else{
        _recuperarLexema(ID, 0);
    }
}


void _identificarNumeros(char *caracter){ 
    char caracterSiguiente = *caracter;
    // Comprobamos si es un numero en notacion hexadecimal
    if (*caracter == '0'){
        caracterSiguiente = siguienteCaracter();
        if (caracterSiguiente == 'x' || caracterSiguiente == 'X'){
            _identificarNumerosHexadecimales(caracter);
            return;
        }
        else{
            retrocederCaracter();
        }
    }

    // Leemos hasta encontrar un caracter que no sea un digito
    if (isdigit(*caracter)){
        do{
            *caracter = siguienteCaracter();
        }while (isdigit(*caracter));
    }

    // Comprobamos si es un numero en notacion decimal
    if (*caracter == '.'){
        *caracter = siguienteCaracter();
        if (isdigit(*caracter)){
            do{
                *caracter = siguienteCaracter();
            }while (isdigit(*caracter));
        }
    }

    // Comprobamos si es un numero en notacion cientifica
    // (exponencial)
    if(*caracter == 'e' || *caracter == 'E'){
        *caracter = siguienteCaracter();
        if (*caracter == '+' || *caracter == '-'){
            *caracter = siguienteCaracter();
        }
        if (isdigit(*caracter)){
            do{
                *caracter = siguienteCaracter();
            }while (isdigit(*caracter));
        }
        else{
            printf("Error: Numero mal formado\n");
            exit(1);
            // ERROR TODO
        }
    }

    // Hemos terminado de leer el numero
    // aceptamos el lexema
    if (_esLexemaUnicaracter(*caracter) || isblank(*caracter) || *caracter == '\n' || *caracter == EOF || *caracter == '+' || *caracter == '-' || *caracter == '*' || *caracter == '/' || *caracter == '<' || *caracter == '>' || *caracter == '='){
        _recuperarLexema(NUM, 1);
    }
    else{
        printf("Error: Numero mal formado\n");
        exit(1);
        // ERROR TODO
    }
}


void _identificarNumerosHexadecimales(char *caracter){
    do{
        *caracter = siguienteCaracter();
    }while (isxdigit(*caracter));
    if (_esLexemaUnicaracter(*caracter) || isblank(*caracter) || *caracter == '\n' || *caracter == EOF){
        _recuperarLexema(NUM_HEX, 1);
    }
    else{
        printf("Error: Numero mal formado\n");
        exit(1);
        // ERROR TODO
    }

}


void _identificarStrings(char *caracter, char tipoDeComillas){
    do{
        *caracter = siguienteCaracter();
        if (*caracter == EOF){
            printf("Error: String no cerrado\n");
            exit(1);
        }
    }while (*caracter != tipoDeComillas);

    _recuperarLexema(STRING, 0);
}


void _identificarOperadores(char *caracter){
    char caracterAnterior = *caracter;
    // Comrobamos si es '=' o '=='
    if (*caracter == '='){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(COMPARACION_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '<' o '<='
    else if (*caracter == '<'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MENOR_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '>' o '>='
    else if (*caracter == '>'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MAYOR_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '!' o '!='
    else if (*caracter == '!'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(DISTINTO, 0);
            return;
        }
        else{
            printf("Error: Operador no reconocido\n");
            exit(1);
            // ERROR
            // TODO
        }
    }
    // Comprobamos si es '+' o '+='
    else if (*caracter == '+'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MAS_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '-' o '-='
    else if (*caracter == '-'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MENOS_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Compribamos si es '*' o '*=' o '**'
    else if (*caracter == '*'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(POR_IGUAL, 0);
            return;
        }
        else if(*caracter == '*'){
            _recuperarLexema(EXPONENTE, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '/' o '/='
    else if (*caracter == '/'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(DIVIDIDO_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '%' o '%='
    else if (*caracter == '%'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MODULO_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }

    else if (isblank(*caracter) || *caracter == '\n' || *caracter == EOF){
        _recuperarLexema(caracterAnterior, 1);
    }
    else{
        // GESTIONAR ERROR
        // -- TODO --
        printf("Error: Operador no reconocido\n");
        exit(1);
    }
}


void _recuperarLexema(int tipo, int retroceder){
    //printf("\tTipo: %d\n", tipo);
    if (retroceder){
        retrocederCaracter();
    }
    if (tipo == ID){
        obtenerLexema(&returnValue);
        returnValue.valor = obtenerValorTS(returnValue.lexema);
    }
    else if (tipo == NUM){
        obtenerLexema(&returnValue);
        returnValue.valor = NUM;
    }
    else if (tipo == NUM_HEX){
        obtenerLexema(&returnValue);
        returnValue.valor = NUM_HEX;
    }
    else if (tipo == MAS_IGUAL){
        printf("MAS_IGUAL\n");
        returnValue.valor = MAS_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '+';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == MENOS_IGUAL){
        returnValue.valor = MENOS_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '-';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == POR_IGUAL){
        returnValue.valor = POR_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '*';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == DIVIDIDO_IGUAL){
        returnValue.valor = DIVIDIDO_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '/';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == MODULO_IGUAL){
        returnValue.valor = MODULO_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '%';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == EXPONENTE){
        returnValue.valor = EXPONENTE;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '*';
        returnValue.lexema[1] = '*';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == MENOR_IGUAL){
        returnValue.valor = MENOR_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '<';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
     else if (tipo == MAYOR_IGUAL){
        returnValue.valor = MAYOR_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '>';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == DISTINTO){
        returnValue.valor = DISTINTO;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '!';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == COMPARACION_IGUAL){
        returnValue.valor = COMPARACION_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '=';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == STRING){
        returnValue.valor = STRING;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(7 * sizeof(char));
        strcpy(returnValue.lexema, "STRING"); // Copiamos los 6 primeros caracteres
        returnValue.lexema[6] = '\0';
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
