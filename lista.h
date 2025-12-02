#ifndef LISTA_H
#define LISTA_H

#include "proceso.h"

typedef struct Nodo {
    Proceso dato;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *cabeza;
} Lista;

Lista *crearLista();

int listaVacia(Lista *l);

void insertarFinal(Lista *l, Proceso p);

int eliminarPorId(Lista *l, int id);

Nodo *buscarPorId(Lista *l, int id);

void destruirLista(Lista *l);

#endif
