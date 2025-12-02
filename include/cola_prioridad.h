#ifndef COLA_PRIORIDAD_H
#define COLA_PRIORIDAD_H

#include "proceso.h"

typedef struct {
    Proceso *arr;
    int capacidad;
    int tamano;
} ColaPrioridad;

ColaPrioridad *crearColaPrioridad(int capacidad);

void destruirColaPrioridad(ColaPrioridad *cp);

void insertarPrioridad(ColaPrioridad *cp, Proceso p);

Proceso extraerMayorPrioridad(ColaPrioridad *cp);

int colaVacia(ColaPrioridad *cp);

void heapificarArriba(ColaPrioridad *cp, int indice);

void heapificarAbajo(ColaPrioridad *cp, int indice);

#endif
