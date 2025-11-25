#ifndef HEAP_H
#define HEAP_H
#include "proceso.h"

typedef struct {
    Proceso **datos;
    int tamaño;
    int capacidad;
} MaxHeap;

MaxHeap* crearHeap(int capacidad);
void insertarHeap(MaxHeap* heap, Proceso* p);
Proceso* extraerMax(MaxHeap* heap);
Proceso* verMax(MaxHeap* heap);

#endif
