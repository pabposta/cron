#ifndef __ESTRUCTURAS_H
#define __ESTRUCTURAS_H

#include <windows.h>


/* estructura de las ordenes a lanzar */
typedef struct STRUCT_ORDEN
{

	DWORD dwProximaEjecucion; /* tiempo hasta la proxima ejecucion de la orden */
	CMD_t *comando; /* la cadena o estructura que contiene la orden o puntero a ella */
	struct STRUCT_ORDEN *pSiguiente; /* puntero a la siguiente orden */

} ORDEN;


/* estructura de procesos lanzados */
typedef struct STRUCT_PROCESO
{

	HANDLE hProceso; /* descriptor del proceso */
	LPSTR strComando; /* la cadena o estructura que contiene la orden o puntero a ella */
	struct STRUCT_PROCESO *pSiguiente; /* puntero al siguiente proceso */


} PROCESO;


#endif
