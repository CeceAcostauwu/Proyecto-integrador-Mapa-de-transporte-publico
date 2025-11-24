#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
typedef char tString[MAX];

typedef struct {
    tString nombreChofer;
    int nroColectivo;
    char genChofer;
    int recorridosDiarios;
} tRegistroColectivo;

FILE * archivoColectivo;
tRegistroColectivo registro, registroAnterior;

int cantMas, cantFem, cantNB;
int totalRecorridos;

int totalChoferes = 0, totalRecorridosGenerales = 0;
int totalMas = 0, totalFem = 0, totalNB = 0;

int maxRecorridos = 0;
int colectivoMax = 0;

void inicializar();
void procesarCorte();
void finCorte(int colectivoActual);
void finalizar();

int main() {
    inicializar();
    procesarCorte();
    finalizar();
    
    return 0;
}

void inicializar() {
    archivoColectivo = fopen("Colectivos.dat", "rb");
    if(archivoColectivo != NULL){
        fread(&registro, sizeof(tRegistroColectivo), 1, archivoColectivo);
    } else{
    	printf("No se encuentra el archivo\n");
        exit(EXIT_FAILURE);
    }
    registroAnterior = registro;

    cantMas = cantFem = cantNB = 0;
    totalRecorridos = 0;

    totalChoferes = totalRecorridosGenerales = 0;
    totalMas = totalFem = totalNB = 0;
    maxRecorridos = 0;
    colectivoMax = 0;
}

void procesarCorte() {
	
	printf("\n\t*** Listado de Choferes por Colectivo ***\n");

    while(!feof(archivoColectivo)) {

        cantMas = cantFem = cantNB = 0;
        totalRecorridos = 0;
        
        int colectivoActual = registro.nroColectivo;
        
        while(!feof(archivoColectivo) && registro.nroColectivo == colectivoActual) {
            printf("Colectivo %d | Chofer: %s | Genero: %c | Recorridos diarios: %d\n", registro.nroColectivo, registro.nombreChofer, registro.genChofer, registro.recorridosDiarios);

            switch(registro.genChofer){
                case 'm': case 'M': cantMas++; totalMas++; break;
                case 'f': case 'F': cantFem++; totalFem++; break;
                case 'x': case 'X': cantNB++; totalNB++; break;
            }
            totalRecorridos += registro.recorridosDiarios;

            totalRecorridosGenerales += registro.recorridosDiarios;
            totalChoferes++;

		    fread(&registro, sizeof(tRegistroColectivo), 1, archivoColectivo);
        }

        finCorte(colectivoActual);
    }


}

void finCorte(int colectivoActual) {
    printf("\n------ Totales Colectivo %d ------\n", colectivoActual);
    printf("Choferes masculinos: %d | femeninos: %d | no binario: %d\n", cantMas, cantFem, cantNB);
    printf("Total de recorridos diarios: %d\n\n", totalRecorridos);

    if(totalRecorridos > maxRecorridos) {
        maxRecorridos = totalRecorridos;
        colectivoMax = colectivoActual;
    }
    
    registroAnterior = registro;
}

void finalizar() {
	printf("\n---------------- Totales Generales ----------------\n");
    printf("Total choferes: %d\n", totalChoferes);
    printf("Total recorridos diarios: %d\n", totalRecorridosGenerales);
    printf("Choferes masculinos: %d (%.2f%%) | femeninos: %d (%.2f%%) | no binario: %d (%.2f%%)\n",
           totalMas, totalChoferes ? 100.0*totalMas/totalChoferes : 0,
           totalFem, totalChoferes ? 100.0*totalFem/totalChoferes : 0,
           totalNB, totalChoferes ? 100.0*totalNB/totalChoferes : 0);
    printf("Promedio de recorridos por chofer: %.2f\n",
           totalChoferes ? (float)totalRecorridosGenerales/totalChoferes : 0);
    printf("Colectivo con más recorridos diarios: %d (%d recorridos)\n", colectivoMax, maxRecorridos);
    
    
    fclose(archivoColectivo);
    printf("Archivo cerrado.\n");
}

