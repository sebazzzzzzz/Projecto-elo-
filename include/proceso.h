#ifndef PROCESO_H
#define PROCESO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

typedef struct {
    int id;
    char nombre[50];
    int prioridad;
    int estado;
    Fecha fecha_vencimiento;
    int tiempo_estimado;
    int ticksCPU;
} Proceso;

Proceso crearProceso(int id, const char *nombre, int prioridad, int estado, Fecha venc, int tiempo);

void actualizar_prioridad(Proceso *p, int nueva_prioridad);

void actualizar_estado(Proceso *p, int nuevo_estado);

void actualizar_vencimiento(Proceso *p, Fecha nueva_fecha);

void proceso_a_string(const Proceso *p, char *buffer);

#endif
