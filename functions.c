#include "functions.h"

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

int ValidarFecha(const t_fecha* f) // 0 invalida, 1 válida
{
    int diasEnMes;

    if (f->mes < 1 || f->mes > 12) // Validar mes
    {
        return 0;
    }

    // Calcular d�as del mes
    if (f->mes == 1 || f->mes == 3 || f->mes == 5 || f->mes == 7 || f->mes == 8 || f->mes == 10 || f->mes == 12)
        diasEnMes = 31;
    else
    {
        if (f->mes == 4 || f->mes == 6 || f->mes == 9 || f->mes == 11)
            diasEnMes = 30;
        else // Febrero
        {
            if ((f->anio % 4 == 0 && f->anio % 100 != 0) || (f->anio % 400 == 0))
                diasEnMes = 29;  // A�o bisiesto
            else
                diasEnMes = 28;
        }
    }

    if (f->dia < 1 || f->dia > diasEnMes) // Validar d�a
    {
        return 0;
    }

    if (f->anio < 1) // Validar año
    {
        return 0;
    }
    return 1;
}


int ValidarFechaNacimiento(const t_fecha* nacimiento)
{
    time_t t;
    struct tm *fecha;

    // Obtener tiempo actual
    time(&t);

    // Convertir a fecha local
    fecha = localtime(&t);

    // Fecha del sistema
    int diaHoy  = fecha->tm_mday;
    int mesHoy  = fecha->tm_mon + 1;
    int anioHoy = fecha->tm_year + 1900;

    printf("Hoy es: %02d/%02d/%d\n", diaHoy, mesHoy, anioHoy);

    // Calcular edad
    int edad = anioHoy - nacimiento->anio;

    if (mesHoy < nacimiento->mes || (mesHoy == nacimiento->mes && diaHoy < nacimiento->dia))
        edad--;

    return (edad >= 10);
}



void normalizar(char * cad)
{
    char temp[61]; // funciona para cadenas de 60 carac
    char *lect = cad;
    char *esc = temp;


    int primeraLetraPalabra;

    while (*lect)
    {
        while (*lect && isspace(*lect))
            lect++;

        if (*lect)
        {
            primeraLetraPalabra = 1;

            while (*lect && !isspace(*lect))
            {
                *esc = primeraLetraPalabra ? toupper(*lect) : tolower(*lect);
                primeraLetraPalabra = 0;
                esc++;
                lect++;
            }
                *esc++ = ',';
                *esc++ = ' ';

        }
    }
    *(esc-2) = '\0';
    strcpy(cad, temp);
}
