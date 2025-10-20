#include "functions.h"

int main(int argc, char *argv[])
{
    int num;

    ///Invocacion validacion nacimiento // num= fNacValido(&fechanac);
    //printf("Es nac val: %d",num);


    //------------

    t_fecha fechaProceso, fechaRecuperar;
    t_fecha *pf = &fechaProceso;
    t_fecha *pr = &fechaRecuperar;

    int valorFechaProc, valorFechaRecu;
    char recuperar;

    do
    {
        if (valorFecha == ERROR) /// valor fecha no está definido
            printf("Ingrese la fecha nuevamente (D/M/A): ");
        else
            printf("Ingrese la fecha del proceso (D/M/A): ");

        scanf(FORMATO_FECHA, &pf->dia, &pf->mes, &pf->anio);
        valorFechaProc = validarFecha(pf);   // pf es puntero válido
    }
    while (valorFechaProc == ERROR);

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
        }
        while(valorFechaRecu == ERROR );  //DEBE SER MENOR A LA FECHA DEL PROCESO ACTUAL?

        //pasajeTextoBinario()
    }


    //num = pasajeTextoBinario("Miembros-VC.txt","Miembros-VC-15102025.dat","1.dat");
    //printf("%d",num);

}


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
