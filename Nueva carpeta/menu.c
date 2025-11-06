#include "functions.h"
#include "indice.h"

void menuMiembros(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
{
    char op;
    int resultado;
    do
    {

        printf("\n");


        op = mostrarMenu(
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
                printf("\nError al dar de alta el miembro.\n");
            else
                printf("\nAlta realizada con exito.\n");
            sleep(2);
            system("cls");
            break;

        case 'b':
            resultado = Baja(nombreArch, ind);
            if (resultado == ERROR)
                printf("\nError al dar la baja del miembro.\n");
            else
                printf("\nBaja realizada con exito.\n");
            sleep(2);
            system("cls");
            break;

        case 'c':
            resultado = Modificacion(nombreArch, ind, fecha);
            if (resultado == ERROR)
                printf("\nError al modificar el miembro.\n");
            else
                printf("\nModificacion realizada con exito.\n");
            sleep(2);
            system("cls");
            break;

        case 'd':
            resultado = MostrarInfoMiembro(nombreArch,ind);
            if (resultado == ERROR)
                printf("\nError al mostrar\n");
            else
                printf("\nRealizada con exito.\n");
            system("pause");
            break;

        case 'e':
            resultado = ListadoXDNI(nombreArch, ind);
            if (resultado == ERROR)
                printf("\nError al listar\n");
            else
                printf("\nRealizada con exito.\n");
            system("pause");
            break;

        case 'f':
            resultado = ListadoXPlan(nombreArch, ind);
            if (resultado == ERROR)
                printf("\nError al Listar\n");
            else
                printf("\nRealizada con exito.\n");
            system("pause");
            break;
        }
    }
    while(op != 'g');
}

void eliminarFinDeLinea(char *cad)
{
    char *p = strchr(cad, '\n');
    if (p)
        *p = '\0';
}

char mostrarMenu(const char *msj, const char *opc)
{
    char opta;
    int priVez = 1;
    printf("\n===================================================\n");
    printf("              GESTION DE MIEMBROS\n");
    printf("===================================================\n");
    do
    {
        fflush(stdin);
        printf("%s%s",
               priVez ? priVez = 0, "" : "ERROR - Opcion No valida\n",
               msj);
        printf("\nIngrese seleccion: ");
        scanf("%c", &opta);
    }
    while(strchr(opc, opta) == NULL);    //busca el carácter dentro del conjunto de válidos
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
        printf("No se pudo abrir el archivo");
        return ERROR;
    }
    // Calculo la posición física en el archivo
    fseek(pf, 0, SEEK_END);
    unsigned nro_reg = ftell(pf) / sizeof(t_miembro);

    // Ingreso datos del nuevo miembro
    printf("\n\nIngrese DNI: ");
    scanf("%ld", &m.dni);
    fflush(stdin);
    reg.dni = m.dni;

    // Valido si ya existe en el índice
    pos = indice_buscar(ind, &reg, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if (pos != NO_EXISTE)
    {
        printf("Error: ya existe un miembro con DNI %ld.\n", m.dni);
        fclose(pf);
        return ERROR;
    }

    //inicializar lo que falta de t_reg
    reg.nro_reg = nro_reg;

    //Ingreso del resto de los datos
    printf("Ingrese Apellido y Nombre: ");
    fgets(m.nya, sizeof(m.nya), stdin);
    eliminarFinDeLinea(m.nya);
    normalizar(m.nya);


    printf("Ingrese sexo (M/F): ");
    scanf("%c", &m.sexo);
    fflush(stdin);
    while(!sexValido(m.sexo))
    {
        printf("\n Ingreso Incorrecto\n");
        printf("Ingrese sexo (M/F): ");
        scanf("%c", &m.sexo);
        fflush(stdin);
    }

    printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio);
    fflush(stdin);
    while(fNacValido(&m.fecha_nac,fecha))
    {
        printf("\n Ingreso Incorrecto\n");
        printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio);
        fflush(stdin);
    }

    printf("Ingrese fecha de afiliacion (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &m.fecha_afi.dia, &m.fecha_afi.mes, &m.fecha_afi.anio);
    fflush(stdin);
    while(fAfiliacionValido(&m.fecha_afi,fecha,&m.fecha_nac)==ERROR)
    {
        printf("\n Ingreso Incorrecto\n");
        printf("Ingrese fecha de afiliacion (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &m.fecha_afi.dia, &m.fecha_afi.mes, &m.fecha_afi.anio);
        fflush(stdin);
    }

    printf("Ingrese categoria (MENOR/ADULTO): ");
    fgets(m.cat, sizeof(m.cat), stdin);
    eliminarFinDeLinea(m.cat);
    while(validarFechaCategoria(m.cat,&m.fecha_nac,fecha))
    {
        printf("\n Ingreso Incorrecto\n");
        printf("Ingrese categoria (MENOR/ADULTO): ");
        fgets(m.cat, sizeof(m.cat), stdin);
        eliminarFinDeLinea(m.cat);
    }

    printf("Ingrese fecha de ultima cuota paga (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &m.fecha_cuota.dia, &m.fecha_cuota.mes, &m.fecha_cuota.anio);
    fflush(stdin);
    while(fUltCoutaValido(&m.fecha_cuota,&m.fecha_afi,fecha))
    {
        printf("\n Ingreso Incorrecto\n");
        printf("Ingrese fecha de ultima cuota paga (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &m.fecha_cuota.dia, &m.fecha_cuota.mes, &m.fecha_cuota.anio);
        fflush(stdin);
    }

    m.estado='A';

    printf("Ingrese plan (BASIC/PREMIUM/VIP/FAMILY): ");
    fgets(m.plan, sizeof(m.plan), stdin);
    eliminarFinDeLinea(m.plan);
    while(!planValido(m.plan))
    {
        printf("\n Ingreso Incorrecto\n");
        printf("Ingrese plan (BASIC/PREMIUM/VIP/FAMILY): ");
        fgets(m.plan, sizeof(m.plan), stdin);
        eliminarFinDeLinea(m.plan);
    }

    if(strcmpi(m.cat,"MENOR")== 0)
    {
        printf("Ingrese email: ");
        fgets(m.email, sizeof(m.email), stdin);
        eliminarFinDeLinea(m.email);
                    while(validarEmail(m.email))
            {
                printf("\n Ingreso Incorrecto\n");
                printf("Ingrese email: ");
                fgets(m.email, sizeof(m.email), stdin);
                eliminarFinDeLinea(m.email);

            }
    }
    // Escribir al final:
    fseek(pf, 0, SEEK_END);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fflush(pf);

    // Insertar en el  índice:
    if(indice_insertar(ind, &reg, sizeof(t_reg_indice), cmp_por_dni) != OK)
    {
        fclose(pf);
        return ERROR;
    }
    else
        printf("Alta realizada. DNI=%ld, nro_reg=%u\n", reg.dni, reg.nro_reg);
    fclose(pf);
    return OK;
}

int Baja(const char *nombreArch, t_indice *ind)
{
    int pos;
    t_miembro m;
    t_reg_indice clave;
    t_reg_indice *vec;
    FILE *pf;
    unsigned nro_reg;
    // Solicito DNI y lo asigno a la clave
    printf("DNI a dar de baja: ");
    scanf("%ld", &m.dni);
    clave.dni = m.dni;
    // Busco posición en el índice
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if (pos == NO_EXISTE)
    {
        printf("No existe miembro con DNI %ld\n", clave.dni);
        return ERROR;
    }
    // Accede al vector de índices para obtener el número de registro físico
    vec = (t_reg_indice*)ind->vindice;
    nro_reg = (vec+pos)->nro_reg;

    pf = fopen(nombreArch, "r+b");
    if (!pf)
    {
        printf("No se pudo abrir el archivo");
        return ERROR;
    }
    // Leo el miembro desde el archivo
    fseek(pf, nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    m.estado = 'B';
    fseek(pf, (long)-sizeof(t_miembro), SEEK_CUR);
    fwrite(&m, sizeof(t_miembro), 1, pf);

    fclose(pf);
    if(indice_eliminar(ind, &clave, sizeof(t_reg_indice), cmp_por_dni)==ERROR)
    {
        return ERROR;
    }
    return OK;
}

int Modificacion(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
{
    t_miembro m;
    t_reg_indice clave;
    char aux;
    FILE *pf;
    int pos;
    t_reg_indice *vec;

    printf("\n=== MODIFICACION DE MIEMBRO ===\n");
    printf("Ingrese DNI a modificar: ");
    scanf("%ld", &m.dni);
    getchar();
    clave.dni = m.dni;

    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if(pos == NO_EXISTE)
    {
        printf("No se encontro el DNI.\n");
        return ERROR;
    }

    pf = fopen(nombreArch, "r+b");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }
    vec = (t_reg_indice *)ind->vindice; //casteo

    // Posiciona el puntero en el registro correspondiente y lo lee
    fseek(pf, (vec+pos)->nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    printf("Modificando a: %s\n", m.nya);

    preguntarCambio("apellido y nombre", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nuevo apellido y nombre: ");
        fgets(m.nya, sizeof(m.nya), stdin);
        eliminarFinDeLinea(m.nya);
        normalizar(m.nya);
    }

    preguntarCambio("fecha de nacimiento", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nueva Fecha de Nacimiento(DD/MM/AAAA): ");
        scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio);
        fflush(stdin);
        while(fNacValido(&m.fecha_nac,fecha))
        {
            printf("\n Ingreso Incorrecto\n");
            printf("Nueva Fecha de Nacimiento(DD/MM/AAAA): ");
            scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio);
            fflush(stdin);
        }
    }

    preguntarCambio("sexo", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nuevo sexo: ");
        scanf("%c", &m.sexo);
        fflush(stdin);
        while(!sexValido(m.sexo))
        {
            printf("\n Ingreso Incorrecto\n");
            printf("Nuevo sexo: ");
            scanf("%c", &m.sexo);
            fflush(stdin);
        }
    }

    preguntarCambio("fecha de afiliacion: ", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nueva Fecha de afiliacion (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &m.fecha_afi.dia, &m.fecha_afi.mes, &m.fecha_afi.anio);
        fflush(stdin);
        while(fAfiliacionValido(&m.fecha_afi,fecha,&m.fecha_nac)==ERROR)
        {
            printf("\n Ingreso Incorrecto\n");
            printf("Nueva Fecha de afiliacion (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &m.fecha_afi.dia, &m.fecha_afi.mes, &m.fecha_afi.anio);
            fflush(stdin);
        }
    }

    preguntarCambio("categoria", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nueva categoria: ");
        fgets(m.cat, sizeof(m.cat), stdin);
        eliminarFinDeLinea(m.cat);
        while(validarFechaCategoria(m.cat,&m.fecha_nac,fecha))
        {
            printf("\n Ingreso Incorrecto\n");
            printf("Nueva categoria: ");
            fgets(m.cat, sizeof(m.cat), stdin);
            eliminarFinDeLinea(m.cat);
        }
    }

    preguntarCambio("fecha de ultima cuota: ", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nueva Fecha de ultima cuota (DD/MM/AAAA): ");
        scanf("%d/%d/%d", &m.fecha_cuota.dia, &m.fecha_cuota.mes, &m.fecha_cuota.anio);
        fflush(stdin);
        while(fUltCoutaValido(&m.fecha_cuota,&m.fecha_afi,fecha))
        {
            printf("\n Ingreso Incorrecto\n");
            printf("Nueva Fecha de ultima cuota (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &m.fecha_cuota.dia, &m.fecha_cuota.mes, &m.fecha_cuota.anio);
            fflush(stdin);
        }
    }

    preguntarCambio("plan", &aux);
    if(toupper(aux) == 'S')
    {
        printf("Nuevo plan: ");
        fgets(m.plan, sizeof(m.plan), stdin);
        eliminarFinDeLinea(m.plan);
        while(!planValido(m.plan))
        {
            printf("\n Ingreso Incorrecto\n");
            printf("Nuevo plan: ");
            fgets(m.plan, sizeof(m.plan), stdin);
            eliminarFinDeLinea(m.plan);
        }
    }

    if(strcmpi(m.cat,"MENOR")==0)
    {
        preguntarCambio("email", &aux);
        if(toupper(aux) == 'S')
        {
            printf("Nuevo email: ");
            fgets(m.email, sizeof(m.email), stdin);
            eliminarFinDeLinea(m.email);
            while(validarEmail(m.email))
            {
                printf("\n Ingreso Incorrecto\n");
                printf("Nuevo email: ");
                fgets(m.email, sizeof(m.email), stdin);
                eliminarFinDeLinea(m.email);

            }
        }
    }
    /*
    if(validaciones(&m, fecha) != OK)
    {
        fclose(pf);
        return ERROR;
    }
    // Valida los datos modificados con la fecha actual
    */
    fseek(pf, (vec+pos)->nro_reg * sizeof(t_miembro), SEEK_SET);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fclose(pf);
    return OK;
}

int MostrarInfoMiembro(const char *nombreArch, t_indice *ind)
{
    t_miembro m;
    t_reg_indice clave;
    int pos;
    FILE *pf;
    t_reg_indice *vec;

    printf("\n============= INFORMACION DE MIEMBRO =============\n");
    printf("Ingrese DNI a visualizar: ");
    scanf("%ld", &m.dni);
    getchar();
    clave.dni = m.dni;

    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if(pos == NO_EXISTE)
    {
        printf("No se encontro el DNI.\n");
        return ERROR;
    }

    pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }

    vec = (t_reg_indice *)ind->vindice;
    fseek(pf, (vec+pos)->nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);
    printf("-  DNI: %8ld  -  Nombre: %-60s  -  FNac: %02d/%02d/%04d  -  Sexo: %c  -  FAfi: %02d/%02d/%04d  -  Categoria: %-10s  -  FUltCuota: %02d/%02d/%04d  -  Plan: %-10s  -  Email: %s\n", m.dni, m.nya, m.fecha_nac.dia,
           m.fecha_nac.mes, m.fecha_nac.anio, m.sexo, m.fecha_afi.dia,m.fecha_afi.mes, m.fecha_afi.anio, m.cat, m.fecha_cuota.dia, m.fecha_cuota.mes, m.fecha_cuota.anio, m.plan, m.email);
    fclose(pf);
    return OK;
}

int ListadoXDNI(const char *nombreArch, t_indice *ind)
{
    int i;
    t_miembro m;
    FILE *pf;
    t_reg_indice *vecOrig;

    pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        return ERROR;
    }

    if (indice_vacio(ind)!=ERROR)
    {
        printf("\nNo hay registros en el indice.\n");
        fclose(pf);
        return ERROR;
    }

    // Castear el índice original
    vecOrig = (t_reg_indice *)ind->vindice;


    printf("\n\n=====================================================================================================================================================================================\n");
    printf("                                                                                                    LISTADO POR DNI");
    printf("\n======================================================================================================================================================================================\n");
    for(i = 0; i < ind->cantidad_elementos_actual; i++)
    {
        fseek(pf, (vecOrig+i)->nro_reg * sizeof(t_miembro), SEEK_SET);
        fread(&m, sizeof(t_miembro), 1, pf);
        printf("-  DNI: %8ld  -  Nombre: %-60s  -  FNac: %02d/%02d/%04d  -  Sexo: %c  -  FAfi: %02d/%02d/%04d  -  Categoria: %-10s  -  FUltCuota: %02d/%02d/%04d  -  Plan: %-10s  -  Email: %s\n", m.dni, m.nya, m.fecha_nac.dia,
               m.fecha_nac.mes, m.fecha_nac.anio, m.sexo, m.fecha_afi.dia,m.fecha_afi.mes, m.fecha_afi.anio, m.cat, m.fecha_cuota.dia, m.fecha_cuota.mes, m.fecha_cuota.anio, m.plan, m.email); ///IMPRIMIR MAS CAMPOS
    }
    fclose(pf);
    return OK;
}

int ListadoXPlan(const char *nombreArch, t_indice *ind)
{
    int i, n;
    char planes[4][10]= {"BASIC","PREMIUM","VIP","FAMILY"};
    t_miembro aux;
    FILE *pf;
    t_reg_indice *vecInd;


    pf = fopen(nombreArch, "rb");
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

    vecInd = (t_reg_indice *)ind->vindice;

    for(i=0; i<4; i++)
    {
        printf("\n\n=====================================\n");
        printf("             PLAN %s \n",(planes[i]));
        printf("=====================================\n");
        for(n=0; n<ind->cantidad_elementos_actual; n++)
        {
            fseek(pf,sizeof(t_miembro)*((vecInd+n)->nro_reg),0);
            fread(&aux,sizeof(t_miembro),1,pf);
            if(strcmpi(*(planes+i),aux.plan)==0 && toupper(aux.estado)=='A')
            {
                printf("DNI: %ld\n", aux.dni);
                printf("Nombre y Apellido: %s\n", aux.nya);
                printf("Fecha de Nacimiento: %02d/%02d/%04d\n", aux.fecha_nac.dia, aux.fecha_nac.mes, aux.fecha_nac.anio);
                printf("Sexo: %c\n", aux.sexo);
                printf("Fecha de Afiliacion: %02d/%02d/%04d\n", aux.fecha_afi.dia, aux.fecha_afi.mes, aux.fecha_afi.anio);
                printf("Categoria: %s\n", aux.cat);
                printf("Fecha de ultima Cuota: %02d/%02d/%04d\n", aux.fecha_cuota.dia, aux.fecha_cuota.mes, aux.fecha_cuota.anio);
                printf("Estado: %c\n", aux.estado);
                printf("Plan: %s\n", aux.plan);
                printf("Email: %s\n", aux.email);
                printf("-------------------------------------\n");
            }
        }
    }

    fclose(pf);
    return OK;
}

void preguntarCambio(const char * cad, char* aux)
{
    printf("Desea cambiar %s (S/N): ", cad);
    do
    {
        scanf("%c", aux);
        fflush(stdin);
        if(toupper(*aux)!='S' && toupper(*aux)!='N')
        {
            printf("Opcion invalida, ingrese de nuevo: ");
        }
    }
    while(toupper(*aux)!='S' && toupper(*aux)!='N');
}
