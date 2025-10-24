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

    if(flagProcesar==FALLA)
    {
        procesamiento=pasajeTextoBinario(nombreArchivoTexto,nombreArchivoBinario,nombreArchivoError,&fechaProceso,&indice,cmp_por_dni);
        if (procesamiento==EXITO)
            printf("Procesamiento exitoso\n");
        else
        {
            printf("Error en el procesamiento\n");
            return 0;
        }

    printf("Nombre archivo binario:%s\n",nombreArchivoBinario);
    }
    else if(indice_cargar(nombreArchivoBinario,&indice,indice.vindice,sizeof(t_reg_indice),cmp_por_dni)==ERROR){
        indice_vaciar(&indice);
        return 0;
    }


    system("pause");
    //mostrarMiembros(nombreArchivoBinario); //Solo para testear

    /// LOGICA PARTE 2 Y MENU
    if(flagProcesar!=FALLA)
    {
      if(indice_cargar(nombreArchivoBinario,&indice,indice.vindice,sizeof(t_reg_indice),cmp_por_dni)==ERROR)
            return 0;
    }

    menuMiembros(nombreArchivoBinario,&indice, &fechaProceso);

    indice_vaciar(&indice);
    return 0;
}
/*
    LeeSubCarpeta(subcarpeta_error,nombreArchivo); ///busca si hay algo en la carpeta de arch texto
    if(*nombreArchivo==NULL)
        printf("vacio");
    else
        printf("%s\n",nombreArchivo);
*/

/* TEST
//t_miembro miembro[5];
t_fecha *p_fechanac,fechanac;
p_fechanac=&fechanac;
p_fechanac->anio=2013;
p_fechanac->mes=02;
p_fechanac->dia=28;
*/
///REGISTROS PARA TESTEAR VALIDACIONES
/*
t_fecha fechaProceso = {2025, 10, 17};

t_miembro miembros[10] = {
    // 0️Válido — todo correcto
    {12345678, "Perez, Juana", {2010, 10, 25}, 'F', {2020, 1, 1}, "MENOR", {2024, 1, 1}, 'B', "FAMILY", "juana@correo.com"},

    // 1️DNI inválido (menor a 1.000.000)
    {99999, "Lopez, Ana", {2000, 1, 1}, 'F', {2020, 1, 1}, "ADULTO", {2024, 1, 1}, 'A', "BASIC", "ana@correo.com"},

    // 2️Fecha de nacimiento futura (2030)
    {23456789, "Martinez, Jose", {2030, 1, 1}, 'M', {2020, 1, 1}, "ADULTO", {2024, 1, 1}, 'A', "VIP", "jose@mail.com"},

    // 3️Sexo inválido (no 'M' ni 'F')
    {34567890, "Rodriguez, Carla", {1990, 1, 1}, 'X', {2020, 1, 1}, "ADULTO", {2024, 1, 1}, 'A', "PREMIUM", "carla@mail.com"},

    // 4️Fecha afiliación > fecha de proceso (futuro)
    {45678901, "Gomez, Juan", {1990, 1, 1}, 'M', {2025, 12, 2}, "ADULTO", {2024, 1, 1}, 'A', "BASIC", "juan@mail.com"},

    // 5️Fecha afiliación < fecha de nacimiento
    {56789012, "Sanchez, Laura", {2000, 5, 1}, 'F', {1999, 1, 1}, "ADULTO", {2024, 1, 1}, 'A', "FAMILY", "laura@mail.com"},

    // 6️Categoría incorrecta (tiene 15 años pero figura como ADULTO)
    {67890123, "Diaz, Tomas", {2010, 1, 1}, 'M', {2020, 1, 1}, "ADULTO", {2024, 1, 1}, 'A', "VIP", "tomas@mail.com"},

    // 7️Última cuota < afiliación
    {78901234, "Suarez, Belen", {1990, 1, 1}, 'F', {2020, 1, 1}, "ADULTO", {2019, 1, 1}, 'A', "PREMIUM", "belen@mail.com"},

    // 8️Plan inválido (no pertenece a BASIC, PREMIUM, VIP, FAMILY)
    {89012345, "Moreno, Pablo", {1990, 1, 1}, 'M', {2020, 1, 1}, "ADULTO", {2024, 1, 1}, 'A', "DELUXE", "pablo@mail.com"},

    // 9️Menor sin email (debe tenerlo)
    {90123456, "Ramos, Sofia", {2010, 1, 25}, 'F', {2020, 1, 1}, "MENOR", {2024, 1, 1}, 'A', "BASIC", ""}
};
*/
/*
//,*="miembros-VC.txt",aux[10];
    // char *subcarpeta_binario="ArchivoBinario",*subcarpeta_error="ArchivoTextoError";
    if(*nombreArchivoBinario==' ') // si no hay ningun archivo anterior en la subcarpeta
    {
        printf("Se procesara Archivo miembros-VC.txt , no hay archivos para recuperar \n");
        strcpy(nombreArchivoBinario,subcarpeta_binario);
        strcat(nombreArchivoBinario, "/miembros-VC-");
        sprintf(aux,"%04d%02d%02d",pf->anio,pf->mes,pf->dia);
        strcat(nombreArchivoBinario,aux);
        strcat(nombreArchivoBinario,".dat");

        strcpy(nombreArchivoError,subcarpeta_error);
        strcat(nombreArchivoError, "/error-VC-");
        strcat(nombreArchivoError,aux);
        strcat(nombreArchivoError,".txt");

        procesamiento=pasajeTextoBinario(nombreArchivoTexto,nombreArchivoBinario,nombreArchivoError,pf);
        if (procesamiento==EXITO)
            printf("Procesamiento exitoso\n");
        else
            printf("Error en el procesamiento\n");
    }

        else
        {
            strcpy(aux,subcarpeta_binario);
            strcat(aux,"/");
            strcat(aux,nombreArchivoBinario);
            strcpy(nombreArchivoBinario,aux);
            printf("El archivo a recuperar es : %s\n",nombreArchivoBinario);
        }
*/
