#include <stdio.h>
#include <windows.h>
#include "listas.h"
#include "defines.h"
#include "vector.h"
#include "error.h"
#include "inicializacion.h"
#include "log.h"
#include "tiempo.h"
#include "redirecciones.h"
#include "protocolo.h"
#include "ficheros.h"


void LanzarProceso (void);
void Recron (void);
void Vercron (void);
void ProcesoTerminado (HANDLE hProceso);



void principal (void)
{

	DWORD dwRetornoWFMO, dwTiempoWFMO, dwTiempoDeEspera;
	SYSTEMTIME st, st2;
				
	/* imprimir mensaje de bienvenida */
	printf ("Bienvenido a cron - 2a ampliacion\n");
	printf ("***********************************\n");

	while (1) /* bucle infinito */
	{
		
		/* este es el bucle de espera a eventos (que pueden ser final de proceso, timeout,
		   recron, vercron o error)
		   durante la espera, el programa no consume cpu */
		/* medir tiempo ... */
		GetSystemTime (&st);

		/* mirar si es lista vacia, si es asi esperar ilimitadamente hasta recron */
		if (ListaDeOrdenesVacia())
			dwTiempoDeEspera = INFINITE;
		else
			dwTiempoDeEspera = SiguienteTiempoDeEjecucion();

		dwRetornoWFMO = WaitForMultipleObjects (ObtenerNumeroDeHandles(),
			                                    ObtenerVector(),
												FALSE,
												dwTiempoDeEspera);
				
		GetSystemTime (&st2);
		dwTiempoWFMO = CalcularDiferenciaDeTiempos (st, st2);
		/* ... alrededor de WFMO() */
		/* no es el punto mas exacto para medir, pero el mas claro y decentemente exacto */

		
		/* evaluar el valor de retorno de WFMO() */
		
		/* ha pasado el tiempo para que se lanze el siguiente proceso */
		if (dwRetornoWFMO == WAIT_TIMEOUT)
		{	
			
			LanzarProceso();
			
			/* actualizar los tiempos de proxima ejecucion de todos los procesos */
			ActualizarTiempos (dwTiempoWFMO);
			
			/* Resetear el tiempo hasta la proxima ejecucion de la orden lanzada y moverla a su
			   posicion correspondiente en la cola */
			ReinsertarUltimaOrdenEjecutada();

		}
		/* señal de recron */
		else if (dwRetornoWFMO == WAIT_OBJECT_0)
		{
			
			Recron();

		}
		/* señal de vercron */
		else if (dwRetornoWFMO == WAIT_OBJECT_0 + 1)
		{
			
			Vercron();
			ActualizarTiempos (dwTiempoWFMO);

		}
		/* ha finalizado un proceso lanzado */
		else if (dwRetornoWFMO > WAIT_OBJECT_0 + 1 && dwRetornoWFMO < WAIT_OBJECT_0 + 
		         ObtenerNumeroDeHandles())
		{
			
			ProcesoTerminado (ObtenerHandle (dwRetornoWFMO - WAIT_OBJECT_0));
			ActualizarTiempos (dwTiempoWFMO);

		}
		/* ha habido un error */
		else
		{
			
			ErrorCriticoAPI ("WaitForMultipleObjects()");

		}		

	}

}



