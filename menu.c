#include "functions.h"
#include "indice.h"
#include <stdlib.h>

void menuMiembros(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
{
    char op;
    int resultado; // Para guardar el valor devuelto por cada operación
    do{
    indice_mostrar(ind);
    printf("\n");
    op = menu(
             "a. Alta\n"
             "b. Baja\n"
             "c. Modificacion\n"
             "d. Mostrar informacion de un miembro\n"
             "e. Listado de miembros ordenados por DNI\n"
             "f. Listado de todos los miembros agrupados por plan\n"
             "g. Salir\n",
             "abcdefg"
             );
            switch(op)
            {
                case 'a':
                    resultado = Alta(nombreArch,ind, fecha);
                    if (resultado == ERROR)
                        printf("Error al dar de alta el miembro.\n");
                    else
                        printf("Alta realizada con exito.\n");
                    break;

                case 'b':
                    resultado = Baja(nombreArch, ind);
                    if (resultado == ERROR)
                        printf("Error al dar la baja del miembro.\n");
                    else
                        printf("Baja realizada con exito.\n");
                    break;

                case 'c':
                    resultado = Modificacion(nombreArch, ind, fecha);
                    if (resultado == ERROR)
                        printf("Error al modificar el miembro.\n");
                    else
                        printf("Modificacion realizada con exito.\n");
                    break;

                case 'd':
                    resultado = MostrarInfoMiembro(nombreArch,ind);
                    if (resultado == ERROR)
                        printf("Error al modificar el miembro.\n");
                    else
                        printf("Modificacion realizada con exito.\n");
                    break;

                case 'e':
                    resultado = ListadoXDNI(nombreArch, ind);
                    if (resultado == ERROR)
                        printf("Error al modificar el miembro.\n");
                    else
                        printf("Modificacion realizada con exito.\n");
                    break;

                case 'f':
                    resultado = ListadoXPlan(nombreArch, ind);
                    if (resultado == ERROR)
                        printf("Error al modificar el miembro.\n");
                    else
                        printf("Modificacion realizada con exito.\n");
                    break;
            }
        }while(op != 'g');
}

void eliminarFinDeLinea(char *cad)
{
    char *p = strchr(cad, '\n');
    if (p)
        *p = '\0';
}

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
    } while(strchr(opc, opta) == NULL); //busca el carácter dentro del conjunto de válidos
    return opta;
}

