#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "reportes.h"
#include "maxheap.h"

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
    if (!f) 
    {printf("ERROR: no se pudo abrir el archivo '%s'\n", archivo);
        return;
    }
    char linea[256];
    int count = 0;
    while (fgets(linea, sizeof(linea), f)) {
        printf("Leyendo linea: %s", linea);
        Proceso p = parsearLinea(linea);
        registrarProceso(cpu, p);
        count++;
    }

    fclose(f);
    printf("Se cargaron %d procesos desde '%s'\n", count, archivo);
}

CPU *crearCPU(int capacidadCola, int capacidadHash) {
    CPU *cpu = malloc(sizeof(CPU));
    if (!cpu) return NULL;

    cpu->ready   = crearColaPrioridad(capacidadCola);
    cpu->blocked = crearLista();
    cpu->running = crearLista();
    cpu->finished= crearLista();
    cpu->tabla   = crearHash(capacidadHash);
    cpu->terminadosHeap = crearMaxHeap(capacidadCola);
    cpu->numNucleos = 2;
    cpu->quantum = 5; 


    return cpu;
}

void destruirCPU(CPU *cpu) {
    if (!cpu) return;
    destruirColaPrioridad(cpu->ready);
    destruirLista(cpu->running);
    destruirLista(cpu->blocked);
    destruirLista(cpu->finished);
    destruirHash(cpu->tabla);
    destruirMaxHeap(cpu->terminadosHeap);

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
        p.estado = 2;              
        insertarFinal(cpu->blocked, p);
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


    if (!eliminarPorId(cpu->blocked, id)) {

        return 0;
    }

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
            hashActualizar(cpu->tabla, p);
        }

        Nodo *n = cpu->running->cabeza;

        cpu->running->cabeza = NULL;

        while (n != NULL) {
            Nodo *siguiente = n->siguiente;
            Proceso p = n->dato;
            free(n);

            p.tiempo_estimado -= 1;
            p.ticksCPU += 1;

            if (p.tiempo_estimado <= 0) {
                p.estado = 3;
                p.ticksCPU = 0;
                insertarFinal(cpu->finished, p);
                insertarHeap(cpu->terminadosHeap, p);
                hashActualizar(cpu->tabla, p);

                char linea[256];
                proceso_a_string(&p, linea);
                printf("TERMINA -> %s\n", linea);
                escribirLog(linea);
                escribirLog("\n");
            }
            else if (p.ticksCPU >= cpu->quantum) {
                p.estado = 0;
                p.ticksCPU = 0;
                insertarPrioridad(cpu->ready, p);
                hashActualizar(cpu->tabla, p);
            }
            else {
                insertarFinal(cpu->running, p);
            }

            n = siguiente;
        }

        if (t % 5 == 0) {
            int running = contarLista(cpu->running);
            int ready = cpu->ready->tamano;
            int finished = contarLista(cpu->finished);

            char tickLine[128];
            sprintf(tickLine, "[TICK %d] running=%d | ready=%d | finished=%d\n",
                    t, running, ready, finished);
            escribirLog(tickLine);
            printf("%s", tickLine);
        }

        if (colaVacia(cpu->ready) && cpu->running->cabeza == NULL) {
            printf("Simulacion termina en TICK %d (no hay procesos activos)\n", t);
            break;
        }
    }
}


void generarReporteFinal(CPU *cpu) {
    ArrayProcesos ap = listaAArray(cpu->finished);

    if (ap.tamano == 0) {
        printf("No hay procesos finalizados, no se puede generar reporte.\n");
        return;
    }


    ordenarPorPrioridad(ap); 

    char bufferPrioridad[8192];  
    generarReporte(ap, bufferPrioridad);

    printf("\n===== REPORTE ORDENADO POR PRIORIDAD (mayor a menor) =====\n");
    printf("%s", bufferPrioridad);

    FILE *fprio = fopen("data/reporte_prioridad.txt", "w");
    if (fprio) {
        fputs(bufferPrioridad, fprio);
        fclose(fprio);
    } else {
        printf("No se pudo crear data/reporte_prioridad.txt\n");
    }


    ordenarPorFecha(ap);  

    char bufferFecha[8192];
    generarReporte(ap, bufferFecha);

    printf("\n===== REPORTE ORDENADO POR FECHA (más antigua primero) =====\n");
    printf("%s", bufferFecha);

    FILE *ffecha = fopen("data/reporte_fecha.txt", "w");
    if (ffecha) {
        fputs(bufferFecha, ffecha);
        fclose(ffecha);
    } else {
        printf("No se pudo crear data/reporte_fecha.txt\n");
    }


    liberarArrayProcesos(ap);
}



int main() {
    CPU *cpu = crearCPU(200, 200);
    int num_caso = 0;

    prepararLog("data/salida.txt");

    printf("Bienvenido! introduce el número de caso a probar (1,2,3): ");
    scanf("%d", &num_caso);

    while (num_caso < 1 || num_caso > 3) {
        printf("Entrada invalida. Seleccione 1-3: ");
        scanf("%d", &num_caso);
    }

    char nombreArchivo[64];
    sprintf(nombreArchivo, "data/caso_prueba%d.txt", num_caso);

    printf("Cargando archivo: %s\n", nombreArchivo);
    cargarArchivo(cpu, nombreArchivo);

    simular(cpu, 3000);

    cerrarLog();

    generarReporteFinal(cpu);

    destruirCPU(cpu);

    return 0;
}

