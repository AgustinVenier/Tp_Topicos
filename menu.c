#include "functions.h"
#include "indice.h"
#include <stdlib.h>

void menuMiembros(const char * nombreArch)
{
    char op;
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
        do
        {
            switch(op)
            {
                case 'a':
                    Alta(nombreArch,&indice, &fechaProceso);
                    break;
                case 'b':
                    Baja(nombreArch, &indice);
                    break;
                case 'c':
                    Modificacion(nombreArch, &indice, &fechaProceso);
                    break;
                case 'd':
                    MostrarInfoMiembro(nombreArch,&indice);
                    break;
                case 'e':
                    ListadoXDNI(nombreArch, &indice);
                    break;
                case 'f':
                    ListadoXPlan(nombreArch, &indice);
                    break;
            }
        }while(op != 'g');
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

int validaciones(const t_miembro *m, const t_fecha *fecha)
{
    if (m->dni <= 0)
    {
        printf("Error: DNI inválido (%ld)\n", m->dni);
        return ERROR;
    }

    if (strlen(m->nya) == 0)
    {
        printf("Error: nombre o apellido vacío.\n");
        return ERROR;
    }

    if (strlen(m->cat) == 0)
    {
        printf("Error: categoría no especificada.\n");
        return ERROR;
    }

    if (fecha->anio < 2000 || fecha->anio > 2100)
    {
        printf("Error: fecha fuera de rango.\n");
        return ERROR;
    }
    return EXITO;
}


void Alta(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
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
            exit(1);
        }
    }
    // Calculo laposición física en el archivo
    fseek(pf, 0, SEEK_END);
    unsigned nro_reg = ftell(pf) / sizeof(t_miembro);

    // Ingreso datos del nuevo miembro
    printf("\n\nIngrese DNI: ");
    scanf("%ld", &m.dni);
    clave.dni = m.dni;

    // Valido si ya existe en el índice
    pos = indice_buscar(ind, &reg, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if (pos != -1)
    {
        printf("Error: ya existe un miembro con DNI %ld.\n", m.dni);
        fclose(pf);
        return ERROR;
    }
    // Valido los campos del miembro
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

    if(indice_insertar(ind, &reg, sizeof(t_reg_indice), cmp_por_dni) != EXITO)
        printf("No se pudo insertar en índice.\n");
    else
        printf("Alta realizada. DNI=%ld, nro_reg=%u\n", reg.dni, reg.nro_reg);
    fclose(pf);
}

void Baja(const char *nombreArch, t_indice *ind)
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
        exit(1);//return ERROR;
    }
    // Accede al vector de índices para obtener el número de registro físico
    t_reg_indice *vec = (t_reg_indice*)ind->vindice;
    unsigned nro_reg = vec[pos].nro_reg;

    FILE *pf = fopen(nombreArch, "r+b");
    if (!pf)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    // Leo el miembro desde el archivo
    fseek(pf, nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    if (m.estado == 'B')
    {
        printf("El registro ya está dado de baja.\n");
        fclose(pf);
        indice_eliminar(ind, nclave, sizeof(t_reg_indice), cmp_por_dni);
        exit(1);//return EXITO;
    }

    m.estado = 'B';
    fseek(pf, nro_reg * sizeof(t_miembro), SEEK_SET);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fflush(pf);

    indice_eliminar(ind, nclave, sizeof(t_reg_indice), cmp_por_dni);
    printf("Baja realizada para DNI %ld (nro_reg=%u)\n", dni, nro_reg);

    fclose(pf);
}


