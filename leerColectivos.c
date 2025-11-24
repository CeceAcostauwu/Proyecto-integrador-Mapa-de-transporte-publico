#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 50
#define LARGO_HORARIO 6
#define NOMBRE_ARCHIVO "Colectivos.dat"

typedef char tString[MAX_STRING];

typedef struct {
    int nroColectivo;
    tString nombreChofer;
    char genero;
    char horario_salida[LARGO_HORARIO];
    int cant_recorridos_dia;      
} tRegistroColectivos;

int main() {
    FILE *archivo;
    tRegistroColectivos reg;

    archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        printf("No se puede abrir el archivo %s\n", NOMBRE_ARCHIVO);
        return 1;
    }

    printf("--- Lista de Choferes ---\n");
    printf("Colectivo | Nombre del Chofer               | Género | Salida | Recorridos diarios\n");
    printf("--------------------------------------------------------------------------------------\n");

    while (fread(&reg, sizeof(tRegistroColectivos), 1, archivo) == 1) {
        printf("%9d | %-30s |   %c    | %-5s | %d\n",
               reg.nroColectivo,
               reg.nombreChofer,
               reg.genero,
               reg.horario_salida,
               reg.cant_recorridos_dia);
    }

    fclose(archivo);
    printf("\nLectura finalizada.\n");

    return 0;
}

