#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define dniValido(a) (1000000 < (a) && (a) < 100000000)
#define sexValido(a) ( (a) == 'F' || (a)== 'M' )//hacer un toupper previamente
#define estadoValido(a) ( (a) == 'A' || (a)== 'B' )//hacer un toupper previamente
#define planValido(a) ((strcmpi ( (a),"BASIC")==0) || (strcmpi ( (a),"PREMIUM") ==0)|| (strcmpi ( (a),"VIP")==0) || (strcmpi ( (a),"FAMILY")==0) ) // no es sensible mayus/minus
#define fNacValido(a) ( (ValidarFecha(a))  &&  (ValidarFechaNacimiento(a))) // a &fecha nac
#define fAfiliacionValido(a,b,c) ( /*validacion formal  && */ (compararFecha((a),(b)) <=0 ) && (compararFecha((a),(c)) >0 ))// a fecha afiliacion , b fecha proceso, c fecha nacimiento
#define fUltCoutaValid(a,b,c) ( (compararFecha((a),(b)) >0 ) && (compararFecha((a),(c)) <=0 ) )//a fecha ult couta b fecha afiliacion , c fecha proceso


typedef struct{
    int anio, mes, dia;
}t_fecha;

typedef struct{
        long dni; //1000000< DNI <100000000
        char *nya[61];
        t_fecha fecha_nac; //Validaci�n formal y < fecha de proceso � 10 a�os
        char sexo; //'F' | 'M'
        t_fecha fecha_afi; //Validaci�n formal, <= fecha de proceso y > fecha nacimiento
        char cat[11]; //�MENOR�, menor de 18 a�os |||||| �ADULTO�, mayor de 18 a�os.
        t_fecha fecha_cuota; //> fecha de afiliaci�n y <= fecha de proceso.
        char estado;// 'A'(ALTA) o 'B'(BAJA)
        char plan[10]; //'BASIC'-'PREMIUM'-'VIP'-'FAMILY'
        char email[30]; //si es menor de edad, debera tener un email asociado. Validar formato del email.
}t_miembro;

t_fecha fechaActual();
int ValidarFecha(const t_fecha* f);
int compararFecha(t_fecha fecha_1, t_fecha fecha_2);
void normalizar(char * cad); //normalizar cadena
int ValidarFechaNacimiento(const t_fecha *nacimiento);
#endif // FUNCTIONS_H_INCLUDED
