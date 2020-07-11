#include "abb.h"
#include "stdlib.h"
#include "string.h"
#include "cola.h"

typedef struct nodo{
    struct nodo* der;
    struct nodo* izq;
    char* clave;
    void* dato;
} nodo_t;
struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t destruir;
};
struct abb_iter{
    cola_t* cola_inorder;
};

nodo_t* crear_nodo(const char* clave, void* dato);
nodo_t* _abb_guardar(nodo_t* nodo, const char* clave, void* dato, abb_t* arbol);
nodo_t* _buscar_nodo(nodo_t* nodo, const char* clave, abb_comparar_clave_t comparar);
void _destruir_nodos(nodo_t* nodo, abb_destruir_dato_t destruir);
bool _abb_in_order(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra);
void* _abb_borrar(nodo_t* nodo, const char *clave, abb_t *arbol, nodo_t** eliminado);
bool _encolar_in_order(const char* clave, void* dato, void *extra);
nodo_t* minimo_nodo(nodo_t* nodo);
void cambiar_clave_dato(nodo_t* nodo, nodo_t* minimo);

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (abb == NULL){
        return NULL;
    }
    abb->cantidad = 0;
    abb->raiz = NULL;
    abb->comparar = cmp;
    abb->destruir = destruir_dato;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t* nodo =  _abb_guardar(arbol->raiz, clave, dato, arbol);
    if (nodo == NULL){
        return false;
    }
    if (abb_cantidad(arbol) == 1){
        arbol->raiz = nodo;
    }
    return true;
}

void* abb_borrar(abb_t *arbol, const char *clave){
    nodo_t** eliminado = malloc(sizeof(nodo_t*));
    arbol->raiz = _abb_borrar(arbol->raiz, clave, arbol, eliminado);
    if (eliminado[0] == NULL){
        free(eliminado);
        return NULL;
    }
    free(eliminado[0]->clave);
    void* valor = eliminado[0]->dato;
    free(eliminado[0]);
    free(eliminado);
    arbol->cantidad--;
    return valor;
}

void* abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t* nodo = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
    if (nodo == NULL){
        return NULL;
    }
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_t* nodo = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
    return nodo != NULL;
}

void abb_destruir(abb_t *arbol){
    _destruir_nodos(arbol->raiz, arbol->destruir);
    free(arbol);
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    _abb_in_order(arbol->raiz, visitar, extra);
}

abb_iter_t* abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    cola_t* cola = cola_crear();
    _abb_in_order(arbol->raiz, _encolar_in_order, cola);
    iter->cola_inorder = cola;
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (!cola_esta_vacia(iter->cola_inorder)){
        cola_desencolar(iter->cola_inorder);
        return true;
    }
    return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (!cola_esta_vacia(iter->cola_inorder)){
        return cola_ver_primero(iter->cola_inorder);
    }
    return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return cola_esta_vacia(iter->cola_inorder);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    cola_destruir(iter->cola_inorder, NULL);
    free(iter);
}

/** FUNCIONES AUXILIARES **/

bool _encolar_in_order(const char* clave, void* dato, void *extra){
    cola_t* cola = extra;
    cola_encolar(cola, (void*)clave);
    return true;
}

bool _abb_in_order(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra){
    if (nodo == NULL){
        return true;
    }
    bool avanzar = _abb_in_order(nodo->izq, visitar, extra);
    if(avanzar && !visitar(nodo->clave, nodo->dato, extra)){
        return false;
    }
    if (avanzar){
        avanzar = _abb_in_order(nodo->der, visitar, extra);
    }
    return avanzar;
}

nodo_t* _abb_guardar(nodo_t* nodo, const char* clave, void* dato, abb_t* arbol){
    if (nodo == NULL){
        arbol->cantidad++;
        return crear_nodo(clave, dato);
    }
    if (arbol->comparar(clave, nodo->clave) == 0){
        if (arbol->destruir != NULL){
            void* dato_a_eliminar = nodo->dato;
            arbol->destruir(dato_a_eliminar);
        }
        nodo->dato = dato;
        return nodo;
    }
    if (arbol->comparar(clave, nodo->clave) < 0){
        nodo->izq  = _abb_guardar(nodo->izq, clave, dato, arbol);
    }
    else if (arbol->comparar(clave, nodo->clave) > 0){
        nodo->der = _abb_guardar(nodo->der, clave, dato, arbol);
    }
    return nodo;
}

void* _abb_borrar(nodo_t* nodo, const char *clave, abb_t *arbol, nodo_t** eliminado){
    if (nodo == NULL) {
        eliminado[0] = NULL;
        return NULL;
    }
    if (arbol->comparar(clave, nodo->clave) < 0){
        nodo->izq =  _abb_borrar(nodo->izq, clave, arbol, eliminado);
    }
    else if (arbol->comparar(clave, nodo->clave) > 0) {
        nodo->der =  _abb_borrar(nodo->der, clave, arbol, eliminado);
    }
    else {
        if (nodo->izq == NULL) {
            eliminado[0] = nodo;
            return nodo->der;
        } else if (nodo->der == NULL) {
            eliminado[0] = nodo;
            return nodo->izq;
        }
        nodo_t* minimo = minimo_nodo(nodo->der);
        cambiar_clave_dato(nodo, minimo);
        nodo->der = _abb_borrar(nodo->der, minimo->clave, arbol, eliminado);
        eliminado[0] = minimo;
    }
    return nodo;
}

nodo_t* _buscar_nodo(nodo_t* nodo, const char* clave, abb_comparar_clave_t comparar){
    if (nodo == NULL){
        return NULL;
    }
    if (comparar(clave, nodo->clave) < 0){
        return _buscar_nodo(nodo->izq, clave, comparar);
    }
    else if (comparar(clave, nodo->clave) > 0){
        return _buscar_nodo(nodo->der, clave, comparar);
    }
    return nodo;

}

void _destruir_nodos(nodo_t* nodo, abb_destruir_dato_t destruir){
    if (nodo == NULL){
        return;
    }
    _destruir_nodos(nodo->izq, destruir);
    _destruir_nodos(nodo->der, destruir);
    if (destruir != NULL){
        destruir(nodo->dato);
    }
    free(nodo->clave);
    free(nodo);
}

nodo_t* crear_nodo(const char* clave, void* dato){
    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    nuevo_nodo->izq = NULL;
    nuevo_nodo->der = NULL;
    nuevo_nodo->dato = dato;
    nuevo_nodo->clave = malloc(sizeof(char)*(strlen(clave)+1));
    nuevo_nodo->clave = strcpy(nuevo_nodo->clave, clave);
    return nuevo_nodo;
}

nodo_t* minimo_nodo(nodo_t* nodo) {
    if (nodo != NULL && nodo->izq == NULL){
        return nodo;
    }
    return minimo_nodo(nodo->izq);
}

void cambiar_clave_dato(nodo_t* nodo, nodo_t* minimo){
    char* clave = nodo->clave;
    void* dato = nodo->dato;
    nodo->clave = minimo->clave;
    nodo->dato = minimo->dato;
    minimo->clave = clave;
    minimo->dato = dato;
}