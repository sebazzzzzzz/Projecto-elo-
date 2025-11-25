#ifndef LISTA_H
#define LISTA_H
#include "proceso.h"

typedef struct NodoLista {
    Proceso *proc;
    struct NodoLista *sig;
} NodoLista;

typedef struct {
    NodoLista *cabeza;
    int tamaño;
} ListaProcesos;

ListaProcesos* crearLista();
void insertarLista(ListaProcesos* lista, Proceso* p);

#endif
