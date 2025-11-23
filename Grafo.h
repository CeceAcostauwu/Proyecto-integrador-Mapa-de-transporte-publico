#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BARRIOS 8
#define INF 9999

typedef int tPeso;
typedef int tVertice;

typedef struct {
    tVertice origen;
    tVertice destino;
    tPeso peso;
} tArco;

/* Información del arco: peso + colectivos que pasan por ese tramo */
typedef struct {
    int peso;
    int colectivos[8];   /* IDs de colectivos, máximo 8 por tramo (ajustable) */
    int cantColectivos;
} tInfoArco;


typedef bool conjuntoVertices[MAX_BARRIOS+1];
typedef tInfoArco conjuntoArcos[MAX_BARRIOS+1][MAX_BARRIOS+1];

/* Grafo completo */
typedef struct {
    conjuntoVertices vertices;
    conjuntoArcos arcos;
} tGrafoPonderado;

/* ===== NOMBRES DE BARRIOS ===== */
typedef char tString[20];
tString vecNombreBarrios[8]={"Margen","Luna de Ketchup","asfalto","Separacion","Laguna Mojada","Brahma","El Cholo","San Catalino"};

/* ===== PROTOTIPPOS ===== */
void inicializarGrafo(tGrafoPonderado *g);
void agregarVertice(tGrafoPonderado *g, tVertice v);
void agregarColectivoAArco(tGrafoPonderado *g, int origen, int destino, int cole);
void agregarArcoP(tGrafoPonderado *g, tArco arc);
bool existeVertice(tGrafoPonderado g, int v);
void visualizarMatrizPesos(tGrafoPonderado g);
void visualizarMatrizAdyacencia(tGrafoPonderado g);
void cargarMapaTransporte(tGrafoPonderado *g);

void dijkstra(tGrafoPonderado *g, int origen, int dist[], int prev[]);
void mostrarRuta(int origen, int destino, int prev[]);
void mostrarColectivosDeRuta(tGrafoPonderado *g, int camino[], int tope);
void indicarCombinaciones(tGrafoPonderado *g, int camino[], int tope);

/* ===============================
        FUNCIONES DEL GRAFO
   =============================== */
   
/* Inicializa el grafo: vertices inactivos y arcos con peso=0 */
void inicializarGrafo(tGrafoPonderado *g) {
    int i, j;
    for (i = 1; i <= MAX_BARRIOS; i++) {
        g->vertices[i] = false;
        for (j = 1; j <= MAX_BARRIOS; j++) {
            g->arcos[i][j].peso = 0;
            g->arcos[i][j].cantColectivos = 0;
    	}
    }
}

/* Activa un vértice (1..MAX_BARRIOS) */
void agregarVertice(tGrafoPonderado *g, tVertice v) {
    g->vertices[v] = 1;
}

/* Agrega un arco ponderado (bidireccional). Inicializa cantColectivos en 0. */
void agregarArcoP(tGrafoPonderado *g, tArco arc) {
if (g->vertices[arc.origen] && g->vertices[arc.destino]) {
        g->arcos[arc.origen][arc.destino].peso = arc.peso;
        g->arcos[arc.origen][arc.destino].cantColectivos = 0;
        /* Copiar a la inversa (no dirigido) */
        g->arcos[arc.destino][arc.origen] = g->arcos[arc.origen][arc.destino];
    }
}

/* Agrega una línea de colectivo (cole) al arco origen-destino (bidireccional) */
void agregarColectivoAArco(tGrafoPonderado *g, int origen, int destino, int cole) {
    int k;
    /* verificar espacio y evitar duplicados */
    k = 0;
    while (k < g->arcos[origen][destino].cantColectivos) {
        if (g->arcos[origen][destino].colectivos[k] == cole) return; /* ya existe */
        k++;
    }
    /* agregar si hay lugar */
    if (g->arcos[origen][destino].cantColectivos < 8) {
        k = g->arcos[origen][destino].cantColectivos;
        g->arcos[origen][destino].colectivos[k] = cole;
        g->arcos[origen][destino].cantColectivos++;
        /* espejo */
        k = g->arcos[destino][origen].cantColectivos;
        g->arcos[destino][origen].colectivos[k] = cole;
        g->arcos[destino][origen].cantColectivos++;
    }
}

bool existeVertice(tGrafoPonderado g, int v) {
    return g.vertices[v] != 0;
}

void visualizarMatrizPesos(tGrafoPonderado g) {
    printf("\nMATRIZ DE PESOS\n");
    int i,j;
    for(i = 1; i <= MAX_BARRIOS; i++) {
        for(j = 1; j <= MAX_BARRIOS; j++) {
            printf("%3d ", g.arcos[i][j].peso);
        }
        printf("\n");
    }
}

void visualizarMatrizAdyacencia(tGrafoPonderado g) {
    int i, j;
    printf("\nMATRIZ DE ADYACENCIA\n");
    for (i = 1; i <= MAX_BARRIOS; i++) {
        for (j = 1; j <= MAX_BARRIOS; j++) {
            printf("%d ", (g.arcos[i][j].peso != 0) ? 1 : 0);
        }
        printf("\n");
    }
}