void LanzarProceso (void)
{

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE **phMatriz;
	char cBuffer[MAX_LINEA];
	int i, j;
	
	/* crear la matriz con las redirecciones y tubos para cada proceso */
	phMatriz = CrearMatrizDeRedirecciones();

	/* para cada orden de la linea... */
	for (i = 0; SiguienteOrden()->args[i][0]; i++)
	{
			
		/* redirigir entrada y salida */
		GetStartupInfo (&si);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = phMatriz[i][0];
		si.hStdOutput = phMatriz[i][1];
		si.hStdError = phMatriz[i][2];
				
		/* construir buffer con la orden y sus argumentos */
		cBuffer[0] = '\0';
		for (j = 0; SiguienteOrden()->args[i][j]; j++)
		{	
	
			strcat (cBuffer, SiguienteOrden()->args[i][j]);
			strcat (cBuffer, " ");
		
		}

		/* crear el proceso y comprobar que haya funcionado */
		if (!CreateProcess (NULL, cBuffer, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
			ErrorCriticoAPI ("CreateProcess()");
			
		/* Cerrar los handles utilizados para el hijo */
		CerrarVectorDeHandles (phMatriz[i]);
				
		/* Agregar proceso a lista de procesos lanzados */
		InsertarProceso (pi.hProcess, cBuffer);

		/* Agregar handle al vector de handles */
		InsertarHandle (pi.hProcess);

	}

	/* liberar la memoria aun reservada en la matriz de handles */
	CerrarMatrizDeHandles (phMatriz);

	

}



void Recron (void)
{

	/* cerrar directorio */
	FindCloseChangeNotification (ObtenerHandle (WAIT_OBJECT_0));
	CloseHandle (ObtenerHandle (WAIT_OBJECT_0));
			
	/* vaciar lista */
	VaciarListaDeOrdenes();

	/* inicializar */
	inicializar();

}



void Vercron (void)
{

	HANDLE hVercron;
	ORDEN *pAux;
	char cBuffer[MAX_LINEA];
	int i, j;


	/* resetear evento de vercron */
		
	/* cerrar directorio */
	FindCloseChangeNotification (ObtenerHandle (WAIT_OBJECT_0 + 1));
	CloseHandle (ObtenerHandle (WAIT_OBJECT_0 + 1));
			
	/* volver a abrirlo */
	if (!(hVercron = FindFirstChangeNotification (DirectorioDeVercron(), FALSE, FILE_NOTIFY_CHANGE_SIZE)))
		ErrorCriticoAPI ("No se puede abrir el directorio de vercron @ Vercron()");
			
	InsertarEventoDeVercron (hVercron);

	
	/* abrir el tubo */
	AbrirTubo();
			
	
	/* reunir informacion y pasar a vercron */
		
	/* nombre del fichero de configuracion */
	MandarCabezera (MSJ_CONFIGURACION, strlen (FicheroDeConfiguracion()));
	MandarCadena (FicheroDeConfiguracion());			
		

	/* pid y orden de los procesos lanzados */
	
	/* se llegua a todos los procesos recorriendo el vector de handles de los
	   procesos lanzados */
	/* las dos primeras posiciones las ocupan recron y vercron */
	for (i = 2; i < (int) ObtenerNumeroDeHandles(); i++)
	{
	
		MandarCabezera (MSJ_PROCESO, strlen (OrdenDelProceso (ObtenerHandle (i))));
		MandarEntero ((DWORD) ObtenerHandle (i));
		MandarCadena (OrdenDelProceso (ObtenerHandle (i)));

	}
	

	/* segundos hasta la proxima ejecucion y mandato de las ordenes a ejecutar */
	/* los mandatos a ejecutar estan en la lista de ordenes a ejecutar */
	while (pAux = AvanzarYSacarOrden())
	{
			
		/* montar la linea con la orden */
		itoa (pAux->dwProximaEjecucion / 1000, cBuffer, 10);
		for (i = 0; pAux->comando->args[i][0]; i++)	
			for (j = 0; pAux->comando->args[i][j]; j++)
			{	
	
				strcat (cBuffer, " ");
				
				/* si hay una tuberia, poner el simbolo de tuberia */
				if (i > 0 && j == 0)
					strcat (cBuffer, "| ");

				strcat (cBuffer, pAux->comando->args[i][j]);

			}
		
		/* poner las redirecciones, si hay */
		if (pAux->comando->fent)
		{	
			
			strcat (cBuffer, " < ");
			strcat (cBuffer, pAux->comando->fent);

		}
		
		if (pAux->comando->fsal)
		{	
			
			if (pAux->comando->dwModo == APPEND)
				strcat (cBuffer, " >> ");
			else
				strcat (cBuffer, " > ");
			
			strcat (cBuffer, pAux->comando->fsal);

		}
			
		if (pAux->comando->ferr)
		{	
			
			strcat (cBuffer, " 2> ");
			strcat (cBuffer, pAux->comando->ferr);

		}

		/* mandar la linea */
		MandarCabezera (MSJ_MANDATO, strlen (cBuffer));
		MandarCadena (cBuffer);
	
	}
			

	/* cerrar el tubo */
	CerrarTubo();
			
}



void ProcesoTerminado (HANDLE hProceso)
{

	SYSTEMTIME st;
	LPSTR strOrden;
	DWORD dwCodigoDeSalida;

			
	/* reunir informacion para log */
	/* codigo de salida, hora de salida, orden */
	GetExitCodeProcess (hProceso, &dwCodigoDeSalida);
	GetSystemTime (&st);
	strOrden = OrdenDelProceso (hProceso);
					
	/* escribir log */
	EscribirLog (dwCodigoDeSalida, st, strOrden);

	BorrarProceso (hProceso);
			
	/* cerrar todos los handles si no es que ya lo estan */
	CloseHandle (hProceso);

	/* borrar proceso de vector de handles */
	BorrarHandle (hProceso);

}
