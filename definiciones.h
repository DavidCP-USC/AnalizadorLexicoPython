// Constantes y definiciones para el analizador léxico

// -- Constantes --
#define TAM_MAX 255
#define TAM_MAX_LEXEMA 64
#define TAM_LEXEMAS_UNICARACTER 14

// -- Tokens -- 
#define DEF 300
#define IMPORT 301
#define AS 302
#define FOR 303
#define IN 304
#define IF 305
#define ELIF 306
#define ELSE 307
#define RETURN 308
#define NOT 309
#define ID 310
#define NUM 311

// -- Tokens de operadores de mas de un caracter --
#define MAYORIGUAL 411 // >=
#define MENORIGUAL 412 // <=
#define DISTINTO 414 // !=

// -- Tokens de gestion de errores --
#define ERROR_SISTEMA_DE_ENTRADA 500
#define ERROR_ANALIZADOR_LEXICO 501
#define ERROR_ANALIZADOR_SINTACTICO 502




