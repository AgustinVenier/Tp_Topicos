#include "functions.h"


void validarFecha(t_fecha f)
{
    int diasEnMes;

    if (f.mes < 1 || f.mes > 12) // Validar mes
    {
        printf("Fecha invalida: mes incorrecto.\n");
        return;
    }

    // Calcular días del mes
    if (f.mes == 1 || f.mes == 3 || f.mes == 5 || f.mes == 7 || f.mes == 8 || f.mes == 10 || f.mes == 12)
        diasEnMes = 31;
    else
    {
        if (f.mes == 4 || f.mes == 6 || f.mes == 9 || f.mes == 11)
            diasEnMes = 30;
        else // Febrero
        {
            if ((f.anio % 4 == 0 && f.anio % 100 != 0) || (f.anio % 400 == 0))
                diasEnMes = 29;  // Año bisiesto
            else
                diasEnMes = 28;
        }
    }

    if (f.dia < 1 || f.dia > diasEnMes) // Validar día
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



t_fecha fechaActual() {
    time_t t = time(NULL);
    struct tm *hoy = localtime(&t);

    t_fecha f;
    f.anio = hoy->tm_year + 1900;  // tm_year cuenta desde 1900
    f.mes  = hoy->tm_mon + 1;      // tm_mon va de 0 a 11
    f.dia  = hoy->tm_mday;         // tm_mday va de 1 a 31
    return f;
}

int compararFecha(t_fecha fecha_1, t_fecha fecha_2) { // devuelve 0 si son iguales, >0 si fecha 1 es mayor, <0 si fecha 1 es menor
    if (fecha_1.anio != fecha_2.anio)
        return fecha_1.anio - fecha_2.anio;
    if (fecha_1.mes != fecha_2.mes)
        return fecha_1.mes - fecha_2.mes;
    return fecha_1.dia - fecha_2.dia;
}
