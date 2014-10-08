#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <windows.h>
#include "sintactico.h"
#include "defines.h"
#include "ficheros.h"
#include "error.h"
#include "listas.h"
#include "vector.h"


void inicializar (void)
{
	
	FILE *pFichero;
	char cLinea[MAX_LINEA];
	DWORD dwNumeroDeLinea = 0, dwAux;
	CMD_t comando;
	HANDLE hRecron, hVercron;
	LPSTR strAux;
	int i, j;


	/* abrir fichero de configuracion */
	if ((pFichero = fopen (FicheroDeConfiguracion(), "r")) == NULL)
		ErrorCritico ("No se puede abrir el fichero de configuracion @ inicializar");
	
	while (fgets (cLinea, MAX_LINEA, pFichero) != NULL) /* leer fichero linea a linea */
	{
		
		dwNumeroDeLinea++;

		if ((dwAux = obtener_orden (cLinea, &comando)) == -1) 
			ErrorSintactico (dwNumeroDeLinea);
		else if (dwAux == 0) /* insertar linea en lista de ordenes a ejecutar */
		{	
			
			for (i = 0; comando.args[i][0]; i++)
			{
				
				/* comprobar si es orden interna */
				if (BuscarOrdenInterna(comando.args[i][0]))
				{
					
					/* contar los argumentos */
					for (j = 0; comando.args[i][j]; j++);

					
					/* mover los argumentos dos posiciones hacia atras */
					for (--j; j >= 0; j--)
						comando.args[i][j + 2] = comando.args[i][j];
					
					/* cambiar la orden a cmd /c para que ejecute las ordenes internas que
					   pasan a ser argumentos */
					comando.args[i][0] = "cmd";
					comando.args[i][1] = "/c";

				}

				/* comprobar si es un punto com */
				if (BuscarPuntoCom (comando.args[i][0]))
				{	
					
					/* añadir la extension .com a la orden */

					/* primero hay que reservar memoria para una cadena mas larga
					   (se utliza un puntero auxiliar) */
					strAux = (LPSTR) malloc ((strlen (comando.args[i][0] + 5)) * sizeof (char));
					strcpy (strAux, comando.args[i][0]);
					strcat (strAux, ".com");
					comando.args[i][0] = strAux;
				
				}
			
			}

			InsertarOrden (comando);

		}	
			
	}

	/* cerrar fichero de configuracion */
	fclose (pFichero);

	
	/* recron */
	
	/* crear directorio de recron. si ya existe, la funcion no hace nada */
	_mkdir (DirectorioDeRecron());
	
	/* vaciar fichero de recron */
	VaciarFicheroDeRecron();
	
	/* meter evento de recron en vector de handles */
	if (!(hRecron = FindFirstChangeNotification (DirectorioDeRecron(), FALSE, FILE_NOTIFY_CHANGE_SIZE)))
		ErrorCriticoAPI ("incializacion()");
	
	InsertarEventoDeRecron (hRecron);

	
	/* vercron */
	
	/* crear directorio de vercron. si ya existe, la funcion no hace nada */
	_mkdir (DirectorioDeVercron());

	/* vaciar fichero de vercron */
	VaciarFicheroDeVercron();
	
	/* meter evento de vercron en vector de handles */
	if (!(hVercron = FindFirstChangeNotification (DirectorioDeVercron(), FALSE, FILE_NOTIFY_CHANGE_SIZE)))
		ErrorCriticoAPI ("incializacion()");
	
	InsertarEventoDeVercron (hVercron);
	
}
