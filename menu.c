#include "functions.h"
#include "indice.h"

void menuMiembros(const char * nombreArch)
{
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
        do{
            switch(op){
                case 'a':
                    Alta(nombreArchivoBinario,&indice, &fechaProceso);
                    break;
                case 'b':
                    Baja(nombreArchivoBinario, &indice);
                    break;
                case 'c':
                    Modificacion(nombreArchivoBinario, &indice, &fechaProceso);
                    break;
                case 'd':
                    MostrarInfoMiembro(nombreArchivoBinario,&indice);
                    break;
                case 'e':
                    ListadoXDNI(nombreArchivoBinario, &indice);
                    break;
                case 'f':
                    ListadoXPlan(nombreArchivoBinario, &indice);
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
    } while(strchr(opc, opta) == NULL);
    return opta;
}

int validaciones(const t_miembro *m)
{
    // Validar DNI
    if (m->dni <= 0)
    {
        printf("Error: DNI inválido (%ld)\n", m->dni);
        return ERROR;
    }
    // Validar nombre
    if (strlen(m->nombre) == 0 || strlen(m->apellido) == 0)
    {
        printf("Error: nombre o apellido vacío.\n");
        return ERROR;
    }
    return EXITO;
}

int validaciones(const t_miembro *m, const t_fecha *fecha)
{
    if (m->dni <= 0)
    {
        printf("Error: DNI inválido (%ld)\n", m->dni);
        return ERROR;
    }

    if (strlen(m->nombre) == 0 || strlen(m->apellido) == 0)
    {
        printf("Error: nombre o apellido vacío.\n");
        return ERROR;
    }

    if (strlen(m->categoria) == 0)
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
    FILE *pf = fopen(nombreArch, "r+b");
    if (!pf)
    {
        // si no existe, crear nuevo archivo
        fb = fopen(nombreArch, "w+b");
        if (!pf)
        {
            printf("No se pudo abrir el archivo");
            return 1;
        }
    }
    unsigned nro_reg = 0;
    // Calculo el número de registro actual (antes de escribir)
    fseek(pf, 0, SEEK_END);
    nro_reg = ftell(pf) / sizeof(t_miembro);

    // Ingreso datos del nuevo miembro
    t_miembro m;
    int pos;

    printf("\n\nIngrese DNI: ");
    scanf("%ld", &m->dni);

    // Valido si ya existe en el índice
    t_reg_indice clave;
    clave.dni = m.dni;

    pos = indice_buscar(ind, &clave, ind->cant, sizeof(t_reg_indice), cmp_por_dni);

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

    // Insertar en índice:
    t_reg_indice ri;

    ri.dni = m.dni;
    ri.nro_reg = nro_reg;

    if(indice_insertar(ind, &ri, sizeof(t_reg_indice), cmp_por_dni) != EXITO)
        printf("No se pudo insertar en índice.\n");
    else
        printf("Alta realizada. DNI=%ld, nro_reg=%u\n", ri.dni, ri.nro_reg);
    fclose(pf);
}

void Baja(const char *nombreArch, t_indice *ind)
{
    long dni;
    int pos;
    t_miembro m;
    pedir_miembro_por_teclado(&m);

    t_reg_indice clave;
    clave.dni = m.dni;

    printf("DNI a dar de baja: ");
    scanf("%ld", &m->dni);
    pos = indice_buscar(ind, &clave, ind->cant, sizeof(t_reg_indice), cmp_por_dni);

    if (pos == -1)
    {
        printf("No existe miembro con DNI %ld\n", dni);
        return ERROR;
    }

    unsigned nro_reg = ind->vec[pos].nro_reg;

    FILE *pf = fopen(nombre_dat, "r+b");
    if (!pf)
    {
        printf("No se pudo abrir el archivo");
        return 1;
    }

    if (m.estado == 'B')
    {
        printf("El registro ya está dado de baja.\n");
        fclose(pf);
        indice_eliminar(ind, (size_t)pos);
        return EXITO;
    }
    else
        m.estado = 'B';
    }
    indice_eliminar(ind, &clave, ind->cant, sizeof(t_reg_indice), cmp_por_dni);
    printf("Baja realizada para DNI %ld (nro_reg=%u)\n", dni, nro_reg);

    fclose(pf);
}

