#include "maxheap.h"
#include <stdlib.h>

MaxHeap *crearMaxHeap(int capacidad) {
    if (capacidad <= 0) {
        return NULL;
    }

    MaxHeap *h = malloc(sizeof(MaxHeap));
    if (!h) return NULL;

    h->arr = malloc(sizeof(Proceso) * capacidad);
    if (!h->arr) {
        free(h);
        return NULL;
    }

    h->capacidad = capacidad;
    h->tamano = 0;
    return h;
}

void destruirMaxHeap(MaxHeap *h) {
    if (!h) return;
    free(h->arr);
    free(h);
}

int heapVacio(MaxHeap *h) {
    if (!h) return 1;
    return h->tamano == 0;
}

void insertarHeap(MaxHeap *h, Proceso p) {
    if (!h) return;

    if (h->tamano == h->capacidad) {
        // printf("MaxHeap lleno, no se puede insertar\n");
        return;
    }

    h->arr[h->tamano] = p;
    h->tamano++;
    heapificarArribaHeap(h, h->tamano - 1);
}

Proceso extraerMaximo(MaxHeap *h) {
    Proceso vacio;
    vacio.id = -1;

    if (!h || h->tamano == 0) {
        return vacio;   // o crearProcesoVacio()
    }

    Proceso max = h->arr[0];
    h->arr[0] = h->arr[h->tamano - 1];
    h->tamano--;
    heapificarAbajoHeap(h, 0);
    return max;
}

void heapificarArribaHeap(MaxHeap *h, int indice) {
    if (!h) return;

    while (indice > 0) {
        int padre = (indice - 1) / 2;
        if (h->arr[indice].prioridad > h->arr[padre].prioridad) {
            Proceso temp = h->arr[indice];
            h->arr[indice] = h->arr[padre];
            h->arr[padre] = temp;
            indice = padre;
        } else {
            break;
        }
    }
}

void heapificarAbajoHeap(MaxHeap *h, int indice) {
    if (!h) return;

    while (1) {
        int izq = 2 * indice + 1;
        int der = 2 * indice + 2;
        int mayor = indice;

        if (izq < h->tamano && h->arr[izq].prioridad > h->arr[mayor].prioridad)
            mayor = izq;

        if (der < h->tamano && h->arr[der].prioridad > h->arr[mayor].prioridad)
            mayor = der;

        if (mayor != indice) {
            Proceso temp = h->arr[indice];
            h->arr[indice] = h->arr[mayor];
            h->arr[mayor] = temp;
            indice = mayor;
        } else {
            break;
        }
    }
}
