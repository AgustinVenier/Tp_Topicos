#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "indice.h"
#include "functions.h"

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
    indice->cantidad_elementos_maxima = nmemb * INCREMENTO; ///VA A DAR CON UN NUMERO DECIMAL
}
int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo,
                     int (*cmp)(const void *, const void *))
{
    char *base = (char*)indice->vindice;
    // Buscar si ya existe
    if(indice_buscar(indice,registro,indice->cantidad_elementos_actual,tamanyo,cmp)!=NO_EXISTE)
    {
        return ERROR;
    }
    // Verificar capacidad
    if (indice_lleno(indice) == OK)
    {
        indice_redimensionar(indice,indice->cantidad_elementos_maxima,tamanyo);
    }
   /*int i = indice->cantidad_elementos_actual - 1;
    while (i >= 0 && cmp(base + i * tamanyo, registro) > 0)
    {
        // Desplazar a la derecha
        memcpy(base + (i + 1) * tamanyo, base + i * tamanyo, tamanyo);
        i--;
    }
    memcpy(base + (i + 1) * tamanyo, registro, tamanyo);*/   //← vuela??

    memcpy(base + indice->cantidad_elementos_actual * tamanyo, registro, tamanyo);
    indice->cantidad_elementos_actual+=1;

    qsort(indice->vindice,indice->cantidad_elementos_actual,tamanyo,cmp);
    return OK;
}

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    if (!indice || !indice->vindice || indice->cantidad_elementos_actual == 0)
        return ERROR;

    int pos = indice_buscar(indice, registro, indice->cantidad_elementos_actual, tamanyo, cmp);

    if (pos == NO_EXISTE)
        return ERROR;

    size_t ult = indice->cantidad_elementos_actual - 1;

    // Si no estamos eliminando el último, desplazamos la “cola” una posición a la izquierda
    if ((size_t)pos < ult)
    {
        char *base = (char *)indice->vindice;
        memmove(base + pos * tamanyo, base + (pos + 1) * tamanyo, (ult - pos) * tamanyo);
    }

    indice->cantidad_elementos_actual--;
    return OK;
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

void indice_vaciar(t_indice *indice)
{
    if (!indice || !indice->vindice)
        return ;
    free(indice->vindice);
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = 0;
}

// suponemos que el registro de indice temporal que se carga es válido.
int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
                  tamanyo, int (*cmp)(const void *, const void *)) ///no se si esta bien, revisar
{
    int pos = 0;
    t_miembro m;
    t_reg_indice reg;

    FILE *arch = fopen(path, "rb");
    if(!arch)
    {
        return ERROR;
    }

    while(fread(&m, sizeof(t_miembro), 1, arch))
    {
        reg.dni = m.dni;
        reg.nro_reg = pos;
        if(toupper(m.estado)!='B') // valida no insertar al indice los dados de baja
        {
            indice_insertar(indice,&reg,tamanyo,cmp);
        }
        pos++;
    }
    fclose(arch);
    return OK;
}

int cmp_por_dni(const void *a, const void *b)
{
    const t_reg_indice *r1 = (const t_reg_indice *)a;
    const t_reg_indice *r2 = (const t_reg_indice *)b;

    return (r1->dni - r2->dni);
}

int busquedaBinaria(const void *vec, const void *buscado, unsigned cantelem, size_t tamanyo, int(*cmp)(const void *, const void*))
{
    int i = 0, f = cantelem - 1, medio, res;

    while(i <= f)
    {
        medio = (i + f)/2;
        res = cmp(buscado, vec + (medio * tamanyo));

        if(!res)
        {
            //Devuelve la posicion donde se encontro el elemento
            return medio;
        }
        else if(res < 0)
        {
            f = medio - 1;
        }
        else
        {
            i = medio + 1;
        }
    }
    return NO_EXISTE;
}

///FUNCION GENERICA
//int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t tamanyo,
//                  int (*cmp)(const void *, const void *))
//{
//    FILE *fp = fopen(path, "rb");
//    if(!fp)
//        return ERROR;
//
//    while(fread(vreg_ind, tamanyo, 1, fp) == 1)
//        indice_insertar(indice, vreg_ind, tamanyo, cmp);
//
//    fclose(fp);
//    return OK;
//}
