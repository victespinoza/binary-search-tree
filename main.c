#include "testing.h"
#include <stdio.h>
#include "abb.h"
#include "string.h"

void pruebas_abb_alumno(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

#ifndef CORRECTOR
bool imprimir(const char* clave, void* dato, void* extra);

int main(int argc, char *argv[])
{
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    //pruebas_abb_alumno();
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_guardar(abb,"m","m");
    abb_guardar(abb,"d","d");
    abb_guardar(abb,"e","e");
    abb_guardar(abb,"o","o");
    abb_guardar(abb,"u","u");
    abb_guardar(abb,"a","a");
    abb_guardar(abb,"t","t");
    abb_guardar(abb,"m","m");
    abb_in_order(abb, imprimir, NULL);
    printf("cantidad es: %zu \n",abb_cantidad(abb));
    abb_iter_t* iter = abb_iter_in_crear(abb);
    while (!abb_iter_in_al_final(iter)){
        char* clave = abb_iter_in_ver_actual(iter);
        printf("clave iter: %s\n",clave);
        abb_iter_in_avanzar(iter);
    }
    abb_iter_in_destruir(iter);
    if (abb_pertenece(abb,"w")){
        printf("pertenece");
    }
    abb_borrar(abb, "m");
    abb_borrar(abb, "w");
    abb_borrar(abb, "d");
    abb_in_order(abb, imprimir, NULL);
    printf("cantidad es: %zu \n",abb_cantidad(abb));

    abb_destruir(abb);
    return failure_count() > 0;
}

bool imprimir(const char* clave, void* dato, void* extra){
    printf("clave: %s, valor: %s \n",clave, dato);
    return true;
}
#endif
