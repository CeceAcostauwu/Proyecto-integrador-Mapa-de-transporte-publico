#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void generarDatosAleatorios();

int main() {
    generarDatosAleatorios();
    return 0;
}

void generarDatosAleatorios() {
    FILE *archivo = NULL;
    tRegistroColectivos reg;
    int num_cole, i;
    int cant_choferes;

    char *nombres[] = {"Juan Perez", "Maria Gomez", "Carlos Lopez", "Ana Garcia", "Pedro Diaz", "Laura Ruiz", "Miguel Torres", "Sofia Blanco"};
    char generos[] = {'H', 'M', 'X'};
    char *horarios[] = {"06:00", "09:00", "12:00", "15:00", "18:00", "21:00"};
    int total_nombres = 8;
    int total_generos = 3;
    int total_horarios = 6;

    srand(time(NULL));

    archivo = fopen(NOMBRE_ARCHIVO, "wb");
    if (archivo == NULL) {
        printf("ERROR: No se pudo abrir el archivo %s para escritura.\n", NOMBRE_ARCHIVO);
        return;
    }

    printf("--- Generando Archivo Binario ORDENADO: %s ---\n", NOMBRE_ARCHIVO);

    // Iteracion ORDENADA por num_colectivo
    for (num_cole = 101; num_cole <= 108; num_cole++) {
        cant_choferes = 4 + (rand() % 2); // 4 o 5 choferes

        for (i = 0; i < cant_choferes; i++) {
            reg.nroColectivo = num_cole;

            // Datos aleatorios
            strncpy(reg.nombreChofer, nombres[rand() % total_nombres], MAX_STRING - 1);
            reg.nombreChofer[MAX_STRING - 1] = '\0';

            reg.genero = generos[rand() % total_generos];

            strncpy(reg.horario_salida, horarios[rand() % total_horarios], LARGO_HORARIO - 1);
            reg.horario_salida[LARGO_HORARIO - 1] = '\0';

            reg.cant_recorridos_dia = 2 + (rand() % 5);

            // Escritura binaria
            if (fwrite(&reg, sizeof(tRegistroColectivos), 1, archivo) != 1) {
                printf("ERROR al escribir el registro en el archivo.\n");
                fclose(archivo);
                return;
            }
        }
    }

    fclose(archivo);
    printf("Archivo binario '%s' creado y ordenado exitosamente.\n", NOMBRE_ARCHIVO);
}
