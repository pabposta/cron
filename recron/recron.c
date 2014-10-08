#include <windows.h>
#include <string.h>
#include <stdio.h>


int main (int argc, char *argv[])
{

	LPSTR lpFicheroEstandar = "recron.tbo";
	LPSTR lpDirectorioEstandar = "."; 
	LPSTR lpFichero; /* nombre del fichero al que se escribe */
	LPSTR lpDirectorio; /* el directorio de cron */
	LPSTR lpRuta; /* la ruta completa */
	FILE *pFichero;
	SYSTEMTIME st;


	if (argc > 2)
		lpFichero = argv[2];
	else
		lpFichero = lpFicheroEstandar;

	if (argc > 1)
		lpDirectorio = argv[1];
	else
		lpDirectorio = lpDirectorioEstandar;

	if (!(lpRuta = (LPSTR) malloc (256 * sizeof (char)))) /* maximo de caracteres de una ruta */
	{

		printf ("%s: memoria insuficiente\n", argv[0]);
		return 1;

	}

	strcpy (lpRuta, lpDirectorio);
	strcat (lpRuta, "\\recron\\");
	strcat (lpRuta, lpFichero);

	/* al cambiar el tama�o de un fichero del directorio cron\recron se activa la se�al de 
	   recron en el servidor */

	if (!(pFichero = fopen (lpRuta, "a")))
	{

		printf ("%s: no se puede abrir el fichero de recron\n", argv[0]);
		return 1;

	}

	
	/* escribir la linea con la hora de recron al fichero */
	GetSystemTime (&st);
	fprintf (pFichero, "recron a las %d:%d:%d\n", st.wHour, st.wMinute, st.wSecond);

	fclose (pFichero);

}