/* ===============================
        ACÁ CARGAMOS EL MAPA
   =============================== */

void cargarMapaTransporte(tGrafoPonderado *g) {
    tArco arc;
    int i, idx;

    /* activar vértices 1..MAX_BARRIOS */
    for (i = 1; i <= MAX_BARRIOS; i++) {
        agregarVertice(g, i);
    }

    /* conexiones: {origen, destino, peso} */
    {
        int conexiones[][3] = {
            {1,2,4}, {1,4,3}, {1,5,6},
            {4,5,3}, {2,5,4}, {2,3,4},
            {5,3,5}, {4,6,4}, {6,7,3},
            {4,7,2}, {7,5,4}, {7,8,7},
            {3,8,10}
        };
        int cant;
        cant = (int)(sizeof(conexiones) / sizeof(conexiones[0]));

        for (idx = 0; idx < cant; idx++) {
            arc.origen  = conexiones[idx][0];
            arc.destino = conexiones[idx][1];
            arc.peso    = conexiones[idx][2];
            agregarArcoP(g, arc);
        }
    }

    /* asignamos colectivos a los tramos (ejemplo) */
    /*101: 1-4-7-8*/
    agregarColectivoAArco(g, 1, 4, 101);
    agregarColectivoAArco(g, 4, 7, 101);
    agregarColectivoAArco(g, 7, 8, 101);
    /*102: 1-5-3-8*/
    agregarColectivoAArco(g, 1, 5, 102);
    agregarColectivoAArco(g, 5, 3, 102);
    agregarColectivoAArco(g, 3, 8, 102);
    /*103: 1-2-3*/
    agregarColectivoAArco(g, 1, 2, 103);
    agregarColectivoAArco(g, 2, 3, 103);
    /*104: 4-6-7-5*/
    agregarColectivoAArco(g, 4, 6, 104);
    agregarColectivoAArco(g, 6, 7, 104);
    agregarColectivoAArco(g, 7, 5, 104);
    /*105: 2-5-4*/
    agregarColectivoAArco(g, 2, 5, 105);
    agregarColectivoAArco(g, 5, 4, 105);
    /*106: 3-5-7-8*/
    agregarColectivoAArco(g, 3, 5, 106);
    agregarColectivoAArco(g, 5, 7, 106);
    agregarColectivoAArco(g, 7, 8, 106);
    /*107: 2-5-4-6*/
    agregarColectivoAArco(g, 2, 5, 107);
    agregarColectivoAArco(g, 5, 4, 107);
    agregarColectivoAArco(g, 4, 6, 107);
    /*108: 1-5-7-6*/
    agregarColectivoAArco(g, 1, 5, 108);
    agregarColectivoAArco(g, 5, 7, 108);
    agregarColectivoAArco(g, 7, 6, 108);
}

/* ===============================
        DIJKSTRA
   =============================== */

