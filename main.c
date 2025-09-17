#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

typedef struct{
    int anio, mes, dia;
}t_fecha;

typedef struct{
        long dni; //1000000< DNI <100000000
        char *nya[61];
        t_fecha fecha_nac; //Validación formal y < fecha de proceso – 10 años
        char sexo; //'F' | 'M'
        t_fecha fecha_afi; //Validación formal, <= fecha de proceso y > fecha nacimiento
        char cat[11]; //‘MENOR’, menor de 18 años |||||| ‘ADULTO’, mayor de 18 años.
        t_fecha fecha_cuota; //> fecha de afiliación y <= fecha de proceso.
        char estado;// 'A'(ALTA) o 'B'(BAJA)
        char plan[10]; //'BASIC'-'PREMIUM'-'VIP'-'FAMILY'
        char email[30]; //si el menor es menor de edad, debera tener un email asociado. Validar formato del email.
}t_miembro;

int main()
{

    return 0;
}
