#include "hash.h"
#include <stdlib.h>

HashTable *crearHash(int capacidad) {
    if (capacidad <= 0) {
        return NULL;
    }

    HashTable *h = malloc(sizeof(HashTable));
    if (!h) return NULL;

    h->capacidad = capacidad;
    h->cubetas = malloc(sizeof(Lista*) * capacidad);
    if (!h->cubetas) {
        free(h);
        return NULL;
    }

    for (int i = 0; i < capacidad; i++) {
        h->cubetas[i] = crearLista();
        if (!h->cubetas[i]) {
            // liberar lo ya creado
            for (int j = 0; j < i; j++) {
                destruirLista(h->cubetas[j]);
            }
            free(h->cubetas);
            free(h);
            return NULL;
        }
    }

    return h;
}

void destruirHash(HashTable *h) {
    if (!h) return;

    for (int i = 0; i < h->capacidad; i++) {
        destruirLista(h->cubetas[i]);
    }
    free(h->cubetas);
    free(h);
}

int hashFuncion(int id, int capacidad) {
    int r = id % capacidad;
    if (r < 0) r += capacidad;
    return r;
}

void hashInsertar(HashTable *h, Proceso p) {
    if (!h) return;
    int idx = hashFuncion(p.id, h->capacidad);
    insertarFinal(h->cubetas[idx], p);
}

Nodo *hashBuscar(HashTable *h, int id) {
    if (!h) return NULL;
    int idx = hashFuncion(id, h->capacidad);
    return buscarPorId(h->cubetas[idx], id);
}

int hashEliminar(HashTable *h, int id) {
    if (!h) return 0;
    int idx = hashFuncion(id, h->capacidad);
    return eliminarPorId(h->cubetas[idx], id);
}

void hashActualizar(HashTable *h, Proceso p) {
    if (!h) return;
    int idx = hashFuncion(p.id, h->capacidad);
    Nodo *n = buscarPorId(h->cubetas[idx], p.id);
    if (n != NULL) {
        n->dato = p;
    }
}