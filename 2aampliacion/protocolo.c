#include <windows.h>
#include "error.h"
#include "defines.h"


static HANDLE hTubo;


void MandarCabezera (DWORD dwTipoDeMensaje, DWORD dwLongitudDeMensaje)
{

	DWORD dwBytesEscritos;
	

	/* mandar el tipo de mensaje */
	WriteFile (hTubo, (LPVOID) &dwTipoDeMensaje, sizeof (DWORD), &dwBytesEscritos, NULL);

	/* mandar la longitud del mensaje */
	WriteFile (hTubo, (LPVOID) &dwLongitudDeMensaje, sizeof (DWORD), &dwBytesEscritos, NULL);

}



void MandarEntero (DWORD dwEntero)
{

	DWORD dwBytesEscritos;


	WriteFile (hTubo, (LPVOID) &dwEntero, sizeof (DWORD), &dwBytesEscritos, NULL);

}



void MandarCadena (LPSTR strCadena)
{

	DWORD dwBytesEscritos;


	WriteFile (hTubo, (LPVOID) strCadena, strlen (strCadena), &dwBytesEscritos, NULL);

}



void AbrirTubo (void)
{

	/* crear el tubo */
	if ((hTubo = CreateNamedPipe ("\\\\.\\pipe\\vercron", PIPE_ACCESS_OUTBOUND, 
		                           PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		                           10, 0, 0, INFINITE,
								   NULL)) == INVALID_HANDLE_VALUE)
		ErrorCriticoAPI ("CreateNamedPipe()");
	
	/* esperar la conexion del cliente vercron */
	if (!ConnectNamedPipe (hTubo, NULL))
		ErrorCriticoAPI ("ConnectNamedPipe()");

}



void CerrarTubo (void)
{

	/* mandar señal de cierre */
	MandarCabezera (MSJ_CIERRE, 0);

	/* vaciar los buffers del tubo */
	FlushFileBuffers (hTubo);
	
	/* desconectar */
	DisconnectNamedPipe (hTubo);

	/* cerrar el handle */
	CloseHandle (hTubo);

}
