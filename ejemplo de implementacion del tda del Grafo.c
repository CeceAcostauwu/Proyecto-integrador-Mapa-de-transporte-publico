#include "Grafo.h"

int main() {

    tGrafoPonderado grafo;

    /* Arrays auxiliares para Dijkstra */
    int dist[MAX_BARRIOS+1];
    int prev[MAX_BARRIOS+1];

    int origen, destino;

    /* ============================================
       1) Inicializar el grafo y cargar todo
       ============================================ */
    inicializarGrafo(&grafo);
    cargarMapaTransporte(&grafo);

    printf("\n============================================\n");
    printf("   MAPA DE TRANSPORTE - RUTAS OPTIMAS\n");
    printf("============================================\n");

    /* ============================================
       2) Ingreso de datos por el usuario
       ============================================ */
    printf("\n Lista de Barrios:\n 1-Margen\n 2-Luna de Ketchup\n 3-Asfalto\n 4-Separacion\n 5-Laguna Mojada\n 6-Brahma\n 7-El Cholo\n 8-San Catalino\n");

    printf("\nIngrese barrio de ORIGEN (1 a %d): ", MAX_BARRIOS);
    scanf("%d", &origen);

    printf("Ingrese barrio de DESTINO (1 a %d): ", MAX_BARRIOS);
    scanf("%d", &destino);

    if(origen < 1 || origen > MAX_BARRIOS || destino < 1 || destino > MAX_BARRIOS) {
        printf("\nERROR: Los valores deben estar entre 1 y %d\n", MAX_BARRIOS);
        return 0;
    }

    /* ============================================
       3) Ejecutar DIJKSTRA
       ============================================ */

    dijkstra(&grafo, origen, dist, prev);

    if(dist[destino] == INF) {
        printf("\nNo existe ruta posible entre los barrios.\n");
        return 0;
    }

    /* ============================================
       4) Mostrar la ruta más corta
       ============================================ */

    printf("\nDistancia minima desde %s a %s: %d\n",
        vecNombreBarrios[origen-1],
        vecNombreBarrios[destino-1],
        dist[destino]);

    mostrarRuta(origen, destino, prev);

    /* ============================================
       5) Obtener el camino exacto para mostrar colectivos
       ============================================ */
    int camino[20];
    int tope = 0;
    int actual = destino;

    while(actual != -1) {
        camino[tope++] = actual;
        if(actual == origen) break;
        actual = prev[actual];
    }

    /* Invertir el vector para dejarlo origen?destino */
    int i;
    for(i = 0; i < tope/2; i++) {
        int temp = camino[i];
        camino[i] = camino[tope-1-i];
        camino[tope-1-i] = temp;
    }

    /* ============================================
       6) Mostrar colectivos que sirven para la ruta
       ============================================ */
    mostrarColectivosDeRuta(&grafo, camino, tope);

    return 0;
}