void Modificacion(const char *nombreArch, t_indice *ind, const t_fecha *fecha)
{
    long dni;
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
        exit(1);
    }

    FILE *pf = fopen(nombreArch, "r+b");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        exit(1);
    }
    // Posiciona el puntero en el registro correspondiente y lo lee
    fseek(pf, ind->vec[pos].nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    printf("Modificando a: %s\n", m.nya);
    // Solicita nuevo email (opcional)
    printf("Nuevo email (enter para dejar igual): ");
    char nuevoEmail[100];
    fgets(nuevoEmail, sizeof(nuevoEmail), stdin);
    if(strcmp(nuevoEmail, "\n") != 0) {
        nuevoEmail[strcspn(nuevoEmail, "\n")] = '\0';
        strcpy(m.email, nuevoEmail);
    }
    // Solicita nuevo plan (opcional)
    printf("Nuevo plan (enter para dejar igual): ");
    char nuevoPlan[30];
    fgets(nuevoPlan, sizeof(nuevoPlan), stdin);
    if(strcmp(nuevoPlan, "\n") != 0) {
        nuevoPlan[strcspn(nuevoPlan, "\n")] = '\0';
        strcpy(m.plan, nuevoPlan);
    }

    if(validaciones(&m, fecha) != EXITO)
    {
        printf("Modificacion invalida.\n");
        fclose(pf);
        exit(1);
    }
    // Valida los datos modificados con la fecha actual
    fseek(pf, ind->vec[pos].nro_reg * sizeof(t_miembro), SEEK_SET);
    fwrite(&m, sizeof(t_miembro), 1, pf);
    fflush(pf);

    printf("Modificacion guardada.\n");
    fclose(pf);
}

void MostrarInfoMiembro(const char *nombreArch, t_indice *ind)
{
    long dni;
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
        exit(1);
    }

    FILE *pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        exit(1);
    }

    fseek(pf, indice->vec[pos].nro_reg * sizeof(t_miembro), SEEK_SET);
    fread(&m, sizeof(t_miembro), 1, pf);

    printf("\nDNI: %ld\nNombre: %s\nSexo: %c\nCategoria: %s\nPlan: %s\nEmail: %s\nEstado: %c\n", m.dni, m.nya, m.sexo, m.cat, m.plan, m.email, m.estado);

    fclose(pf);
}

void ListadoXDNI(const char *nombreArch, t_indice *ind)
{
    int i;
    t_miembro m;
    FILE *pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        exit(1);
    }
    printf("\n=== LISTADO POR DNI ===\n");

    for(i = 0; i < ind->cantidad_elementos_actual; i++)
    {
        fseek(pf, ind->vec[i].nro_reg * sizeof(t_miembro), SEEK_SET);
        fread(&m, sizeof(t_miembro), 1, fb);
        if(m.estado == 'A')
            printf("%ld - %-30s - Plan: %-10s - Email: %s\n", m.dni, m.nya, m.plan, m.email);
    }
    fclose(pf);
}

void ListadoXPlan(const char *nombreArch, t_indice *ind)
{
    int i, n=0;
    FILE *pf = fopen(nombreArch, "rb");
    if(!pf)
    {
        printf("\nError al abrir archivo binario");
        exit(1);
    }
    // Reserva memoria para almacenar todos los miembros activos
    t_miembro *vec = malloc(ind->cantidad_elementos_actual * sizeof(t_miembro));

    for(i = 0; i < ind->cantidad_elementos_actual; i++)
    {
        fseek(pf, ind->vec[i].nro_reg * sizeof(t_miembro), SEEK_SET);
        fread(&vec[n], sizeof(t_miembro), 1, pf);
        if(vec[n].estado == 'A')    // Solo guardo miembros activos
            n++;
    }
    // Ordena los miembros activos por plan y luego por DNI
    qsort(vec, n, sizeof(t_miembro), cmpPlanDni); //qsort está en stdlib

    printf("\n=== LISTADO POR PLAN ===\n");
    char planActual[30] = "";
    for(i = 0; i < n; i++)
    {
        if(strcasecmp(planActual, vec[i].plan) != 0)
        {
            // Si el plan cambió, imprime encabezado nuevo
            strcpy(planActual, vec[i].plan);
            printf("\n--- PLAN: %s ---\n", planActual);
        }
        printf("%ld - %-30s - Email: %s\n", vec[i].dni, vec[i].nya, vec[i].email);
    }
    free(vec);
    fclose(pf);
}
