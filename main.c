#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "functions.h"




int main(int argc, char *argv[])
{
    int num;
    /* TEST
    //t_miembro miembro[5];
    t_fecha *p_fechanac,fechanac;
    p_fechanac=&fechanac;
    p_fechanac->anio=2013;
    p_fechanac->mes=02;
    p_fechanac->dia=28;


    ///Invocacion validacion nacimiento // num= fNacValido(&fechanac);
    //printf("Es nac val: %d",num);
    */

    //------------

    t_fecha fechaProceso, fechaRecuperar;
    t_fecha *pf = &fechaProceso;
    t_fecha *pr = &fechaRecuperar;

    int valorFechaProc, valorFechaRecu;
    char recuperar;
    /*
    do
    {
        if (valorFecha == ERROR) /// valor fecha no está definido
            printf("Ingrese la fecha nuevamente (D/M/A): ");
        else
            printf("Ingrese la fecha del proceso (D/M/A): ");

        scanf(FORMATO_FECHA, &pf->dia, &pf->mes, &pf->anio);
        valorFechaProc = validarFecha(pf);   // pf es puntero válido
    }while (valorFechaProc == ERROR);

    printf("Desea recuperar un archivo modificado en corridas previas a la fecha? (S/N): ");

    scanf("%c", &recuperar);

    if(toupper(recuperar) == 'S')
    {
        do
        {
            if (valorFecha == ERROR) /// valor fecha no está definido
                printf("Ingrese la fecha nuevamente (D/M/A): ");
            else
                printf("Ingrese la fecha del archivo a recuperar (D/M/A): ");

            scanf(FORMATO_FECHA, &pr->dia, &pr->mes, &pr->anio);
            valorFechaRecu = validarFecha(pr);
        }while(valorFechaRecu == ERROR ); //DEBE SER MENOR A LA FECHA DEL PROCESO ACTUAL?

        //pasajeTextoBinario()
    }
    */

    num = pasajeTextoBinario("Miembros-VC.txt","Miembros-VC-15102025.dat","1.dat");
    printf("%d",num);



        return 0;
}
