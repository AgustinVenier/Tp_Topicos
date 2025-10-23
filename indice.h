#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#define CANT_ELEMENTOS 100
#define INCREMENTO 1.3
#define OK 1
#define ERROR 0
#define NO_EXISTE -1

typedef struct
{
    unsigned nro_reg;
    long dni;
}t_reg_indice;

typedef struct
{
    void *vindice;
    unsigned cantidad_elementos_actual;
    unsigned cantidad_elementos_maxima;
}t_indice;

void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo);

void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo);

int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *));

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int
(*cmp)(const void *, const void *));

int cmp_por_dni(const void *a, const void *b);

int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb,
size_t tamanyo, int (*cmp)(const void *, const void *));

int indice_lleno(const t_indice *indice);

int indice_vacio(const t_indice *indice);

int indice_lleno(const t_indice *indice);

void indice_vaciar(t_indice* indice);

int indice_cargar(const char* path, t_indice* indice, void *vreg_ind, size_t
tamanyo, int (*cmp)(const void *, const void *));

//eliminar si no se utilizan

int busquedaBinaria(void *vec, void *buscado, unsigned cantelem, size_t tamanyo, int(*cmp)(const void *, const void*));

void ordenamientoBurbujeo(void *vec, unsigned nmemb, size_t tamanyo, int (*cmp)(const void *, const void *));

void intercambio(void *a, void *b, size_t tamanyo);

#endif // INDICE_H_INCLUDED
