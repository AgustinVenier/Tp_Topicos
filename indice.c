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
    int i;
    // Buscar si ya existe
    for (i = 0; i < indice->cant; i++)
    {
        if (cmp(registro, &indice->registros[i]) == 0)
        {
            printf("Error: registro duplicado.\n");
            return ERROR;
        }
    }
    // Verificar capacidad
    if (indice->cant >= 1000)
    {
        printf("Error: índice lleno.\n");
        return ERROR;
    }
    // Insertar al final
    memcpy(&indice->registros[indice->cant], registro, tamanyo);
    indice->cant++;

    return EXITO;
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

int cmp_por_dni(const void *a, const void *b)
{
    const t_reg_indice *r1 = (const t_reg_indice *)a;
    const t_reg_indice *r2 = (const t_reg_indice *)b;

    if (r1->dni < r2->dni)
        return -1;
    if (r1->dni > r2->dni)
        return 1;
    return 0;
}

int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb,
size_t tamanyo, int (*cmp)(const void *, const void *))
{
    const char *p = (const char *)indice->registros;
    size_t i;
    for (i = 0; i < nmemb; i++)
    {
        const void *elem = p + i * tamanyo;
        if (cmp(registro, elem) == 0)
            return (int)i;  // encontrado
    }
    return -1; // no encontrado
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
    free(indice->datos);
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_total = 0;
    indice->datos == NULL;
}

int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
tamanyo, int (*cmp)(const void *, const void *)) ///no se si esta bien, revisar
{
    FILE *arch = fopen(path, "rb");
    int n = 1;
    if(!arch)
    {
        printf("No se ha podido abrir el archivo...\n");
        return ERROR;
        exit(ERROR);
    }

    while(fread(vreg_ind, sizeof(tamanyo), 1, arch))
    {
        indice->datos->dni = vreg_ind->dni;
        indice->datos->nro_reg = n++;
        indice->cantidad_elementos_actual++;
    }

    ordenamientoBurbujeo(vreg_ind, n, sizeof(tamanyo), cmp)/// ???

    close(arch);

    return OK;
}
