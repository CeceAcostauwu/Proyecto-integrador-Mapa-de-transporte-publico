#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
typedef char tString[MAX];

typedef struct{
    tString nombreChofer;
    int nroColectivo;
    char genChofer;
    int horario;
} tColectivo;

int main() {
    FILE *archivo;
    tColectivo reg;

    archivo = fopen("Colectivos.dat", "rb");
    if(archivo == NULL){
        printf("No se puede abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n--- Lista de colectivos ---\n");
    printf("NroColectivo | NombreChofer               | Genero | RecorridosDiarios\n");
    printf("---------------------------------------------------------------------\n");

    while(fread(&reg, sizeof(tColectivo), 1, archivo) == 1) {
        printf("%11d | %-25s | %-6c | %d\n", 
               reg.nroColectivo, reg.nombreChofer, reg.genChofer, reg.horario);
    }

    fclose(archivo);
    printf("\nLectura finalizada.\n");

    return 0;
}

