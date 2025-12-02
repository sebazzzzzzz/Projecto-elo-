#ifndef REPORTES_H
#define REPORTES_H

#include "lista.h"

typedef struct {
    Proceso *arr;
    int tamano;
} ArrayProcesos;

ArrayProcesos listaAArray(Lista *l);

void liberarArrayProcesos(ArrayProcesos ap);

void ordenarPorPrioridad(ArrayProcesos ap);

void ordenarPorFecha(ArrayProcesos ap);

void generarReporte(ArrayProcesos ap, char *buffer);

void prepararLog(const char *ruta);

void escribirLog(const char *texto);

void cerrarLog();

#endif