void dijkstra(tGrafoPonderado *g, int origen, int dist[], int prev[]) {
    bool visitado[MAX_BARRIOS+1];
	int i,k,u,v,minDist, nuevaDist;
    for(i = 1; i <= MAX_BARRIOS; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visitado[i] = false;
    }

    dist[origen] = 0;

    for(k = 1; k <= MAX_BARRIOS; k++) {
		
        u = -1, minDist = INF;

        for(i = 1; i <= MAX_BARRIOS; i++) {
            if(!visitado[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if(u == -1) break;
        visitado[u] = true;

        for (v = 1; v <= MAX_BARRIOS; v++) {
            if (g->arcos[u][v].peso > 0 && !visitado[v]) {
                nuevaDist = dist[u] + g->arcos[u][v].peso;
                if (nuevaDist < dist[v]) {
                    dist[v] = nuevaDist;
                    prev[v] = u;
                }
            }
    	}
    }
}

/* Reconstruye e imprime la ruta (usando prev[]) */
void mostrarRuta(int origen, int destino, int prev[]) {
    int camino[20];
    int tope = 0;
    int actual = destino;
    int i;

    while (actual != -1) {
        camino[tope++] = actual;
        if (actual == origen) break;
        actual = prev[actual];
    }

    /* Si no llegó al origen -> no hay ruta */
    if (camino[tope - 1] != origen) {
        printf("No existe ruta entre %d y %d\n", origen, destino);
        return;
    }

    printf("\nRuta mas optima:\n");
    for (i = tope - 1; i >= 0; i--) {
        int b = camino[i];
        printf("[%d] %s", b, vecNombreBarrios[b - 1]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

/* Muestra colectivos por tramo y los que recorren toda la ruta.
   Recibe: grafo, camino[] (lista de vertices en orden) y tope (cantidad) */
void mostrarColectivosDeRuta(tGrafoPonderado *g, int camino[], int tope) {
    int i, j, k;
    int a, b;
    int candidatos[20];
    int cantCand = 0;
    int hayAlguno = 0;

    printf("\n=====================================\n");
    printf(" COLECTIVOS QUE PASAN POR LA RUTA\n");
    printf("=====================================\n\n");

    /* 1) Mostrar colectivos por tramo */
    for (i = 0; i < tope - 1; i++) {
        a = camino[i];
        b = camino[i + 1];

        printf("Tramo %s -> %s: ",
            vecNombreBarrios[a - 1],
            vecNombreBarrios[b - 1]);

        if (g->arcos[a][b].cantColectivos == 0) {
            printf("No hay colectivos registrados\n");
        } else {
            for (k = 0; k < g->arcos[a][b].cantColectivos; k++) {
                printf("%d ", g->arcos[a][b].colectivos[k]);
            }
            printf("\n");
        }
    }

    printf("\n-------------------------------------\n");

    /* 2) Encontrar colectivos que recorren toda la ruta */
    /* Inicializar candidatos con colectivos del primer tramo */
    cantCand = 0;
    if (tope - 1 > 0 && g->arcos[camino[0]][camino[1]].cantColectivos > 0) {
        for (k = 0; k < g->arcos[camino[0]][camino[1]].cantColectivos; k++) {
            candidatos[cantCand++] = g->arcos[camino[0]][camino[1]].colectivos[k];
        }

        /* Para cada tramo siguiente, eliminar candidatos que no aparezcan */
        for (i = 1; i < tope - 1; i++) {
            a = camino[i];
            b = camino[i + 1];
            for (j = 0; j < cantCand; j++) {
                int cole = candidatos[j];
                int aparece = 0;
                for (k = 0; k < g->arcos[a][b].cantColectivos; k++) {
                    if (g->arcos[a][b].colectivos[k] == cole) {
                        aparece = 1;
                    }
                }
                if (!aparece) {
                    candidatos[j] = -1; /* marcar como eliminado */
                }
            }
        }
    }

    /* Mostrar resultado */
    printf("Colectivos que recorren toda la ruta: ");
    hayAlguno = 0;
    for (j = 0; j < cantCand; j++) {
        if (candidatos[j] != -1) {
            printf("%d ", candidatos[j]);
            hayAlguno = 1;
        }
    }
    if (!hayAlguno) {
        printf("Ninguno (se debe combinar lineas)\n");
        indicarCombinaciones(g, camino, tope);
    }
    printf("\n\n");
}

void indicarCombinaciones(tGrafoPonderado *g, int camino[], int tope) {
    int i, k;

    printf("\n=====================================\n");
    printf("  INDICACIONES DE VIAJE (COMBINACIONES)\n");
    printf("=====================================\n\n");

    if (tope < 2) {
        printf("Ruta invalida.\n");
        return;
    }

    /* --- 1) OBTENER TODOS LOS COLECTIVOS DEL PRIMER TRAMO --- */
    int actuales[10];
    int cantActuales = g->arcos[camino[0]][camino[1]].cantColectivos;

    if (cantActuales == 0) {
        printf("No hay colectivos que pasen por el primer tramo.\n");
        return;
    }

    for (k = 0; k < cantActuales; k++)
        actuales[k] = g->arcos[camino[0]][camino[1]].colectivos[k];

    /* Colectivo actual que estamos usando (si existe uno común) */
    int coleActual = -1;

    /* Elegimos el primero como candidato inicial */
    if (cantActuales > 0)
        coleActual = actuales[0];

    int inicioTramo = camino[0];

    /* --- 2) RECORRER CADA TRAMO Y VER SI EL COLECTIVO SIGUE SIRVIENDO --- */
    for (i = 0; i < tope - 1; i++) {
        int a = camino[i];
        int b = camino[i + 1];

        /* Verificar si el colectivo actual sigue apareciendo en este tramo */
        int sirve = 0;
        for (k = 0; k < g->arcos[a][b].cantColectivos; k++) {
            if (g->arcos[a][b].colectivos[k] == coleActual) {
                sirve = 1;
            }
        }

        /* Si NO sirve ? hay que bajar y combinar */
        if (!sirve) {
            /* Explicar tramo anterior */
            printf("Toma el colectivo %d desde %s hasta %s.\n",
                   coleActual,
                   vecNombreBarrios[inicioTramo - 1],
                   vecNombreBarrios[a - 1]);

            /* Buscar un nuevo colectivo para seguir */
            if (g->arcos[a][b].cantColectivos > 0) {
                coleActual = g->arcos[a][b].colectivos[0];
                printf("Cambia en %s y toma el colectivo %d.\n",
                       vecNombreBarrios[a - 1], coleActual);
                inicioTramo = a;
            } else {
                printf("No existe colectivo que continue desde %s.\n",
                       vecNombreBarrios[a - 1]);
                return;
            }
        }
    }

    /* --- 3) CERRAR EL ÚLTIMO TRAMO --- */
    printf("Toma el colectivo %d desde %s hasta %s y habras llegado a tu destino.\n",
           coleActual,
           vecNombreBarrios[inicioTramo - 1],
           vecNombreBarrios[camino[tope - 1] - 1]);

    printf("\n=====================================\n");
}

