#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indice.h"

void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->vindice = (tamanyo *) malloc(nmemb * sizeof(tamanyo));
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
    indice->vindice = realloc(indice->vindice, (nmemb * INCREMENTO) * sizeof(tamanyo));
    if(!indice->vindice)
    {
        printf("No se ha podido asignar memoria\n");
        exit(ERROR);
    }
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = nmemb * INCREMENTO;
}
int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *))
{

}

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int
(*cmp)(const void *, const void *))
{

}

int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb,
size_t tamanyo, int (*cmp)(const void *, const void *))

int indice_lleno(const t_indice *indice)

int indice_vacio(const t_indice *indice)

int indice_lleno(const t_indice *indice)

void indice_vaciar(t_indice* indice)

int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
tamanyo, int (*cmp)(const void *, const void *))
{
    FILE *arch = fopen(path, "rb");
    if(!arch)
    {
        printf("No se ha podido abrir el archivo...\n");
        return ERROR;
        exit(ERROR);
    }
    while(fread())
}
