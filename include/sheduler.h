#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "heap.h"
#include "lista.h"
#include "hash.h"

typedef struct {
    MaxHeap *heap;
    ListaProcesos *lista;
    TablaHash *tablaPID;
} Scheduler;

Scheduler* crearScheduler(int maxProcesos);
void agregarProceso(Scheduler* s, int pid, int prioridad, int tiempoCPU);
Proceso* ejecutarSiguiente(Scheduler* s);

#endif
