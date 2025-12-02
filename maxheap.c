#include "maxheap.h"
#include <stdlib.h>

/** 
static void intercambiarHeap(Proceso *a, Proceso *b) {
    Proceso temp = *a;
    *a = *b;
    *b = temp;
}
**/

MaxHeap *crearMaxHeap(int capacidad) {
    MaxHeap *h = malloc(sizeof(MaxHeap));
    h->arr = malloc(sizeof(Proceso) * capacidad);
    h->capacidad = capacidad;
    h->tamano = 0;
    return h;
}

void destruirMaxHeap(MaxHeap *h) {
    free(h->arr);
    free(h);
}

int heapVacio(MaxHeap *h) {
    return h->tamano == 0;
}

void insertarHeap(MaxHeap *h, Proceso p) {
    if (h->tamano == h->capacidad) return;
    h->arr[h->tamano] = p;
    heapificarArribaHeap(h, h->tamano);
    h->tamano++;
}

Proceso extraerMaximo(MaxHeap *h) {
    if (h->tamano == 0) {
        Proceso vacio;
        vacio.id = -1;
        return vacio;
    }
    Proceso max = h->arr[0];
    h->arr[0] = h->arr[h->tamano - 1];
    h->tamano--;
    heapificarAbajoHeap(h, 0);
    return max;
}

void heapificarArribaHeap(MaxHeap *h, int indice) {
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
