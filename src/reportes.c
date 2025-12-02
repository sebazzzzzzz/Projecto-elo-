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

    ap.arr = NULL;
    if (ap.tamano > 0) {
        ap.arr = malloc(sizeof(Proceso) * ap.tamano);
    }

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


/* ===================== MERGESORT POR PRIORIDAD ===================== */

// Mayor prioridad primero (orden descendente)
static int compararPrioridad(const Proceso *a, const Proceso *b) {
    if (a->prioridad > b->prioridad) return -1;
    if (a->prioridad < b->prioridad) return 1;
    return 0;
}

static void mergePrioridad(Proceso *arr, Proceso *temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (compararPrioridad(&arr[i], &arr[j]) <= 0) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

static void mergesortPrioridad(Proceso *arr, Proceso *temp, int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;
    mergesortPrioridad(arr, temp, left, mid);
    mergesortPrioridad(arr, temp, mid + 1, right);
    mergePrioridad(arr, temp, left, mid, right);
}

void ordenarPorPrioridad(ArrayProcesos ap) {
    if (ap.tamano <= 1 || ap.arr == NULL) return;

    Proceso *temp = malloc(sizeof(Proceso) * ap.tamano);
    if (!temp) return;

    mergesortPrioridad(ap.arr, temp, 0, ap.tamano - 1);

    free(temp);
}

/* ===================== MERGESORT POR FECHA ===================== */

// Más antigua primero (orden cronológico ascendente)
static int compararFecha(const Proceso *a, const Proceso *b) {
    Fecha f1 = a->fecha_vencimiento;
    Fecha f2 = b->fecha_vencimiento;

    int v1 = f1.anio * 10000 + f1.mes * 100 + f1.dia;
    int v2 = f2.anio * 10000 + f2.mes * 100 + f2.dia;

    if (v1 < v2) return -1;  // a es antes que b
    if (v1 > v2) return 1;   // a es después que b
    return 0;                // misma fecha
}

static void mergeFecha(Proceso *arr, Proceso *temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (compararFecha(&arr[i], &arr[j]) <= 0) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

static void mergesortFecha(Proceso *arr, Proceso *temp, int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;
    mergesortFecha(arr, temp, left, mid);
    mergesortFecha(arr, temp, mid + 1, right);
    mergeFecha(arr, temp, left, mid, right);
}

void ordenarPorFecha(ArrayProcesos ap) {
    if (ap.tamano <= 1 || ap.arr == NULL) return;

    Proceso *temp = malloc(sizeof(Proceso) * ap.tamano);
    if (!temp) return;

    mergesortFecha(ap.arr, temp, 0, ap.tamano - 1);

    free(temp);
}

/* ===================== REPORTE Y LOG ===================== */

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
