#include <windows.h>
#include <string.h>

#include "error.h"
#include "defines.h"


HANDLE hTubo;
static DWORD dwLongitudDeMensaje;


DWORD RecibirCabezera (void)
{

	DWORD dwBytesEscritos;
	DWORD dwTipoDeMensaje;

	/* recibir el tipo de mensaje */
	ReadFile (hTubo, (LPVOID) &dwTipoDeMensaje, sizeof (DWORD), &dwBytesEscritos, NULL);

	/* recibir la longitud del mensaje */
	ReadFile (hTubo, (LPVOID) &dwLongitudDeMensaje, sizeof (DWORD), &dwBytesEscritos, NULL);

	return dwTipoDeMensaje;

}



DWORD RecibirEntero (void)
{

	DWORD dwBytesEscritos;
	DWORD dwEntero;


	ReadFile (hTubo, (LPVOID) &dwEntero, sizeof (DWORD), &dwBytesEscritos, NULL);

	return dwEntero;

}



LPSTR RecibirCadena (void)
{

	DWORD dwBytesEscritos;
	LPSTR strCadena;


	if (!(strCadena = (LPSTR) malloc (dwLongitudDeMensaje + 1)))
		ErrorCritico ("memoria insuficiente @ RecibirCadena");
	
	ReadFile (hTubo, (LPVOID) strCadena, dwLongitudDeMensaje, &dwBytesEscritos, NULL);
	strCadena[dwLongitudDeMensaje] = '\0';

	return strCadena;

}



void AbrirTubo (void)
{

	/* esperar a que se abra el tubo */
	while (!WaitNamedPipe ("\\\\.\\pipe\\vercron", NMPWAIT_WAIT_FOREVER));
	
	/* abrir el tubo */
	if (!(hTubo = CreateFile ("\\\\.\\pipe\\vercron", GENERIC_READ, 0, NULL, OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL, NULL)))
		ErrorCriticoAPI ("CreateFile()");	
	
}



void CerrarTubo (void)
{

	/* cerrar el handle */
	CloseHandle (hTubo);

}
