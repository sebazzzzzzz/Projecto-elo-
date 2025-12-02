#include "reportes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ArrayProcesos listaAArray(Lista *l) {
    ArrayProcesos ap;
    ap.tamano = 0;

    Nodo *act = l->cabeza;
    while (act != NULL) {
        ap.tamano++;
        act = act->siguiente;
    }

    ap.arr = malloc(sizeof(Proceso) * ap.tamano);

    act = l->cabeza;
    int i = 0;
    while (act != NULL) {
        ap.arr[i++] = act->dato;
        act = act->siguiente;
    }

    return ap;
}

void liberarArrayProcesos(ArrayProcesos ap) {
    free(ap.arr);
}

static void intercambiar(Proceso *a, Proceso *b) {
    Proceso temp = *a;
    *a = *b;
    *b = temp;
}

void ordenarPorPrioridad(ArrayProcesos ap) {
    for (int i = 0; i < ap.tamano - 1; i++) {
        for (int j = 0; j < ap.tamano - i - 1; j++) {
            if (ap.arr[j].prioridad < ap.arr[j + 1].prioridad) {
                intercambiar(&ap.arr[j], &ap.arr[j + 1]);
            }
        }
    }
}

void ordenarPorFecha(ArrayProcesos ap) {
    for (int i = 0; i < ap.tamano - 1; i++) {
        for (int j = 0; j < ap.tamano - i - 1; j++) {
            Fecha f1 = ap.arr[j].fecha_vencimiento;
            Fecha f2 = ap.arr[j + 1].fecha_vencimiento;

            int v1 = f1.anio * 10000 + f1.mes * 100 + f1.dia;
            int v2 = f2.anio * 10000 + f2.mes * 100 + f2.dia;

            if (v1 > v2) {
                intercambiar(&ap.arr[j], &ap.arr[j + 1]);
            }
        }
    }
}

void generarReporte(ArrayProcesos ap, char *buffer) {
    buffer[0] = '\0';

    char linea[200];

    for (int i = 0; i < ap.tamano; i++) {
        Proceso *p = &ap.arr[i];

        sprintf(linea,
                "ID:%d Nombre:%s Prioridad:%d Estado:%d Fecha:%02d/%02d/%04d Tiempo:%d\n",
                p->id,
                p->nombre,
                p->prioridad,
                p->estado,
                p->fecha_vencimiento.dia,
                p->fecha_vencimiento.mes,
                p->fecha_vencimiento.anio,
                p->tiempo_estimado);

        strcat(buffer, linea);
    }
}

static FILE *logFile = NULL;

void prepararLog(const char *ruta) {
    logFile = fopen(ruta, "w");
}

void escribirLog(const char *texto) {
    if (logFile) {
        fputs(texto, logFile);
    }
}

void cerrarLog() {
    if (logFile) {
        fclose(logFile);
        logFile = NULL;
    }
}
