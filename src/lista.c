#include "lista.h"
#include <stdlib.h>

Lista *crearLista() {
    Lista *l = malloc(sizeof(Lista));
    if (!l) return NULL;
    l->cabeza = NULL;
    return l;
}

int listaVacia(Lista *l) {
    if (!l) return 1;
    return l->cabeza == NULL;
}

void insertarFinal(Lista *l, Proceso p) {
    if (!l) return;

    Nodo *nuevo = malloc(sizeof(Nodo));
    if (!nuevo) return;

    nuevo->dato = p;
    nuevo->siguiente = NULL;

    if (l->cabeza == NULL) {
        l->cabeza = nuevo;
        return;
    }

    Nodo *act = l->cabeza;
    while (act->siguiente != NULL) {
        act = act->siguiente;
    }

    act->siguiente = nuevo;
}

int eliminarPorId(Lista *l, int id) {
    if (!l) return 0;

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
    if (!l) return NULL;

    Nodo *act = l->cabeza;
    while (act != NULL) {
        if (act->dato.id == id)
            return act;
        act = act->siguiente;
    }
    return NULL;
}

void destruirLista(Lista *l) {
    if (!l) return;

    Nodo *act = l->cabeza;
    while (act != NULL) {
        Nodo *sig = act->siguiente;
        free(act);
        act = sig;
    }
    free(l);
}

