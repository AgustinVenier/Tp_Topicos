#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>   // Para manejar directorios
#include <unistd.h> // Para sleep
#include "indice.h"

#define BUFFER 250
#define ANIOS_DESDE_NACIMIENTO 10
#define dniValido(a) (1000000<(a)&&(a)<100000000)
#define sexValido(a) (toupper(a)=='F'||toupper(a)=='M')
#define estadoValido(a) (toupper(a)=='A'||toupper(a)=='B')
#define planValido(a) ((strcmpi((a),"BASIC")==0)||(strcmpi((a),"PREMIUM")==0)||(strcmpi((a),"VIP")==0)||(strcmpi((a),"FAMILY")==0))


typedef struct
{
    int anio, mes, dia;
} t_fecha;

typedef struct
{
    long dni;
    char nya[60];
    t_fecha fecha_nac;
    char sexo;
    t_fecha fecha_afi;
    char cat[10];
    t_fecha fecha_cuota;
    char estado;
    char plan[10];
    char email[30];
} t_miembro;

//Procesar Archivo Texto
int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, char * nombreArchivoError,const t_fecha* f_proceso,t_indice * indice,int (*cmp)(const void *, const void *));

//Validacion General
int validaciones(t_miembro *, const t_fecha* f_proceso);

//Cargar fecha proceso
t_fecha ingresarFechaProceso();

//Validaciones Campos
int validarFecha(const t_fecha* f);
int compararFecha(const t_fecha* fecha_1,const t_fecha* fecha_2);
void normalizar(char * cad);
int validarFechaNacimiento(const t_fecha* nacimiento,const t_fecha * t_proceso,int cant_anios);
int validarEmail(char *cad);
int validarFechaCategoria(char * categ,const t_fecha* fechaNac,const t_fecha * f_proceso);
int fNacValido(const t_fecha* fechaNac,const t_fecha * f_proceso);
int fAfiliacionValido(const t_fecha* fechaAfi, const t_fecha* fechaProc, const t_fecha* fechaNac);
int fUltCoutaValido(const t_fecha* fechaCuota, const t_fecha* fechaAfi, const t_fecha* fechaProc);

//Directorios Archivos
void LeeSubCarpeta (char* subCarpeta,char* nombreArchivo); //devuelve el nombre de  un archivo de esa subcarpeta
int crearNombreArchivo(char *nombreArchivoBinario,char *nombreArchivoError,const char *subcarpeta_binario,const char *subcarpeta_error,const t_fecha *pf); //crea los nombres de los archivos segun fecha proceso

//Funciones Segunda Parte
void menuMiembros(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
char mostrarMenu(const char *msj, const char *opc);
void preguntarCambio(const char * cad, char* aux);
void eliminarFinDeLinea(char *cad);
int Alta(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
int Baja(const char *nombreArch, t_indice *ind);
int Modificacion(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
int MostrarInfoMiembro(const char *nombreArch, t_indice *ind);
int ListadoXDNI(const char *nombreArch, t_indice *ind);
int ListadoXPlan(const char *nombreArch, t_indice *ind);

#endif // FUNCTIONS_H_INCLUDED
