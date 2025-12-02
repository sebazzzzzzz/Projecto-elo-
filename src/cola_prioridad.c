#include "cola_prioridad.h"
#include <stdlib.h>

/** 
static void intercambiar(Proceso *a, Proceso *b) {
    Proceso temp = *a;
    *a = *b;
    *b = temp;
}
**/

ColaPrioridad *crearColaPrioridad(int capacidad) {
    ColaPrioridad *cp = malloc(sizeof(ColaPrioridad));
    cp->arr = malloc(sizeof(Proceso) * capacidad);
    cp->capacidad = capacidad;
    cp->tamano = 0;
    return cp;
}

void destruirColaPrioridad(ColaPrioridad *cp) {
    free(cp->arr);
    free(cp);
}

int colaVacia(ColaPrioridad *cp) {
    return cp->tamano == 0;
}

void insertarPrioridad(ColaPrioridad *cp, Proceso p) {
    if (cp->tamano == cp->capacidad) return;
    cp->arr[cp->tamano] = p;
    heapificarArriba(cp, cp->tamano);
    cp->tamano++;
}

Proceso extraerMayorPrioridad(ColaPrioridad *cp) {
    if (cp->tamano == 0) {
        Proceso vacio;
        vacio.id = -1;
        return vacio;
    }
    Proceso max = cp->arr[0];
    cp->arr[0] = cp->arr[cp->tamano - 1];
    cp->tamano--;
    heapificarAbajo(cp, 0);
    return max;
}

void heapificarArriba(ColaPrioridad *cp, int indice) {
    while (indice > 0) {
        int padre = (indice - 1) / 2;
        if (cp->arr[indice].prioridad > cp->arr[padre].prioridad) {
            Proceso temp = cp->arr[indice];
            cp->arr[indice] = cp->arr[padre];
            cp->arr[padre] = temp;
            indice = padre;
        } else {
            break;
        }
    }
}

void heapificarAbajo(ColaPrioridad *cp, int indice) {
    while (1) {
        int izq = 2 * indice + 1;
        int der = 2 * indice + 2;
        int mayor = indice;

        if (izq < cp->tamano && cp->arr[izq].prioridad > cp->arr[mayor].prioridad)
            mayor = izq;

        if (der < cp->tamano && cp->arr[der].prioridad > cp->arr[mayor].prioridad)
            mayor = der;

        if (mayor != indice) {
            Proceso temp = cp->arr[indice];
            cp->arr[indice] = cp->arr[mayor];
            cp->arr[mayor] = temp;
            indice = mayor;
        } else {
            break;
        }
    }
}
