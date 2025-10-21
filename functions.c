#include "functions.h"



//Procesar archivo

int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, char * nombreArchivoError,const t_fecha* f_proceso)
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
               "%li|%s|%d/%d/%d|%c|" FORMATO_FECHA "|%s|" FORMATO_FECHA "|%c|%s|%s",
               //"%ld,\"%[^\"]\",%d/%d/%d,%c,%d/%d/%d,%[^,],%d/%d/%d,%c,%[^,],", // revisar, pero me parece que anda, con el archivo de texto que nosotros tenemos
               &miembro->dni,miembro->nya, &miembro->fecha_nac.dia, &miembro->fecha_nac.mes,
               &miembro->fecha_nac.anio, &miembro->sexo, &miembro->fecha_afi.dia, &miembro->fecha_afi.mes,
               &miembro->fecha_afi.anio, miembro->cat, &miembro->fecha_cuota.dia, &miembro->fecha_cuota.mes,
               &miembro->fecha_cuota.anio, &miembro->estado, miembro->plan, miembro->email
              );

        valor = validaciones(miembro,f_proceso);

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

//--------------------------------------------------------------------------------------
///Validacion General

int validaciones(t_miembro * miembro,const t_fecha* f_proceso )
{
    if (!dniValido(miembro->dni))
        return 1; //error en DNI

    normalizar(miembro->nya);

    if (fNacValido(&miembro->fecha_nac,f_proceso))
        return 2; //error FNAC

    if (!sexValido(miembro->sexo))
        return 3; //error SEXO

    if (fAfiliacionValido(&miembro->fecha_afi,f_proceso,&miembro->fecha_nac)==ERROR)
        return 4; // error F AFILIACION

    if (validarFechaCategoria(miembro->cat,&miembro->fecha_nac,f_proceso))
        return 5; // Error CATEGORIA, no es la indicada o vacio

    if (fUltCoutaValid(&miembro->fecha_cuota,&miembro->fecha_afi,f_proceso))
        return 6; //error F ULT COUTA VALIDA

    if (!estadoValido(miembro->estado))
        return 7; //error ESTADO

    if (!planValido(miembro->plan))
        return 8; //error PLAN

    if(!strcmpi(miembro->cat,"MENOR"))
    {
        if (validarEmail(miembro->email))
            return 9; // error MAIL
    }
    else
        strcpy(miembro->email,"");

    return EXITO ; // 0, registro VALIDO
}

//--------------------------------------------------------------------------------------

///Validaciones Campos

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


int validarFechaNacimiento(const t_fecha* nacimiento,const t_fecha * t_proceso,int cant_anios) // retorna 0 si es mayor a cant_anios, 1 si es menor, segun cuanto paso desde anios
{

    // Calcular edad
    int edad = t_proceso->anio - nacimiento->anio;

    if (t_proceso->mes < nacimiento->mes || (t_proceso->mes == nacimiento->mes && t_proceso->dia < nacimiento->dia))
        edad--;

    return (edad < cant_anios);
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
    if(strcmp(pc,"")==0)
        return ERROR;
    pc=strchr(pc,'@');

    if(pc!=NULL && isalnum(*(pc-1)) && isalpha(*(pc+1)))  /// el de adelante del @ alphanum? o solo letra?
    {
        pc=strrchr(pc,'.');
        if(pc!=NULL && isalpha(*(pc-1)) && isalpha(*(pc+1)))  /// el de adelante y atras del . alphanum? o solo letra?
            return EXITO;
    }
    return ERROR;
}


int validarFechaCategoria(char * categ,const t_fecha* fechaNac,const t_fecha * f_proceso) //sacar email
{

    if(validarFechaNacimiento(fechaNac,f_proceso,18))
    {
      if(strcmpi(categ, "MENOR") == 0)
            return EXITO;
        else
            return ERROR;
    }
    if(strcmpi(categ, "ADULTO") == 0)
        return EXITO;
    return ERROR;
}



int fNacValido(const t_fecha* fechaNac,const t_fecha * f_proceso)
{
    if (validarFecha(fechaNac) == ERROR)
        return ERROR;

    if (validarFechaNacimiento(fechaNac,f_proceso,10) == ERROR) //pasaron 10 o mas anios
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
//----------------------------------------------------------------------------------------------
//Directorios Archivos


void LeeSubCarpeta (char* subCarpeta,char* nombreArchivo)
{
    struct dirent *dir;
    DIR *d = opendir(subCarpeta);
    int flag=0;

    if (!d)
    {
        perror("opendir() error");
        *nombreArchivo=NULL;
        return ; // La subcarpeta no existe o no se puede leer
    }

    while ((dir = readdir(d)) != NULL && flag==0)
    {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
        {
            strcpy(nombreArchivo, dir->d_name);
            flag=1;

        }
        else
            *nombreArchivo='\0';
    }

    closedir(d); // Cerramos el directorio
    return ;
}

//------------------------------------------------------------------------------------------------------
//menu

char menu(const char *msj, const char *opc)
{
    char opta;
    int priVez = 1;

    do
    {
        printf("%s%s",
               priVez ? priVez = 0, "" : "ERROR - Opcion No valida\n",
               msj);
        fflush(stdin);
        scanf("%c", &opta);
    } while(strchr(opc, opta) == NULL);
    return opta;
}


