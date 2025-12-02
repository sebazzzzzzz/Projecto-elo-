#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "proceso.h"

typedef struct {
    Proceso *arr;
    int capacidad;
    int tamano;
} MaxHeap;

MaxHeap *crearMaxHeap(int capacidad);

void destruirMaxHeap(MaxHeap *h);

void insertarHeap(MaxHeap *h, Proceso p);

Proceso extraerMaximo(MaxHeap *h);

int heapVacio(MaxHeap *h);

void heapificarArribaHeap(MaxHeap *h, int indice);

void heapificarAbajoHeap(MaxHeap *h, int indice);

#endif
