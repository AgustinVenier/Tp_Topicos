#include "functions.h"

t_fecha fechaActual()
{
    time_t t = time(NULL);
    struct tm *hoy = localtime(&t);

    t_fecha f;
    f.anio = hoy->tm_year + 1900;  // tm_year cuenta desde 1900
    f.mes  = hoy->tm_mon + 1;      // tm_mon va de 0 a 11
    f.dia  = hoy->tm_mday;         // tm_mday va de 1 a 31
    return f;
}

int compararFecha(t_fecha fecha_1, t_fecha fecha_2)   // devuelve 0 si son iguales, >0 si fecha 1 es mayor, <0 si fecha 1 es menor
{
    if (fecha_1.anio != fecha_2.anio)
        return fecha_1.anio - fecha_2.anio;
    if (fecha_1.mes != fecha_2.mes)
        return fecha_1.mes - fecha_2.mes;
    return fecha_1.dia - fecha_2.dia;
}

int validarFecha(const t_fecha* f)
{
    int diasEnMes;

    if (f->mes < 1 || f->mes > 12) // Validar mes
    {
        return ERROR;
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
        return ERROR;
    }

    if (f->anio < 1) // Validar año
    {
        return ERROR;
    }
    return EXITO;
}


int validarFechaNacimiento(const t_fecha* nacimiento)
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


int validarEmail(char *cad)
{
    char* pc=cad;
    if(strcmp(pc,"")==0){
        return ERROR;
    }


    pc=strchr(pc,'@');


    if(pc!=NULL && isalnum(*(pc-1)) && isalpha(*(pc+1)))  /// el de adelante del @ alphanum? o solo letra?
    {
        pc=strrchr(pc,'.');
        if(pc!=NULL && isalpha(*(pc-1)) && isalpha(*(pc+1)))  /// el de adelante y atras del . alphanum? o solo letra?
        {
            return EXITO;
        }
    }

    return ERROR;
}


int validarFechaCategoria(char * categ, char * email) ///validamos la edad?
{
    if(strcmpi(categ, "MENOR") == 0)
    {
        if(validarEmail(email)==EXITO)
            return EXITO;
    }
    else if(strcmpi(categ, "MAYOR") == 0)
        return EXITO;
    return ERROR;
}

int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, char * nombreArchivoError)
{
    char cad[BUFFER];
    t_miembro m1;
    t_miembro *miembro = &m1;
    int valor;
    //int cantMiembros = 0;
    FILE* ftexto = fopen(nombreArchivoTexto, "rt");
    FILE* ferror = fopen(nombreArchivoError, "wt");
    FILE* fbin = fopen(nombreArchivoBin, "wb");

    if (!ftexto || !ferror || !fbin)
    {
        printf("Error al abrir un archivo.");
        return ERROR;
    }

    while(fgets(cad, sizeof(cad), ftexto))
    {
        sscanf(nombreArchivoTexto,
           "%ld|%s|" FORMATO_FECHA "|%c|" FORMATO_FECHA "|%s|" FORMATO_FECHA "|%c|%s|%s",
            &miembro->dni, miembro->nya, &miembro->fecha_nac.dia, &miembro->fecha_nac.mes,
            &miembro->fecha_nac.anio, &miembro->sexo, &miembro->fecha_afi.dia, &miembro->fecha_afi.mes,
            &miembro->fecha_afi.anio, miembro->cat, &miembro->fecha_cuota.dia, &miembro->fecha_cuota.mes,
            &miembro->fecha_cuota.anio, &miembro->estado, miembro->plan, miembro->email
            );

        //valor = validaciones();

        if(valor == EXITO)
        {
            fwrite(&miembro, sizeof(t_miembro), 1, fbin);
            //cantMiembros++;
        }
        else if(valor == ERROR)
        {
            fprintf(ferror, "%s", cad);
        }
    }

    fclose(ftexto);
    fclose(fbin);
    fclose(ferror);
    //es necesario retornar la cantidad de miembros?
    return EXITO;
}

//suma los valores de todas las validaciones si retorna exito pasa a binario y
//si retorna error crea un archivo de texto error
//int validaciones()
//{
    //implementar
//}
