# Requisitos

* GNU GCC
* GNU Make

# Versiones probadas
* GNU GCC 13.2.1 20230801
* GNU Make 4.4.1


# Compilación

Para compilar el proyecto, ejecute el siguiente comando en la terminal:

```
make
```
Se generará un archivo ejecutable con nombre ```ejecutable``` que, de no tener permisos de ejecución, se le deben otorgar con el comando
```chmod +x ./ejecutable```

Finalmente, una vez el archivo tenga permisos de ejecución se le debe de pasar como argument el nombre del archivo que se quiere analizar.

# Ejecución

Una vez ya tenemos compilado el proyecto simplemente lo ejecutamos mediante el siguiente comando:
```
./ejecutable Archivo_A_Analizar.py
```