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

int main(){
    FILE *archivo;
    tColectivo reg;
    char opcion;

    archivo = fopen("Colectivos.dat", "ab");
    if(archivo == NULL){
        printf("No se puede abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    do {
        printf("\n--- INGRESO DE DATOS DE COLECTIVO ---\n");

        printf("Nombre del chofer: ");
        fflush(stdin);
        fgets(reg.nombreChofer, MAX, stdin);
        reg.nombreChofer[strcspn(reg.nombreChofer, "\n")] = 0;

        printf("Numero de colectivo: ");
        scanf("%d", &reg.nroColectivo);

        printf("Genero del chofer (m/f/x): ");
        fflush(stdin);
        scanf("%c", &reg.genChofer);

        printf("Cantidad de recorridos diarios: ");
        scanf("%d", &reg.horario);

        fwrite(&reg, sizeof(tColectivo), 1, archivo);

        printf("Desea ingresar otro registro? (s/n): ");
        fflush(stdin);
        scanf("%c", &opcion);

    } while(opcion == 's' || opcion == 'S');

    fclose(archivo);
    printf("\nDatos guardados correctamente en Colectivos.dat\n");

    return 0;
}

