#include "cola_prioridad.h"
#include <stdlib.h>

ColaPrioridad *crearColaPrioridad(int capacidad) {
    if (capacidad <= 0) {
        return NULL;
    }

    ColaPrioridad *cp = malloc(sizeof(ColaPrioridad));
    if (!cp) return NULL;

    cp->arr = malloc(sizeof(Proceso) * capacidad);
    if (!cp->arr) {
        free(cp);
        return NULL;
    }

    cp->capacidad = capacidad;
    cp->tamano = 0;
    return cp;
}

void destruirColaPrioridad(ColaPrioridad *cp) {
    if (!cp) return;
    free(cp->arr);
    free(cp);
}

int colaVacia(ColaPrioridad *cp) {
    if (!cp) return 1;
    return cp->tamano == 0;
}

void insertarPrioridad(ColaPrioridad *cp, Proceso p) {
    if (!cp) return;

    if (cp->tamano == cp->capacidad) {
         printf("ColaPrioridad llena, no se puede insertar\n");
        return;
    }

    // Insertamos al final
    cp->arr[cp->tamano] = p;
    cp->tamano++;

    // Reordenamos hacia arriba
    heapificarArriba(cp, cp->tamano - 1);
}

static Proceso crearProcesoVacio(void) {
    Proceso vacio;
    vacio.id = -1;
    vacio.nombre[0] = '\0';
    vacio.prioridad = 0;
    vacio.estado = 0;
    vacio.tiempo_estimado = 0;
    vacio.fecha_vencimiento.dia = 0;
    vacio.fecha_vencimiento.mes = 0;
    vacio.fecha_vencimiento.anio = 0;
    return vacio;
}

Proceso extraerMayorPrioridad(ColaPrioridad *cp) {
    if (!cp || cp->tamano == 0) {
        return crearProcesoVacio();
    }

    Proceso max = cp->arr[0];
    cp->arr[0] = cp->arr[cp->tamano - 1];
    cp->tamano--;
    heapificarAbajo(cp, 0);
    return max;
}

void heapificarArriba(ColaPrioridad *cp, int indice) {
    if (!cp) return;

    while (indice > 0) {
        int padre = (indice - 1) / 2;
        if (cp->arr[indice].prioridad > cp->arr[padre].prioridad) {
            Proceso temp = cp->arr[indice];
            cp->arr[indice] = cp->arr[padre];
            cp->arr[padre] = temp;
            indice = padre;
        } else {
            break;
        }
    }
}

void heapificarAbajo(ColaPrioridad *cp, int indice) {
    if (!cp) return;

    while (1) {
        int izq = 2 * indice + 1;
        int der = 2 * indice + 2;
        int mayor = indice;

        if (izq < cp->tamano && cp->arr[izq].prioridad > cp->arr[mayor].prioridad)
            mayor = izq;

        if (der < cp->tamano && cp->arr[der].prioridad > cp->arr[mayor].prioridad)
            mayor = der;

        if (mayor != indice) {
            Proceso temp = cp->arr[indice];
            cp->arr[indice] = cp->arr[mayor];
            cp->arr[mayor] = temp;
            indice = mayor;
        } else {
            break;
        }
    }
}
