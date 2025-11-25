#ifndef HASH_H
#define HASH_H
#include "proceso.h"

typedef struct {
    int ocupado;
    int pid;
    Proceso* p;
} EntradaHash;

typedef struct {
    EntradaHash *tabla;
    int capacidad;
} TablaHash;

TablaHash* crearTablaHash(int capacidad);
void insertarHash(TablaHash *th, Proceso *p);
Proceso* buscarHash(TablaHash *th, int pid);

#endif
