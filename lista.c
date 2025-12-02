#include "lista.h"
#include <stdlib.h>

Lista *crearLista() {
    Lista *l = malloc(sizeof(Lista));
    l->cabeza = NULL;
    return l;
}

int listaVacia(Lista *l) {
    return l->cabeza == NULL;
}

void insertarFinal(Lista *l, Proceso p) {
    Nodo *nuevo = malloc(sizeof(Nodo));
    nuevo->dato = p;
    nuevo->siguiente = NULL;

    if (l->cabeza == NULL) {
        l->cabeza = nuevo;
        return;
    }

    Nodo *act = l->cabeza;
    while (act->siguiente != NULL)
        act = act->siguiente;

    act->siguiente = nuevo;
}

int eliminarPorId(Lista *l, int id) {
    Nodo *act = l->cabeza;
    Nodo *prev = NULL;

    while (act != NULL) {
        if (act->dato.id == id) {
            if (prev == NULL)
                l->cabeza = act->siguiente;
            else
                prev->siguiente = act->siguiente;

            free(act);
            return 1;
        }
        prev = act;
        act = act->siguiente;
    }

    return 0;
}

Nodo *buscarPorId(Lista *l, int id) {
    Nodo *act = l->cabeza;
    while (act != NULL) {
        if (act->dato.id == id)
            return act;
        act = act->siguiente;
    }
    return NULL;
}

void destruirLista(Lista *l) {
    Nodo *act = l->cabeza;
    while (act != NULL) {
        Nodo *sig = act->siguiente;
        free(act);
        act = sig;
    }
    free(l);
}
