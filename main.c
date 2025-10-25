#include "functions.h"

#define subcarpeta_binario "ArchivoBinario"
#define subcarpeta_error  "ArchivoTextoError"
#define nombreArchivoTexto "miembros-VC.txt"

int main(int argc, char *argv[])
{
    int procesamiento;
    char nombreArchivoBinario[60],nombreArchivoError[60];
    int flagProcesar;

    t_fecha fechaProceso;
    t_indice indice;

    fechaProceso = ingresarFechaProceso();
    LeeSubCarpeta(subcarpeta_binario,nombreArchivoBinario);
    indice_crear(&indice,CANT_ELEMENTOS,sizeof(t_reg_indice));

    flagProcesar=crearNombreArchivo(nombreArchivoBinario,nombreArchivoError,subcarpeta_binario,subcarpeta_error,&fechaProceso);

    if(flagProcesar==ERROR)
    {
        procesamiento=pasajeTextoBinario(nombreArchivoTexto,nombreArchivoBinario,nombreArchivoError,&fechaProceso,&indice,cmp_por_dni);
        if (procesamiento==OK)
            printf("Procesamiento exitoso\n");
        else
        {
            printf("Error en el procesamiento\n");
            indice_vaciar(&indice);
            return 0;
        }

        printf("Nombre archivo binario:%s\n",nombreArchivoBinario);
    }
    else if(indice_cargar(nombreArchivoBinario,&indice,indice.vindice,sizeof(t_reg_indice),cmp_por_dni)==ERROR){
        indice_vaciar(&indice);
        return 0;
    }


    system("pause");

    /// LOGICA PARTE 2 Y MENU

    menuMiembros(nombreArchivoBinario,&indice, &fechaProceso);

    indice_vaciar(&indice);
    return 0;
}
