#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "error.h"
#include "defines.h"

static LPSTR strTabla[3][MAX_ENTRADAS];
static DWORD dwContadorDeConfiguracion = 0;
static DWORD dwContadorDeProcesos = 0;
static DWORD dwContadorDeMandatos = 0;


void AnyadirConfiguracion (LPSTR strConfiguracion)
{

	if (!(strTabla[0][0] = (LPSTR) malloc (strlen (strConfiguracion) + 1)))
		ErrorCritico ("memoria insuficiente @ AnyadirConfiguracion()");

	strcpy (strTabla[0][0], strConfiguracion);  /* solo puede haber un fichero */
	dwContadorDeConfiguracion++;

}



void AnyadirProceso (LPSTR strProceso)
{

	if (!(strTabla[1][dwContadorDeProcesos] = (LPSTR) malloc (strlen (strProceso) + 1)))
		ErrorCritico ("memoria insuficiente @ AnyadirProceso()");

	strcpy (strTabla[1][dwContadorDeProcesos], strProceso);
	dwContadorDeProcesos++;

}



void AnyadirMandato (LPSTR strMandato)
{

	if (!(strTabla[2][dwContadorDeMandatos] = (LPSTR) malloc (strlen (strMandato) + 1)))
		ErrorCritico ("memoria insuficiente @ AnyadirMandato()");

	strcpy (strTabla[2][dwContadorDeMandatos], strMandato);
	dwContadorDeMandatos++;

}



void ImprimirTabla (void)
{

	int i;


	/* imprimir fichero de configuracion */
	printf ("FICHERO DE CONFIGURACION\n");
	if (dwContadorDeConfiguracion)
		printf ("%s\n", strTabla[0][0]);
	printf ("\n");

	/* imprimir procesos */
	printf ("PROCESOS EN EJECUCION\n");
	for (i = 0; i < (int) dwContadorDeProcesos; i++)
		printf ("%s\n", strTabla[1][i]);
	printf ("\n");

	/* imprimir mandatos */
	printf ("PROXIMOS MANDATOS\n");
	for (i = 0; i < (int) dwContadorDeMandatos; i++)
		printf ("%s\n", strTabla[2][i]);
	printf ("\n");


}
