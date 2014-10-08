#include <windows.h>
#include "listas.h"
#include "error.h"
#include "defines.h"



HANDLE **CrearMatrizDeRedirecciones (void)
{

	HANDLE **phMatriz;
	DWORD dwNumeroDeOrdenes = 0;
	SECURITY_ATTRIBUTES sa = {sizeof (SECURITY_ATTRIBUTES), NULL, TRUE};
	int i;
	
	
	/* contar el numero de ordenes que hay en la linea */
	for (i = 0; SiguienteOrden()->args[i][0]; i++)
		dwNumeroDeOrdenes++;
	
	/* reserver memoria para la matriz */
	if (!(phMatriz = (HANDLE **) malloc (dwNumeroDeOrdenes * sizeof (LPHANDLE))))
		ErrorCritico ("insuficiente memoria @ CrearMatrizDeRedirecciones()");
	for (i = 0; i < (LONG) dwNumeroDeOrdenes; i++)
		/* cada orden tiene un handle para entrada, uno para salida y otro de error */
		if (!(phMatriz[i] = (LPHANDLE) malloc (3 * sizeof (HANDLE))))
			ErrorCritico ("insuficiente memoria @ CrearMatrizDeRedirecciones()");

	/* redireccionar entrada si hay fichero de entrada */
	if (SiguienteOrden()->fent)
	{
		
		if (!(phMatriz[0][0] = CreateFile (SiguienteOrden()->fent, GENERIC_READ,
		    FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, NULL)))
			ErrorCriticoAPI ("CreateFile()");
		
	}
	else /* si no hay fichero de entrada, asignar entrada estandar */
		phMatriz[0][0] = GetStdHandle (STD_INPUT_HANDLE);
	
	/* crear las tuberias */
	for (i = 0; i < (int) dwNumeroDeOrdenes - 1; i++)
		if (!CreatePipe (&phMatriz[i + 1][0], &phMatriz[i][1], &sa, 0))
			ErrorCriticoAPI ("CrearMatrizDeRedirecciones()");
	
	/* redireccionar salida si hay fichero de salida */
	if (SiguienteOrden()->fsal)
	{	
		
		/* ver si esta en modo de append o de sobrescritura */
		if (SiguienteOrden()->dwModo != APPEND) /* sobrescritura */
		{	
			
			if (!(phMatriz[dwNumeroDeOrdenes - 1][1] = CreateFile (SiguienteOrden()->fsal,
				GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, &sa, CREATE_ALWAYS, 0, NULL)))
				ErrorCriticoAPI ("CreateFile()");

		}
		else /* append */
		{
			
			if (!(phMatriz[dwNumeroDeOrdenes - 1][1] = CreateFile (SiguienteOrden()->fsal,
				GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, &sa, OPEN_ALWAYS, 0, NULL)))
				ErrorCriticoAPI ("CreateFile()");

			SetFilePointer (phMatriz[dwNumeroDeOrdenes - 1][1], 0, 0, FILE_END);
		
		}

	}
	else /* si no hay fichero de salida, asignar salida estandar */
		phMatriz[dwNumeroDeOrdenes - 1][1] = GetStdHandle (STD_OUTPUT_HANDLE);

	/* redireccionar salida de error si hay fichero de error */
	if (SiguienteOrden()->ferr)
	{
		
		if (!(phMatriz[dwNumeroDeOrdenes - 1][2] = CreateFile (SiguienteOrden()->ferr,
			GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, &sa, CREATE_ALWAYS, 0, NULL)))
			ErrorCriticoAPI ("CreateFile()");
		
	}
	else /* si no hay fichero de error, asignar salida de error	estandar */
		phMatriz[dwNumeroDeOrdenes - 1][2] = GetStdHandle (STD_ERROR_HANDLE);
	
		
	return phMatriz;

}



void CerrarMatrizDeHandles (HANDLE **phMatriz)
{

	/* liberar la memoria */
	free (phMatriz);

}



void CerrarVectorDeHandles (LPHANDLE lpVector)
{
	
	/* cada orden tiene un handle para entrada, uno para salida y uno de error */
	if (lpVector[0] != GetStdHandle (STD_INPUT_HANDLE))
		CloseHandle (lpVector[0]);
		
	if (lpVector[1] != GetStdHandle (STD_OUTPUT_HANDLE))
		CloseHandle (lpVector[1]);

	if (lpVector[2] != GetStdHandle (STD_ERROR_HANDLE))
		CloseHandle (lpVector[2]);

		
	/* liberar el vector, ya que ya no se va a usar */
	free (lpVector);

}
