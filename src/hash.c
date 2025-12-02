#include "hash.h"
#include <stdlib.h>

HashTable *crearHash(int capacidad) {
    HashTable *h = malloc(sizeof(HashTable));
    h->capacidad = capacidad;
    h->cubetas = malloc(sizeof(Lista*) * capacidad);

    for (int i = 0; i < capacidad; i++)
        h->cubetas[i] = crearLista();

    return h;
}

void destruirHash(HashTable *h) {
    for (int i = 0; i < h->capacidad; i++)
        destruirLista(h->cubetas[i]);
    free(h->cubetas);
    free(h);
}

int hashFuncion(int id, int capacidad) {
    return id % capacidad;
}

void hashInsertar(HashTable *h, Proceso p) {
    int idx = hashFuncion(p.id, h->capacidad);
    insertarFinal(h->cubetas[idx], p);
}

Nodo *hashBuscar(HashTable *h, int id) {
    int idx = hashFuncion(id, h->capacidad);
    return buscarPorId(h->cubetas[idx], id);
}

int hashEliminar(HashTable *h, int id) {
    int idx = hashFuncion(id, h->capacidad);
    return eliminarPorId(h->cubetas[idx], id);
}

void hashActualizar(HashTable *h, Proceso p) {
    int idx = hashFuncion(p.id, h->capacidad);
    Nodo *n = buscarPorId(h->cubetas[idx], p.id);
    if (n != NULL)
        n->dato = p;
}
