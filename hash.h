#ifndef HASH_H
#define HASH_H

#include "proceso.h"
#include "lista.h"

typedef struct {
    Lista **cubetas;
    int capacidad;
} HashTable;

HashTable *crearHash(int capacidad);

void destruirHash(HashTable *h);

int hashFuncion(int id, int capacidad);

void hashInsertar(HashTable *h, Proceso p);

Nodo *hashBuscar(HashTable *h, int id);

int hashEliminar(HashTable *h, int id);

void hashActualizar(HashTable *h, Proceso p);

#endif
