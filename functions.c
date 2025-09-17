#include "functions.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>


void validarFecha(t_fecha f)
{
    int diasEnMes;

    if (f.mes < 1  f.mes > 12) // Validar mes
    {
        printf("Fecha invalida: mes incorrecto.\n");
        return;
    }

    // Calcular días del mes
    if (f.mes == 1  f.mes == 3  f.mes == 5  f.mes == 7  f.mes == 8  f.mes == 10  f.mes == 12)
        diasEnMes = 31;
    else
    {
      if (f.mes == 4  f.mes == 6  f.mes == 9  f.mes == 11)
        diasEnMes = 30;
      else // Febrero
      {
        if ((f.anio % 4 == 0 && f.anio % 100 != 0)  (f.anio % 400 == 0))
            diasEnMes = 29;  // Año bisiesto
        else
            diasEnMes = 28;
      }
    }

    if (f.dia < 1  f.dia > diasEnMes) // Validar día
    {
        printf("Fecha invalida: dia incorrecto.\n");
        return;
    }

    if (f.anio < 1) // Validar año
    {
        printf("Fecha invalida: año incorrecto.\n");
        return;
    }

    printf("La fecha %02d/%02d/%04d es valida.\n", f.dia, f.mes, f.anio);
}
