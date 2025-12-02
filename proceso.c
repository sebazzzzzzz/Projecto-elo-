#include "proceso.h"

Proceso crearProceso(int id, const char *nombre, int prioridad, int estado, Fecha venc, int tiempo) {
    Proceso p;
    p.id = id;
    strncpy(p.nombre, nombre, 49);
    p.nombre[49] = '\0';
    p.prioridad = prioridad;
    p.estado = estado;
    p.fecha_vencimiento = venc;
    p.tiempo_estimado = tiempo;
    return p;
}

void actualizar_prioridad(Proceso *p, int nueva_prioridad) {
    p->prioridad = nueva_prioridad;
}

void actualizar_estado(Proceso *p, int nuevo_estado) {
    p->estado = nuevo_estado;
}

void actualizar_vencimiento(Proceso *p, Fecha nueva_fecha) {
    p->fecha_vencimiento = nueva_fecha;
}

void proceso_a_string(const Proceso *p, char *buffer) {
    sprintf(buffer, "ID:%d Nombre:%s Prioridad:%d Estado:%d Fecha:%02d/%02d/%04d Tiempo:%d", p->id, p->nombre, p->prioridad, p->estado, p->fecha_vencimiento.dia, p->fecha_vencimiento.mes, p->fecha_vencimiento.anio, p->tiempo_estimado);
}
