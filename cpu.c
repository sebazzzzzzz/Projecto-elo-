#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

Proceso parsearLinea(const char *linea) {
    Proceso p;
    Fecha f;
    char nombre[50];
    int id, prioridad, estado, tiempo;
    int dia, mes, anio;

    sscanf(linea, "ID:%d Nombre:%49s Prioridad:%d Estado:%d Fecha:%d/%d/%d Tiempo:%d", &id, nombre, &prioridad, &estado, &dia, &mes, &anio, &tiempo);

    f.dia = dia;
    f.mes = mes;
    f.anio = anio;

    p = crearProceso(id, nombre, prioridad, estado, f, tiempo);
    return p;
}

void cargarArchivo(CPU *cpu, const char *archivo) {
    FILE *f = fopen(archivo, "r");
    if (!f) return;

    char linea[256];
    while (fgets(linea, sizeof(linea), f)) {
        Proceso p = parsearLinea(linea);
        registrarProceso(cpu, p);
    }

    fclose(f);
}

CPU *crearCPU(int capacidadCola, int capacidadHash) {
    CPU *cpu = malloc(sizeof(CPU));
    cpu->ready = crearColaPrioridad(capacidadCola);
    cpu->running = crearLista();
    cpu->finished = crearLista();
    cpu->tabla = crearHash(capacidadHash);
    cpu->numNucleos = 2;
    return cpu;
}

void destruirCPU(CPU *cpu) {
    destruirColaPrioridad(cpu->ready);
    destruirLista(cpu->running);
    destruirLista(cpu->finished);
    destruirHash(cpu->tabla);
    free(cpu);
}

void registrarProceso(CPU *cpu, Proceso p) {
    hashInsertar(cpu->tabla, p);
    insertarPrioridad(cpu->ready, p);
}

static int contarLista(Lista *l) {
    int c = 0;
    Nodo *n = l->cabeza;
    while (n != NULL) {
        c++;
        n = n->siguiente;
    }
    return c;
}

Proceso ejecutarSiguiente(CPU *cpu) {
    if (colaVacia(cpu->ready)) {
        Proceso vacio;
        vacio.id = -1;
        return vacio;
    }

    Proceso p = extraerMayorPrioridad(cpu->ready);
    p.estado = 1;
    insertarFinal(cpu->running, p);
    hashActualizar(cpu->tabla, p);
    return p;
}

int pausarProceso(CPU *cpu, int id) {
    Nodo *n = hashBuscar(cpu->tabla, id);
    if (n == NULL) return 0;

    if (eliminarPorId(cpu->running, id)) {
        Proceso p = n->dato;
        p.estado = 0;
        insertarPrioridad(cpu->ready, p);
        n->dato = p;
        return 1;
    }

    return 0;
}

int reanudarProceso(CPU *cpu, int id) {
    Nodo *n = hashBuscar(cpu->tabla, id);
    if (n == NULL) return 0;

    Proceso p = n->dato;
    if (p.estado == 3) return 0;

    eliminarPorId(cpu->running, id);
    eliminarPorId(cpu->finished, id);

    p.estado = 0;
    insertarPrioridad(cpu->ready, p);
    n->dato = p;

    return 1;
}

int finalizarProceso(CPU *cpu, int id) {
    Nodo *n = hashBuscar(cpu->tabla, id);
    if (n == NULL) return 0;

    if (eliminarPorId(cpu->running, id)) {
        Proceso p = n->dato;
        p.estado = 3;
        insertarFinal(cpu->finished, p);
        n->dato = p;
        return 1;
    }

    return 0;
}

int actualizarProceso(CPU *cpu, Proceso p) {
    Nodo *n = hashBuscar(cpu->tabla, p.id);
    if (n == NULL) return 0;

    n->dato = p;
    return 1;
}

void simular(CPU *cpu, int ticks) {
    for (int t = 0; t < ticks; t++) {

        while (contarLista(cpu->running) < cpu->numNucleos && !colaVacia(cpu->ready)) {
            Proceso p = extraerMayorPrioridad(cpu->ready);
            p.estado = 1;
            insertarFinal(cpu->running, p);
            hashInsertar(cpu->tabla, p);
        }

        Nodo *n = cpu->running->cabeza;
        Nodo *anterior = NULL;

        while (n != NULL) {
            Nodo *siguiente = n->siguiente;
            Proceso p = n->dato;

            p.tiempo_estimado -= 1;

            if (p.tiempo_estimado <= 0) {
                p.estado = 3;
                insertarFinal(cpu->finished, p);
                hashInsertar(cpu->tabla, p);

                char linea[256];
                proceso_a_string(&p, linea);
                printf("%s\n", linea);
                escribirLog(linea);
                escribirLog("\n");
            } else {
                p.estado = 0;
                insertarPrioridad(cpu->ready, p);
                hashInsertar(cpu->tabla, p);
            }

            if (anterior == NULL) {
                cpu->running->cabeza = siguiente;
            } else {
                anterior->siguiente = siguiente;
            }
            free(n);
            n = siguiente;
        }

        if (t % 5 == 0) {
            int running = contarLista(cpu->running);
            int ready = cpu->ready->tamano;
            int finished = contarLista(cpu->finished);

            char tickLine[128];
            sprintf(tickLine,
                    "[TICK %d] running=%d | ready=%d | finished=%d\n",
                    t, running, ready, finished);
            escribirLog(tickLine);
        }

        if (colaVacia(cpu->ready) && cpu->running->cabeza == NULL)
            break;
    }
}


int main() {
    CPU *cpu = crearCPU(200, 200);

    prepararLog("data/salida.txt");

    cargarArchivo(cpu, "data/caso_prueba.txt");

    simular(cpu, 3000);

    cerrarLog();

    destruirCPU(cpu);

    return 0;
}