int Alta(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
{
    t_miembro m;    //nuevo registro completo que se va a dar de alta
    t_reg_indice reg; // Estructura auxiliar: Se usa tanto para buscar como para insertar en el índice
    int pos;

    FILE *pf = fopen(nombreArch, "r+b");
    if (!pf)
    {
        // si no existe, crear nuevo archivo
        pf = fopen(nombreArch, "w+b");
        if (!pf)
        {
            printf("No se pudo abrir el archivo");
            return ERROR;
        }
    }
    // Calculo la posición física en el archivo
    fseek(pf, 0, SEEK_END);
    unsigned nro_reg = ftell(pf) / sizeof(t_miembro);

    // Ingreso datos del nuevo miembro
    printf("\n\nIngrese DNI: ");
    scanf("%ld", &m.dni);
    reg.dni = m.dni;

    // Valido si ya existe en el índice
    pos = indice_buscar(ind, &reg, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if (pos != NO_EXISTE)
    {
        printf("Error: ya existe un miembro con DNI %ld.\n", m.dni);
        fclose(pf);
        return ERROR;
    }
    //Ingreso del resto de los datos
    printf("Ingrese Nombre y Apellido: ");
    fflush(stdin);
    fgets(m.nya, sizeof(m.nya), stdin);
    eliminarFinDeLinea(m.nya);

    printf("Ingrese sexo (M/F): ");
    scanf(" %c", &m.sexo);

    printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio);

    printf("Ingrese fecha de afiliacion (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &m.fecha_afi.dia, &m.fecha_afi.mes, &m.fecha_afi.anio);

    printf("Ingrese categoria (MENOR/ADULTO): ");
    fflush(stdin);
    fgets(m.cat, sizeof(m.cat), stdin);
    eliminarFinDeLinea(m.cat);

    printf("Ingrese fecha de ultima cuota paga (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &m.fecha_cuota.dia, &m.fecha_cuota.mes, &m.fecha_cuota.anio);

    printf("Ingrese estado (A/B): ");
    scanf(" %c", &m.estado);

    printf("Ingrese plan (BASIC/PREMIUM/VIP/FAMILY): ");
    fflush(stdin);
    fgets(m.plan, sizeof(m.plan), stdin);
    eliminarFinDeLinea(m.plan);

    printf("Ingrese email: ");
    fflush(stdin);
    fgets(m.email, sizeof(m.email), stdin);
    eliminarFinDeLinea(m.email);

    // Valido los campos del miembro
    normalizar(m.nya);
    if (validaciones(&m, fecha) != EXITO)
    {
        printf("Registro inválido según validaciones. Alta cancelada.\n");
        fclose(pf);
        return ERROR;
    }

    // Escribir al final:
    fseek(pf, 0, SEEK_END);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fflush(pf);

    // Insertar en el  índice:
    reg.dni = m.dni;
    reg.nro_reg = nro_reg;

    if(indice_insertar(ind, &reg, sizeof(t_reg_indice), cmp_por_dni) != OK)
        return ERROR;
    else
        printf("Alta realizada. DNI=%ld, nro_reg=%u\n", reg.dni, reg.nro_reg);
    fclose(pf);
    return OK;
}

int Baja(const char *nombreArch, t_indice *ind)
{
    long dni;
    int pos;
    t_miembro m;
    t_reg_indice clave; //Estructura auxiliar para verificar

    // Solicito DNI y lo asigno a la clave
    printf("DNI a dar de baja: ");
    scanf("%ld", &m.dni);
    clave.dni = m.dni;
    // Busco posición en el índice
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if (pos == -1)
    {
        printf("No existe miembro con DNI %ld\n", dni);
        return ERROR;
    }
    // Accede al vector de índices para obtener el número de registro físico
    t_reg_indice *vec = (t_reg_indice*)ind->vindice;
    unsigned nro_reg = vec[pos].nro_reg;

    FILE *pf = fopen(nombreArch, "r+b");
    if (!pf)
    {
        printf("No se pudo abrir el archivo");
        return ERROR;
    }
    // Leo el miembro desde el archivo
    fseek(pf, nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    if (m.estado == 'B')
    {
        printf("El registro ya está dado de baja.\n");
        fclose(pf);
        indice_eliminar(ind, &clave, sizeof(t_reg_indice), cmp_por_dni);
        return OK;
    }

    m.estado = 'B';
    fseek(pf, nro_reg * sizeof(t_miembro), SEEK_SET);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fflush(pf);

    indice_eliminar(ind, &clave, sizeof(t_reg_indice), cmp_por_dni);
    fclose(pf);
    return OK;
}


int Modificacion(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
{
    t_miembro m;
    t_reg_indice clave;
    char aux;

    printf("\n=== MODIFICACION DE MIEMBRO ===\n");
    printf("Ingrese DNI a modificar: ");
    scanf("%ld", &m.dni);
    getchar();
    clave.dni = m.dni;

    int pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if(pos == -1)
    {
        printf("No se encontro el DNI.\n");
        return ERROR;
    }

    FILE *pf = fopen(nombreArch, "r+b");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }
    t_reg_indice *vec = (t_reg_indice *)ind->vindice; //casteo

    // Posiciona el puntero en el registro correspondiente y lo lee
    fseek(pf, vec[pos].nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    printf("Modificando a: %s\n", m.nya);

    printf("Desea cambiar el email?(S/N): ");
    scanf("%c", &aux);
    if(aux == 'S')
    {
        printf("Nuevo email: ");
        fflush(stdin);
        fgets(m.email, sizeof(m.email), stdin);
        eliminarFinDeLinea(m.email);
    }
    fflush(stdin);
    printf("Desea cambiar el nombre y apellido?(S/N): "); ///HAY QUE NORMALIZAR???
    scanf("%c", &aux);
    if(aux == 'S')
    {
        printf("Nuevo nombre y apellido: ");
        fflush(stdin);
        fgets(m.nya, sizeof(m.nya), stdin);
        eliminarFinDeLinea(m.nya);
    }
    fflush(stdin);
    printf("Desea cambiar el sexo?(S/N): ");///NO SE POR QUE SEXO NO SE MODIFICA
    scanf("%c", &aux);
    if(aux == 'S')
    {
        printf("Nuevo sexo: ");
        fflush(stdin);
        fgets(m.sexo, sizeof(m.sexo), stdin);
        eliminarFinDeLinea(m.sexo);
    }
    fflush(stdin);

    printf("Desea cambiar el categoria?(S/N): ");
    scanf("%c", &aux);
    if(aux == 'S')
    {
        printf("Nuevo categoria: ");
        fflush(stdin);
        fgets(m.cat, sizeof(m.cat), stdin);
        eliminarFinDeLinea(m.cat);
    }
    fflush(stdin);
    printf("Desea cambiar el plan?(S/N): ");
    scanf("%c", &aux);
    if(aux == 'S')
    {
        printf("Nuevo plan: ");
        fflush(stdin);
        fgets(m.plan, sizeof(m.plan), stdin);
        eliminarFinDeLinea(m.plan);
    }

    ///realizar la modificacion de fechas.


    fflush(stdin);
    if(validaciones(&m, fecha) != 0)///CAMBIAR != 0
    {
        fclose(pf);
        return ERROR;
    }
    // Valida los datos modificados con la fecha actual
    fseek(pf, vec[pos].nro_reg * sizeof(t_miembro), SEEK_SET);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fflush(pf);
    fclose(pf);
    return OK;
}

int MostrarInfoMiembro(const char *nombreArch, t_indice *ind)
{
    t_miembro m;
    t_reg_indice clave;

    printf("\n=== MODIFICACION DE MIEMBRO ===\n");
    printf("Ingrese DNI a modificar: ");
    scanf("%ld", &m.dni);
    getchar();
    clave.dni = m.dni;

    int pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if(pos == -1)
    {
        printf("No se encontro el DNI.\n");
        return ERROR;
    }

    FILE *pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }

    t_reg_indice *vec = (t_reg_indice *)ind->vindice;
    fseek(pf, vec[pos].nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    printf("\nDNI: %ld\nNombre: %s\nSexo: %c\nCategoria: %s\nPlan: %s\nEmail: %s\nEstado: %c\n", m.dni, m.nya, m.sexo, m.cat, m.plan, m.email, m.estado);
    fclose(pf);
    return OK;
}

int ListadoXDNI(const char *nombreArch, t_indice *ind)
{
    int i;
    t_miembro m;
    FILE *pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }

    if (ind->cantidad_elementos_actual == 0)
    {
        printf("\nNo hay registros en el índice.\n");
        fclose(pf);
        return ERROR;
    }

    // Castear el índice original
    t_reg_indice *vecOrig = (t_reg_indice *)ind->vindice;

    // Crear copia dinámica del índice (para poder ordenarla)
    t_reg_indice *vec = malloc(ind->cantidad_elementos_actual * sizeof(t_reg_indice));
    if (!vec)
    {
        printf("\nError al reservar memoria.\n");
        fclose(pf);
        return ERROR;
    }

    memcpy(vec, vecOrig, ind->cantidad_elementos_actual * sizeof(t_reg_indice));

    qsort(vec, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);   // Ordenar la copia por DNI

    printf("\n=== LISTADO POR DNI ===\n");
    for(i = 0; i < ind->cantidad_elementos_actual; i++)
    {
        fseek(pf, vec[i].nro_reg * sizeof(t_miembro), SEEK_SET);
        fread(&m, sizeof(t_miembro), 1, pf);
        if(m.estado == 'A')
            printf("%ld - %-30s - Plan: %-10s - Email: %s\n", m.dni, m.nya, m.plan, m.email);
    }
    free(vec);
    fclose(pf);
    return OK;
}

int ListadoXPlan(const char *nombreArch, t_indice *ind)
{
    int i, n;
    char planActual[10];
    char planes[4][10]={"Basic","Premium","Vip","Family"};
    t_miembro aux;
    FILE *pf = fopen(nombreArch, "rb");
    if (!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }

    if (ind->cantidad_elementos_actual == 0)
    {
        printf("\nNo hay registros en el índice.\n");
        fclose(pf);
        return ERROR;
    }

    t_reg_indice *vecInd = (t_reg_indice *)ind->vindice;    // Casteo el vector del índice al tipo correcto



    for(i=0;i<4;i++){
        for(n=0;n<ind->cantidad_elementos_actual;n++){
            fseek(pf,sizeof(t_miembro)*((vecInd+n)->nro_reg),0);
            fread(&aux,sizeof(t_miembro),1,pf);
            if(strcmpi(*(planes+i),aux.plan)==0 && toupper(aux.estado)=='A'){
                printf("------ESTADO %c------\n",aux.estado);
                printf("DNI: %ld\n", aux.dni);
                printf("Nombre y Apellido: %s\n", aux.nya);
                printf("Fecha de Nacimiento: %02d/%02d/%04d\n", aux.fecha_nac.dia, aux.fecha_nac.mes, aux.fecha_nac.anio);
                printf("Sexo: %c\n", aux.sexo);
                printf("Fecha de Afiliación: %02d/%02d/%04d\n", aux.fecha_afi.dia, aux.fecha_afi.mes, aux.fecha_afi.anio);
                printf("Categoría: %s\n", aux.cat);
                printf("Fecha de última Cuota: %02d/%02d/%04d\n", aux.fecha_cuota.dia, aux.fecha_cuota.mes, aux.fecha_cuota.anio);
                printf("Estado: %c\n", aux.estado);
                printf("Plan: %s\n", aux.plan);
                printf("Email: %s\n", aux.email);
                printf("-------------------------------\n");
            }
        }
    }

    fclose(pf);
    return OK;
}
