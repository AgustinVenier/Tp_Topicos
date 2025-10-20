#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h> // se usa???
#include <dirent.h>   // Para manejar directorios


#define EXITO 0
#define ERROR 1
#define BUFFER 250
#define FORMATO_FECHA "%d/%d/%d"
#define dniValido(a) (1000000<(a)&&(a)<100000000)
#define sexValido(a) (toupper(a)=='F'||toupper(a)=='M')
#define estadoValido(a) (toupper(a)=='A'||toupper(a)=='B')
#define planValido(a) ((strcmpi((a),"BASIC")==0)||(strcmpi((a),"PREMIUM")==0)||(strcmpi((a),"VIP")==0)||(strcmpi((a),"FAMILY")==0)) // no es sensible mayus/minus

///#define fNacValido(a) ((ValidarFecha(a))&&(ValidarFechaNacimiento(a))) // a &fecha nac
///#define fAfiliacionValido(a,b,c) ((ValidarFecha(a))&&(compararFecha((a),(b))<=0)&&(compararFecha((a),(c))>0))// a fecha afiliacion , b fecha proceso, c fecha nacimiento
///#define fUltCoutaValid(a,b,c) ((compararFecha((a),(b))>0)&&(compararFecha((a),(c))<=0))//a fecha ult couta b fecha afiliacion , c fecha proceso

///fUltCoutaValid(a,b,c) ((ValidarFecha(a))&&(compararFecha((a),(b))>0)&&(compararFecha((a),(c))<=0))  ?????

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
int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, char * nombreArchivoError,const t_fecha* f_proceso);

//Validacion General
int validaciones(t_miembro *,const t_fecha* f_proceso);

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



#endif // FUNCTIONS_H_INCLUDED
