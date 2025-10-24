#include "functions.h"
//Procesar archivo
int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, char * nombreArchivoError,const t_fecha* f_proceso,t_indice * indice,int (*cmp)(const void *, const void *))
{
    char cad[BUFFER],aux[BUFFER+30];
    t_miembro m1;
    t_miembro *miembro = &m1;
    t_reg_indice auxReg;
    int valor;
    int seInserta;
    int contador=0;
    FILE* fbin;
    FILE* ftexto;
    FILE* ferror;

    ftexto = fopen(nombreArchivoTexto, "rt");
    if (ftexto == NULL)
    {
        printf("Error al abrir un archivo texto");
        return FALLA;
    }

    ferror = fopen(nombreArchivoError, "wt");
    if (ferror == NULL)
    {
        printf("Error al abrir un archivo error");
        fclose(ftexto);
        return FALLA;
    }

    fbin = fopen(nombreArchivoBin, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir un archivo binario");
        fclose(ftexto);
        fclose(ferror);
        return FALLA;
    }

    //CARGAMOS


    while(fgets(cad, sizeof(cad), ftexto))
    {
        sscanf(cad,
               "%ld,\"%[^\"]\",%d/%d/%d,%c,%d/%d/%d,%10[^,],%d/%d/%d,%c,%9[^,],%29s", // Lee archivo de texto separado por comas, y el string de nya dentro de comillas dobles.
               &miembro->dni,miembro->nya, &miembro->fecha_nac.dia, &miembro->fecha_nac.mes,
               &miembro->fecha_nac.anio, &miembro->sexo, &miembro->fecha_afi.dia, &miembro->fecha_afi.mes,
               &miembro->fecha_afi.anio, miembro->cat, &miembro->fecha_cuota.dia, &miembro->fecha_cuota.mes,
               &miembro->fecha_cuota.anio, &miembro->estado, miembro->plan, miembro->email
              );
        valor = validaciones(miembro,f_proceso);

        auxReg.dni=miembro->dni;
        auxReg.nro_reg=contador;

        if(valor == EXITO)
        {
            if(toupper(miembro->estado)=='A')
                seInserta=indice_insertar(indice,&auxReg,sizeof(t_reg_indice),cmp);
            else
                seInserta=-1;

            if(seInserta==OK)
            {
                contador++;
                fwrite(miembro, sizeof(t_miembro), 1, fbin); ///revisar
            }
            else
            {
                if(seInserta==-1)
                {
                    fwrite(miembro, sizeof(t_miembro), 1, fbin);
                    contador++;
                }

                else
                    valor=10;
            }
        }

        if(valor != EXITO)
        {
            switch (valor)
            {
                case 1:
                    strcpy(aux,"\"Error en campo DNI\",");
                    break;
                case 2:
                    strcpy(aux,"\"Error en campo F NACIMIENTO\",");
                    break;
                case 3:
                    strcpy(aux,"\"Error en campo SEXO\",");
                    break;
                case 4:
                    strcpy(aux,"\"Error en campo F AFILIACION\",");
                    break;
                case 5:
                    strcpy(aux,"\"Error en campo CATEGORIA\",");
                    break;
                case 6:
                    strcpy(aux,"\"Error en campo F ULT COUTA VALIDA\",");
                    break;
                case 7:
                    strcpy(aux,"\"Error en campo ESTADO\",");
                    break;
                case 8:
                    strcpy(aux,"\"Error en campo PLAN\",");
                    break;
                case 9:
                    strcpy(aux,"\"Error en campo MAIL\",");
                    break;
                case 10:
                    strcpy(aux,"\"Error DNI DUPLICADO\",");
                    break;
            }
            strcat(aux,cad);
            fprintf(ferror, "%s", aux);
        }
    }
    fclose(ftexto);
    fclose(fbin);
    fclose(ferror);
    return EXITO;
}

//--------------------------------------------------------------------------------------
///Validacion General

int validaciones(t_miembro * miembro,const t_fecha* f_proceso)
{
    normalizar(miembro->nya);

    if (!dniValido(miembro->dni))
        return 1; //error en DNI

    if (fNacValido(&miembro->fecha_nac,f_proceso))
        return 2; //error FNAC

    if (!sexValido(miembro->sexo))
        return 3; //error SEXO

    if (fAfiliacionValido(&miembro->fecha_afi,f_proceso,&miembro->fecha_nac)==FALLA)
        return 4; // error F AFILIACION

    if (validarFechaCategoria(miembro->cat,&miembro->fecha_nac,f_proceso))
        return 5; // Error CATEGORIA, no es la indicada o vacio

    if (fUltCoutaValido(&miembro->fecha_cuota,&miembro->fecha_afi,f_proceso))
        return 6; //error F ULT COUTA VALIDA

    if (!estadoValido(miembro->estado))
        return 7; //error ESTADO

    if (!planValido(miembro->plan))
        return 8; //error PLAN

    if(strcmp(miembro->email,"")!=0  && !strcmpi(miembro->cat,"MENOR"))
    {
        if (validarEmail(miembro->email))
            return 9; // error MAIL
    }
    else
        strcpy(miembro->email,"");

    return EXITO ;
}

