#ifndef PROCESO_H
#define PROCESO_H

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} EstadoProceso;

typedef struct {
    int pid;
    int prioridad;
    int tiempoCPU;
    int tiempoRestante;
    EstadoProceso estado;
} Proceso;

#endif
