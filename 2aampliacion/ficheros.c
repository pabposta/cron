#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "defines.h"



static LPSTR strFicheroDeConfiguracion;
static LPSTR strFicheroDeSalida;




LPSTR FicheroDeConfiguracion (void)
{

	return strFicheroDeConfiguracion;

}



LPSTR FicheroDeSalida (void)
{

	return strFicheroDeSalida;

}



LPSTR DirectorioDeRecron (void)
{ 

	return ".\\recron";

}



LPSTR DirectorioDeVercron (void)
{

	return ".\\vercron";

}



void GuardarFicheroDeConfiguracion (LPSTR strFicheroDeConfiguracionArg)
{

	strFicheroDeConfiguracion = strFicheroDeConfiguracionArg;

}



void GuardarFicheroDeSalida (LPSTR strFicheroDeSalidaArg)
{

	strFicheroDeSalida = strFicheroDeSalidaArg;

}



void VaciarFicheroDeSalida (void)
{

	FILE *pFichero;


	if (!(pFichero = fopen (FicheroDeSalida(), "w")))
		ErrorCritico ("No se puede abrir el fichero de salida @ VaciarFicheroDeSalida()");

	fclose (pFichero);

}



void VaciarFicheroDeRecron (void)
{

	FILE *pFichero;
	LPSTR lpRuta;
	
	
	if (!(lpRuta = (LPSTR) malloc (256 * sizeof (char)))) /* maximo de caracteres de una ruta */
		ErrorCritico ("Memoria insuficiente @ VaciarFicheroDeRecron()");

	strcpy (lpRuta, DirectorioDeRecron());
	strcat (lpRuta, "\\recron.tbo");

	if (!(pFichero = fopen (lpRuta, "w")))
		ErrorCritico ("No se puede abrir el fichero de vercron @ VaciarFicheroDeRecron()");
		
	fclose (pFichero);

}



void VaciarFicheroDeVercron (void)
{

	FILE *pFichero;
	LPSTR lpRuta;
	
	
	if (!(lpRuta = (LPSTR) malloc (256 * sizeof (char)))) /* maximo de caracteres de una ruta */
		ErrorCritico ("Memoria insuficiente @ VaciarFicheroDeVercron()");

	strcpy (lpRuta, DirectorioDeVercron());
	strcat (lpRuta, "\\vercron.tbo");

	if (!(pFichero = fopen (lpRuta, "w")))
		ErrorCritico ("No se puede abrir el fichero de vercron @ VaciarFicheroDeVercron()");
		
	fclose (pFichero);

}



/* Busca en el directorio de Windows\System32\ si es un fichero con extensión .com */

BOOL BuscarPuntoCom (LPSTR strOrden)
{

	LPSTR strRuta;
	WIN32_FIND_DATA w32fd;

	
	if (!(strRuta = (LPSTR) malloc (sizeof (char) * (MAX_PATH + 1))))
		ErrorCritico ("Insuficiente memoria @ BuscarPuntoCom()");
		
	strcpy (strRuta, "c:\\Windows\\System32\\");
	strcat (strRuta, strOrden);
	strcat (strRuta, ".com");

	if (FindFirstFile (strRuta, &w32fd) == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;

}


/* Comprueba si es una orden interna */

BOOL BuscarOrdenInterna (LPSTR strOrden)
{

	FILE *pFichero;
	char cLinea[MAX_LINEA + 1];


	if (!(pFichero = fopen ("ordenes_internas.txt", "r")))
		ErrorCritico ("No se puede abrir el fichero de ordenes internas @ BuscarOrdenInterna()");

	while (fgets (cLinea, MAX_LINEA, pFichero)) // comprueba con cada orden del fichero
	{	
		
		cLinea[strlen (cLinea) - 1] = '\0';
		if (!strcmp (cLinea, strOrden))
		{

			fclose (pFichero);
			return TRUE;

		}

	}

	fclose (pFichero);
	return FALSE;

}
