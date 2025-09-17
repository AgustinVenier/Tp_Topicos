#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

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
        char email[30]; //si el menor es menor de edad, debera tener un email asociado. Validar formato del email.
}t_miembro;

int main()
{

    return 0;
}
