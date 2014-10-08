#include <windows.h>
#include <stdio.h>


void ErrorCritico (LPSTR strError)
{

	printf ("cron: error critico - %s\n", strError);

	ExitProcess (1);

}



void ErrorCriticoAPI (LPSTR strFuente)
{

	/* codigo de la ayuda sobre FormatMessage() */
	LPVOID lpMsgBuf;

	
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL);

	/* imprimir error */
	printf ("cron: error critico en %s - %s", strFuente,lpMsgBuf);

	ExitProcess (1);

}



void ErrorSintactico (DWORD dwNumeroDeLinea)
{

	printf ("cron: error sintactico en linea %d\n", dwNumeroDeLinea);

}