//--------------------------------------------------------------------------------------
//Cargar fecha proceso
t_fecha ingresarFechaProceso()
{
    int valorFechaProc;
    t_fecha fecha;
    t_fecha *pf = &fecha;

    printf("Ingrese la fecha del proceso (DD/MM/AAAA): ");
    scanf(FORMATO_FECHA, &fecha.dia, &fecha.mes, &fecha.anio);
    valorFechaProc = validarFecha(pf);

    while (valorFechaProc == FALLA)

    {
        printf("Ingrese la fecha nuevamente (DD/MM/AAAA): ");
        fflush(stdin);
        scanf(FORMATO_FECHA, &fecha.dia, &fecha.mes, &fecha.anio);
        valorFechaProc = validarFecha(pf);

    }
    fflush(stdin);
    return fecha;
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

int validarFecha(const t_fecha* f)
{
    int diasEnMes;

    if (f->mes < 1 || f->mes > 12)
    {
        return FALLA;
    }

    if (f->mes == 1 || f->mes == 3 || f->mes == 5 || f->mes == 7 || f->mes == 8 || f->mes == 10 || f->mes == 12)
        diasEnMes = 31;
    else
    {
        if (f->mes == 4 || f->mes == 6 || f->mes == 9 || f->mes == 11)
            diasEnMes = 30;
        else // Febrero
        {
            if ((f->anio % 4 == 0 && f->anio % 100 != 0) || (f->anio % 400 == 0))
                diasEnMes = 29;  // Anio bisiesto
            else
                diasEnMes = 28;
        }
    }

    if (f->dia < 1 || f->dia > diasEnMes)
    {
        return FALLA;
    }

    if (f->anio < 1)
    {
        return FALLA;
    }
    return EXITO;
}

int validarFechaNacimiento(const t_fecha* nacimiento,const t_fecha * t_proceso,int cant_anios) // retorna 0 si es mayor a cant_anios, 1 si es menor, segun cuanto paso desde nacimiento hasta proceso
{
    // Calcula edad
    int edad = t_proceso->anio - nacimiento->anio;

    if (t_proceso->mes < nacimiento->mes || (t_proceso->mes == nacimiento->mes && t_proceso->dia < nacimiento->dia))
        edad--;

    return (edad < cant_anios);
}

void normalizar(char * cad)
{
    char temp[61];
    char *lect = cad;
    char *esc = temp;
    int primeraLetraPalabra,primeraPalabra=0;

    while (*lect)
    {
        while (*lect && (isspace(*lect)|| *lect ==','))
            lect++;
        if (*lect)
        {
            primeraLetraPalabra = FALLA;
            while (*lect && !isspace(*lect) && *lect!=',')
            {
                *esc = primeraLetraPalabra ? toupper(*lect) : tolower(*lect);
                primeraLetraPalabra = EXITO;
                esc++;
                lect++;
            }
            if(!primeraPalabra)
            {
                *esc++ = ',';
                primeraPalabra=FALLA;
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
        return FALLA;
    pc=strchr(pc,'@');

    if(pc!=NULL && isalnum(*(pc-1)) && isalpha(*(pc+1)))
    {
        pc=strrchr(pc,'.');
        if(pc!=NULL && isalpha(*(pc-1)) && isalpha(*(pc+1)))
            return EXITO;
    }
    return FALLA;
}

int validarFechaCategoria(char * categ,const t_fecha* fechaNac,const t_fecha * f_proceso)
{
    if(validarFechaNacimiento(fechaNac,f_proceso,18))
    {
        if(strcmpi(categ, "MENOR") == EXITO)
            return EXITO;
        else
            return FALLA;
    }
    if(strcmpi(categ, "ADULTO") == EXITO)
        return EXITO;
    return FALLA;
}

int fNacValido(const t_fecha* fechaNac,const t_fecha * f_proceso)
{
    if (validarFecha(fechaNac) == FALLA)
        return FALLA;

    if (validarFechaNacimiento(fechaNac,f_proceso,ANIOS_DESDE_NACIMIENTO) == FALLA)
        return FALLA;

    return EXITO;
}

int fAfiliacionValido(const t_fecha* fechaAfi, const t_fecha* fechaProc, const t_fecha* fechaNac)
{
    if (validarFecha(fechaAfi) == FALLA)
        return FALLA;

    if (compararFecha(fechaAfi, fechaProc) > 0) // afiliación > proceso // creo que el problema esta en esta linea
        return FALLA;

    if (compararFecha(fechaAfi, fechaNac) <= 0) // afiliación <= nacimiento
        return FALLA;

    return EXITO;
}

int fUltCoutaValido(const t_fecha* fechaCuota, const t_fecha* fechaAfi, const t_fecha* fechaProc)
{
    if (compararFecha(fechaCuota, fechaAfi) <= EXITO) // cuota <= afiliación
        return FALLA;

    if (compararFecha(fechaCuota, fechaProc) > EXITO) // cuota > proceso
        return FALLA;

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
        *nombreArchivo=' ';
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
            *nombreArchivo=' ';
    }
    closedir(d); // Cerramos el directorio
    return ;
}

int crearNombreArchivo(char *nombreArchivoBinario,char *nombreArchivoError,const char *subcarpeta_binario,const char *subcarpeta_error,const t_fecha *pf)
{
    char aux_fecha[10], aux_nombre[61];

    if (*nombreArchivoBinario == ' ')
    {
        printf("No hay archivos para recuperar, se generaran nuevos.\n");

        strcpy(nombreArchivoBinario,subcarpeta_binario);
        strcat(nombreArchivoBinario, "/miembros-VC-");
        sprintf(aux_fecha,"%04d%02d%02d",pf->anio,pf->mes,pf->dia);
        strcat(nombreArchivoBinario,aux_fecha);
        strcat(nombreArchivoBinario,".dat");

        strcpy(nombreArchivoError,subcarpeta_error);
        strcat(nombreArchivoError, "/error-VC-");
        strcat(nombreArchivoError,aux_fecha);
        strcat(nombreArchivoError,".txt");

        return FALLA; // Procesar
    }
    else
    {
        strcpy(aux_nombre,subcarpeta_binario);
        strcat(aux_nombre,"/");
        strcat(aux_nombre,nombreArchivoBinario);
        strcpy(nombreArchivoBinario,subcarpeta_binario);
        strcat(nombreArchivoBinario, "/miembros-VC-");
        sprintf(aux_fecha,"%04d%02d%02d",pf->anio,pf->mes,pf->dia);
        strcat(nombreArchivoBinario,aux_fecha);
        strcat(nombreArchivoBinario,".dat");

        printf("Recuperando desde: %s\nLuego de finalizar se guardara con el nombre: %s\n",nombreArchivoBinario,aux_nombre);
        //printf("\n- Nombre viejo:%s \n- Nombre nuevo: %s\n\n",aux_nombre);
        if ((rename(aux_nombre,nombreArchivoBinario)) == 0)
        {
            printf("Archivo renombrado exitosamente!\n\n");
        }
        else    // Si falla, imprime el error del sistema
        {
            perror("Error al renombrar el archivo");
        }
        return EXITO; // Recuperar
    }
}

void mostrarMiembros(const char *nombreArch)
{
    FILE *pf;
    t_miembro miembro;

    pf = fopen(nombreArch, "rb");
    if (!pf)
    {
        printf("No se pudo abrir el archivo binario: %s\n", nombreArch);
        return;
    }

    printf("\n--- Miembros en el archivo.dat ---\n");
    while (fread(&miembro, sizeof(t_miembro), 1, pf) == 1)
    {
        printf("  DNI: %ld  ", miembro.dni);
        printf("  Nombre y Apellido: %s", miembro.nya);
        printf("  Fecha de Nacimiento: %02d/%02d/%04d", miembro.fecha_nac.dia, miembro.fecha_nac.mes, miembro.fecha_nac.anio);
        printf("  Sexo: %c", miembro.sexo);
        printf("  Fecha de Afiliacion: %02d/%02d/%04d", miembro.fecha_afi.dia, miembro.fecha_afi.mes, miembro.fecha_afi.anio);
        printf("  Categoria: %s\n", miembro.cat);
        printf("  Fecha de ultima Cuota: %02d/%02d/%04d\n", miembro.fecha_cuota.dia, miembro.fecha_cuota.mes, miembro.fecha_cuota.anio);
        printf("  Estado: %c", miembro.estado);
        printf("  Plan: %s", miembro.plan);
        printf("  Email: %s\n", miembro.email);
        printf("-------------------------------\n");
    }
    fclose(pf);
}
