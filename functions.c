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

int compararFecha(const t_fecha* fecha_1, const t_fecha* fecha_2)   // devuelve 0 si son iguales, >0 si fecha 1 es mayor, <0 si fecha 1 es menor
{
    if (fecha_1->anio != fecha_2->anio)
        return fecha_1->anio - fecha_2->anio;
    if (fecha_1->mes != fecha_2->mes)
        return fecha_1->mes - fecha_2->mes;
    return fecha_1->dia - fecha_2->dia;
}

int validarFecha(const t_fecha* f) // retorna 0 valido, 1 invalido
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


int validarFechaNacimiento(const t_fecha* nacimiento) // retorna 0 valido, 1 invalido
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

    return (edad < 10);
}



void normalizar(char * cad)
{
    char temp[61]; // funciona para cadenas de 60 carac
    char *lect = cad;
    char *esc = temp;


    int primeraLetraPalabra,primeraPalabra=0;

    while (*lect)
    {
        while (*lect && (isspace(*lect)|| *lect ==','))
            lect++;

        if (*lect)
        {
            primeraLetraPalabra = 1;

            while (*lect && !isspace(*lect) && *lect!=',')
            {

                *esc = primeraLetraPalabra ? toupper(*lect) : tolower(*lect);
                primeraLetraPalabra = 0;
                esc++;
                lect++;

            }
            if(!primeraPalabra)
            {
                *esc++ = ',';
                 primeraPalabra=1;
            }

            *esc++ = ' ';

        }
    }
    *(esc-1) = '\0';
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

    if (ftexto ==NULL)
    {
        printf("Error al abrir un archivo texto");
        return ERROR;
    }
    if (ferror ==NULL)
    {
        printf("Error al abrir un archivo error");
        return ERROR;
    }
    if (fbin ==NULL)
    {
        printf("Error al abrir un archivo binario");
        return ERROR;
    }

    while(fgets(cad, sizeof(cad), ftexto))
    {
        sscanf(cad, /// revisar
          /// no lee correctamente
          //"%ld|%s|" FORMATO_FECHA "|%c|" FORMATO_FECHA "|%s|" FORMATO_FECHA "|%c|%s|%s",
          "%ld,\"%[^\"]\",%d/%d/%d,%c,%d/%d/%d,%[^,],%d/%d/%d,%c,%[^,],", // revisar, pero me parece que anda, con el archivo de texto que nosotros tenemos
            &miembro->dni,miembro->nya, &miembro->fecha_nac.dia, &miembro->fecha_nac.mes,
            &miembro->fecha_nac.anio, &miembro->sexo, &miembro->fecha_afi.dia, &miembro->fecha_afi.mes,
            &miembro->fecha_afi.anio, miembro->cat, &miembro->fecha_cuota.dia, &miembro->fecha_cuota.mes,
            &miembro->fecha_cuota.anio, &miembro->estado, miembro->plan, miembro->email
            );

        //valor = validaciones();
        valor =0;

        if(valor == EXITO)
        {
            fwrite(miembro, sizeof(t_miembro), 1, fbin); ///revisar
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
int fNacValido(const t_fecha* fechaNac)
{
    if (validarFecha(fechaNac) == ERROR)
        return ERROR;

    if (validarFechaNacimiento(fechaNac) == ERROR)
        return ERROR;

    return EXITO;
}

int fAfiliacionValido(const t_fecha* fechaAfi, const t_fecha* fechaProc, const t_fecha* fechaNac)
{
    if (validarFecha(fechaAfi) == ERROR)
        return ERROR;

   if (compararFecha(fechaAfi, fechaProc) > 0) // afiliación > proceso // creo que el problema esta en esta linea
       return ERROR;

    if (compararFecha(fechaAfi, fechaNac) <= 0) // afiliación <= nacimiento
        return ERROR;

    return EXITO;
}

int fUltCoutaValid(const t_fecha* fechaCuota, const t_fecha* fechaAfi, const t_fecha* fechaProc)
{
    if (compararFecha(fechaCuota, fechaAfi) <= 0) // cuota <= afiliación
        return ERROR;

    if (compararFecha(fechaCuota, fechaProc) > 0) // cuota > proceso
        return ERROR;

    return EXITO;
}

int validaciones(t_miembro * miembro,const t_fecha* f_proceso )
{
    if (!dniValido(miembro->dni))
        return 1; //error en campo 1

    normalizar(miembro->nya);

    if (fNacValido(&miembro->fecha_nac))
        return 2; //error campo 2

    if (!sexValido(miembro->sexo))
        return 3; //error campo 3

    if (fAfiliacionValido(&miembro->fecha_afi,f_proceso,&miembro->fecha_nac)==ERROR)
        return 4; // error campo 4

    ///if ( // Implementar Validacion Categoria
    ///);

    if (fUltCoutaValid(&miembro->fecha_cuota,&miembro->fecha_afi,f_proceso))
        return 6; //error campo 6

    if (!estadoValido(miembro->estado))
        return 7; //error campo 7

    if (!planValido(miembro->plan))
        return 8; //error campo 8

    if (validarEmail(miembro->email))
        return 9; // error campo 9

    return EXITO ; // 0, registro válido
}
