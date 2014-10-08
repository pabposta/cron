//#ifndef __ERROR_H
//#define __ERROR_H


#include <windows.h>


void ErrorCritico (LPSTR strError);
void ErrorCriticoAPI (LPSTR strFuente);
void ErrorSintactico (DWORD dwNumeroDeLinea);


//#endif