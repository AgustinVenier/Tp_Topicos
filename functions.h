#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>   // Para manejar directorios
#include "indice.h"


#define EXITO 0
#define FALLA 1   //este se cambia
#define BUFFER 250
#define FORMATO_FECHA "%d/%d/%d"
#define dniValido(a) (1000000<(a)&&(a)<100000000)
#define sexValido(a) (toupper(a)=='F'||toupper(a)=='M')
#define estadoValido(a) (toupper(a)=='A'||toupper(a)=='B')
#define planValido(a) ((strcmpi((a),"BASIC")==0)||(strcmpi((a),"PREMIUM")==0)||(strcmpi((a),"VIP")==0)||(strcmpi((a),"FAMILY")==0)) // no es sensible mayus/minus


typedef struct
{
    int anio, mes, dia;
} t_fecha;

typedef struct
{
    long dni; //1000000< DNI <100000000
    char nya[61];
    t_fecha fecha_nac; //Validaci�n formal y < fecha de proceso � 10 a�os
    char sexo; //'F' | 'M'
    t_fecha fecha_afi; //Validaci�n formal, <= fecha de proceso y > fecha nacimiento
    char cat[11]; //�MENOR�, menor de 18 a�os |||||| �ADULTO�, mayor de 18 a�os.
    t_fecha fecha_cuota; //> fecha de afiliaci�n y <= fecha de proceso.
    char estado;// 'A'(ALTA) o 'B'(BAJA)
    char plan[10]; //'BASIC'-'PREMIUM'-'VIP'-'FAMILY'
    char email[30]; //si es menor de edad, debera tener un email asociado. Validar formato del email.
} t_miembro;

//Procesar Archivo
int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, char * nombreArchivoError,const t_fecha* f_proceso,t_indice * indice,int (*cmp)(const void *, const void *));

//Validacion General
int validaciones(t_miembro *, const t_fecha* f_proceso);

//Validaciones Campos

int validarFecha(const t_fecha* f);
int compararFecha(const t_fecha* fecha_1,const t_fecha* fecha_2);
void normalizar(char * cad); //normalizar cadena
int validarFechaNacimiento(const t_fecha* nacimiento,const t_fecha * t_proceso,int cant_anios);
int validarEmail(char *cad);
int validarFechaCategoria(char * categ,const t_fecha* fechaNac,const t_fecha * f_proceso);
int fNacValido(const t_fecha* fechaNac,const t_fecha * f_proceso);
int fAfiliacionValido(const t_fecha* fechaAfi, const t_fecha* fechaProc, const t_fecha* fechaNac);
int fUltCoutaValid(const t_fecha* fechaCuota, const t_fecha* fechaAfi, const t_fecha* fechaProc);

//Directorios Archivos
void LeeSubCarpeta (char* subCarpeta,char* nombreArchivo); //devuelve el nombre de  un archivo de esa subcarpeta
int crearNombreArchivo(char *nombreArchivoBinario,char *nombreArchivoError,const char *subcarpeta_binario,const char *subcarpeta_error,const t_fecha *pf);

void mostrarMiembros(const char *nombreArch);

//Parte 2
void menuMiembros(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
char menu(const char *msj, const char *opc);
void preguntarCambio(const char * cad, char* aux);
void eliminarFinDeLinea(char *cad);
int Alta(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
int Baja(const char *nombreArch, t_indice *ind);
int Modificacion(const char *nombreArch, t_indice *ind, const t_fecha *fecha);

int MostrarInfoMiembro(const char *nombreArch, t_indice *ind);
int ListadoXDNI(const char *nombreArch, t_indice *ind);
int ListadoXPlan(const char *nombreArch, t_indice *ind);

#endif // FUNCTIONS_H_INCLUDED
