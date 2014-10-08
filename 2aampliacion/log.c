#include <windows.h>
#include <stdio.h>
#include "sintactico.h"
#include "ficheros.h"
#include "error.h"



void EscribirLog (int nCodigoDeSalida, SYSTEMTIME st, LPSTR strOrden)
{

	FILE *pFichero;
	

	/* abrir fichero */
	if ((pFichero = fopen (FicheroDeSalida(), "a")) == NULL)
		ErrorCritico ("No se puede abrir el fichero de salida @ inicializar");

	/* escribir la linea al fichero */
	fprintf (pFichero, "%d:%d:%d\t%s\tTerm: %d\n", st.wHour, st.wMinute, st.wSecond, 
	         strOrden, nCodigoDeSalida);

	/* cerrar el fichero */
	fclose (pFichero);

}