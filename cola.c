#include "cola.h"
#include <stdlib.h>

typedef struct nodo{
    void** dato;
    struct nodo* siguiente;
} nodo_t;
struct cola{
    nodo_t* primero;
    nodo_t* ultimo;
};

cola_t* cola_crear(){
    cola_t* cola = malloc(sizeof(cola_t));
    if(cola == NULL){
        return NULL;
    }
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

bool cola_encolar(cola_t *cola, void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL){
        return false;
    }
    nodo->dato = valor;
    nodo->siguiente = NULL;
    if(cola_esta_vacia(cola)){
        cola->primero = nodo;
        cola->ultimo = nodo;
    }
    else{
        cola->ultimo->siguiente = nodo;
        cola->ultimo = nodo;
    }
    return true;
}

void* cola_desencolar(cola_t *cola){
    if(cola_esta_vacia(cola)){
        return NULL;
    }
    nodo_t* primero = cola->primero;
    cola->primero = cola->primero->siguiente;
    void* dato = primero->dato;
    free(primero);
    return dato;
}

void* cola_ver_primero(const cola_t *cola){
    if(cola_esta_vacia(cola)){
        return NULL;
    }
    return cola->primero->dato;
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->primero == NULL;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)){
    void* dato = cola_desencolar(cola);
    while (dato != NULL){
        if(destruir_dato != NULL){
            destruir_dato(dato);
        }
        dato = cola_desencolar(cola);
    }
    free(cola);
}

