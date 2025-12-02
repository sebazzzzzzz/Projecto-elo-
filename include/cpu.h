#ifndef CPU_H
#define CPU_H

#include "proceso.h"
#include "cola_prioridad.h"
#include "lista.h"
#include "hash.h"
#include "reportes.h"

typedef struct {
    ColaPrioridad *ready;
    Lista *running;
    Lista *blocked;
    Lista *finished;
    HashTable *tabla;
    int numNucleos;
} CPU;

Proceso parsearLinea(const char *line);

void cargarArchivo(CPU *cpu, const char *archivo);

CPU *crearCPU(int capacidadCola, int capacidadHash);

void destruirCPU(CPU *cpu);

void registrarProceso(CPU *cpu, Proceso p);

Proceso ejecutarSiguiente(CPU *cpu);

int pausarProceso(CPU *cpu, int id);

int reanudarProceso(CPU *cpu, int id);

int finalizarProceso(CPU *cpu, int id);

int actualizarProceso(CPU *cpu, Proceso p);

void simular(CPU *cpu, int ticks);

#endif
