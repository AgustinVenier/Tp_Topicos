#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indice.h"

void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->vindice = malloc(nmemb * tamanyo);
    if(!indice->vindice)
    {
        printf("No se ha podido asignar memoria\n");
        exit(ERROR);
    }
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = nmemb;
}

void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->vindice = realloc(indice->vindice, (nmemb * INCREMENTO) * tamanyo);   ///a preguntar (nmemb * INCREMENTO)
    if(!indice->vindice)
    {
        printf("No se ha podido asignar memoria\n");
        exit(ERROR);
    }
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = nmemb * INCREMENTO; ///VA A DAR CON UN NUMERO DECIMAL
}
int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *))
{
    int i;
    // Buscar si ya existe
    if(indice_buscar(indice,registro,indice->cantidad_elementos_actual,tamanyo,cmp)!=NO_EXISTE){
        return ERROR;
    }
    // Verificar capacidad
    if (indice_lleno==OK){
        indice_redimensionar(indice,indice->cantidad_elementos_actual,tamanyo);
    }
    // Insertar al final
    memcpy(indice->vindice+(indice->cantidad_elementos_actual*tamanyo), registro, tamanyo);
    indice->cantidad_elementos_actual++; ///no se si funciona

    return OK;
}

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int
(*cmp)(const void *, const void *))
{
    int i, pos = -1;
    for (i = 0; i < indice->cant; i++) // Buscar posición del registro a eliminar
    {
        void *elem = (char *)indice->registros + i * tamanyo;
        if (cmp(registro, elem) == 0)
            pos = i;
    }
    if (pos == -1)// Si no se encontró
        return 0;

    for (i = pos; i < indice->cant - 1; i++) // Mover los registros siguientes una posición atrás
    {
        memcpy((char *)indice->registros + i * tamanyo,(char *)indice->registros + (i + 1) * tamanyo, tamanyo);
    }

    indice->cant--; // Decrementar cantidad de registros
    return EXITO;
}

int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb,
size_t tamanyo, int (*cmp)(const void *, const void *))
{
    return busquedaBinaria(indice->vindice,registro,nmemb,tamanyo,cmp);
}

int indice_vacio(const t_indice *indice)
{
    if(indice->cantidad_elementos_actual == 0)
        return OK;
    return ERROR;
}
int indice_lleno(const t_indice *indice)
{
    if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
        return OK;
    return ERROR;
}

void indice_vaciar(t_indice* indice)
{
    free(indice->vindice);
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_total = 0;
    indice->vindice == NULL; ///no se si va esto
}

int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
tamanyo, int (*cmp)(const void *, const void *)) ///no se si esta bien, revisar
{
    FILE *arch = fopen(path, "rb");
    if(!arch)
    {
        return ERROR;
    }

    while(fread(vreg_ind, tamanyo, 1, arch))
    {
        indice_insertar(indice,vreg_ind,tamanyo,cmp);
    }

    close(arch);
    return OK;
}



int cmp_por_dni(const void *a, const void *b)
{
    const t_reg_indice *r1 = (const t_reg_indice *)a;
    const t_reg_indice *r2 = (const t_reg_indice *)b;

    return (r1->dni - r2->dni);
}

int busquedaBinaria(void *vec, void *buscado, unsigned *cantelem, size_t tamanyo, int(*cmp)(const void *, const void*)){
    int i = 0, f = (*cantelem) - 1, medio, res;

    while(i <= f){

        medio = (i + f)/2;

        res = cmp(buscado, vec + (medio * tamanyo));

        if(!res){
            //Devuelve la posicion donde se encontro el elemento
            return medio;
        } else if(res < 0){
            f = medio - 1;
        } else {
            i = medio + 1;
        }
    }

    return NO_EXISTE;
}
