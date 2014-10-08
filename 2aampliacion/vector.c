#include <windows.h>
#include "defines.h"


static HANDLE hVectorDeHandles[MAX_PROCESOS];
static DWORD dwNumeroDeHandles = 2;


void InsertarHandle (HANDLE hProceso)
{

	if (dwNumeroDeHandles < MAX_PROCESOS)
	{
		
		hVectorDeHandles[dwNumeroDeHandles] = hProceso;
		dwNumeroDeHandles++;

	}

}



void InsertarEventoDeRecron (HANDLE hRecron)
{

	hVectorDeHandles[0] = hRecron;

}



void InsertarEventoDeVercron (HANDLE hVercron)
{

	hVectorDeHandles[1] = hVercron;

}



void BorrarHandle (HANDLE hProceso)
{

	int i = 0;
	LONG lPosicion = -1;

	
	/* buscar la posicion */
	while (i < MAX_PROCESOS && lPosicion == -1)
	{

		if (hVectorDeHandles[i] == hProceso)
			lPosicion = i;
		i++;
	
	}
	
	/* si no se encontrua el handle, no hacer nada */
	if (lPosicion != -1)
	{	
		
		/* si si, mover los handles que estan detras en el vector una posicion hacia delante */
		for (; (DWORD) i < dwNumeroDeHandles; i++) /* i ya tiene el valor correcto */
			hVectorDeHandles[i - 1] = hVectorDeHandles[i];
		dwNumeroDeHandles--;
	
	}

}



HANDLE ObtenerHandle (DWORD dwPosicionEnVector)
{

	return hVectorDeHandles[dwPosicionEnVector];

}



LPHANDLE ObtenerVector (void)
{

	return hVectorDeHandles;

}



DWORD ObtenerNumeroDeHandles (void)
{

	return dwNumeroDeHandles;

}
