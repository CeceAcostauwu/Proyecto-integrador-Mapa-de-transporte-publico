#ifndef CORTE_CONTROL_H
#define CORTE_CONTROL_H

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

FILE *archivoColectivo;
tRegistroColectivos registro, registroAnterior;

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

void corteDeControl(){
    inicializar();
    procesarCorte();
    finalizar();
}

void inicializar() {
    archivoColectivo = fopen(NOMBRE_ARCHIVO, "rb");
    if(archivoColectivo != NULL){
        fread(&registro, sizeof(tRegistroColectivos), 1, archivoColectivo);
    } else{
        printf("No se encuentra el archivo %s\n", NOMBRE_ARCHIVO);
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

            printf("Colectivo %d | Chofer: %s | Genero: %c | Salida: %s | Recorridos diarios: %d\n", registro.nroColectivo, registro.nombreChofer, registro.genero, registro.horario_salida, registro.cant_recorridos_dia);

            switch(registro.genero) {
   				case 'H': case 'h': cantMas++; totalMas++; break;
    			case 'M': case 'm': cantFem++; totalFem++; break;
    			case 'X': case 'x': cantNB++; totalNB++; break;
				}

            totalRecorridos += registro.cant_recorridos_dia;
            totalRecorridosGenerales += registro.cant_recorridos_dia;
            totalChoferes++;

            registroAnterior = registro;
            fread(&registro, sizeof(tRegistroColectivos), 1, archivoColectivo);
        }

        finCorte(colectivoActual);
    }
}

void finCorte(int colectivoActual) {
    printf("\n------ Totales Colectivo %d ------\n", colectivoActual);
    printf("Choferes Hombres: %d | Mujeres: %d | no Binario: %d\n", cantMas, cantFem, cantNB);
    printf("Total de recorridos diarios: %d\n\n", totalRecorridos);

    if(totalRecorridos > maxRecorridos) {
        maxRecorridos = totalRecorridos;
        colectivoMax = colectivoActual;
    }
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
    printf("Colectivo con mas recorridos diarios: %d (%d recorridos)\n", colectivoMax, maxRecorridos);

    fclose(archivoColectivo);
    printf("Archivo cerrado.\n");
}

#endif
