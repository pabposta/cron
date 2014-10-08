#include <string.h>
#include "listas.h"
#include "estructuras.h"
#include "error.h"

/* las dos listas (la de ordenes a lanzar y la de procesos lanzados) son variables estaticas 
   y se crean en este fichero para mayor independencia */

static ORDEN *pListaDeOrdenes = NULL;
static PROCESO *pListaDeProcesos = NULL;



void InsertarOrden (CMD_t orden)
{

	ORDEN *pAnterior, *pSiguiente, *pAux;


	/* buscar posicion en la que meter la orden. la lista esta ordenada por el tiempo de 
	   proxima ejecucion */
	pSiguiente = pListaDeOrdenes;
	pAnterior = NULL;

	while ((pSiguiente != NULL) && (pSiguiente->dwProximaEjecucion < (DWORD) orden.time))
	{

		pAnterior = pSiguiente;
		pSiguiente = pSiguiente->pSiguiente;

	}

	/* reservar memoria para la estructura de la orden */
	if (!(pAux = (ORDEN *) malloc (sizeof (ORDEN))))
		ErrorCritico ("No se ha podido reservar memoria para la orden @ InsertarOrden()");

	/* copiar los campos a la estructura */
	pAux->dwProximaEjecucion = orden.time;
	
	/* copiar la estructura CMD_t campo a campo por contener punteros */
	pAux->comando = CopiarOrden (orden);
	
	/* ajustar los punteros */
	/* averiguar si se inserta en la primera posicion. al ser asi, pAnterior es NULL */
	if (pAnterior == NULL)
		pListaDeOrdenes = pAux;
	else
		pAnterior->pSiguiente = pAux;

	pAux->pSiguiente = pSiguiente;

}



void ReinsertarUltimaOrdenEjecutada (void)
{

	ORDEN *pAux;

	
	/* la orden mas reciente siempre es la apuntada por pListaDeOrdenes */
	if (pListaDeOrdenes != NULL)
	{
		
		/* insertar la orden en la lista, asi se busca sola su posicion y resetea su tiempo de
		   proxima ejecucion */
		InsertarOrden (*(pListaDeOrdenes->comando));

		/* borrar la orden previamente ejecutada */
		pAux = pListaDeOrdenes;
		pListaDeOrdenes = pListaDeOrdenes->pSiguiente;
		
		free (pAux);
		
	}

}



void VaciarListaDeOrdenes (void)
{

	ORDEN *pAux;
	

	while (pListaDeOrdenes != NULL) /* mientras haya elementos en la lista */
	{
		
		/* borrar siempre el primer elemento de la lista */
		pAux = pListaDeOrdenes;
		pListaDeOrdenes = pListaDeOrdenes->pSiguiente;
		free (pAux);

	}	

}



void ActualizarTiempos (DWORD dwDeltaTiempo)
{

	ORDEN *pAux = pListaDeOrdenes;
	LONG lAux; /* para comprobar que el tiempo de proxima ejecucion sea >= 0 */
	
	while (pAux != NULL)
	{

		lAux = pAux->dwProximaEjecucion - dwDeltaTiempo;
		
		if (lAux < 0)
			lAux = 0;
		
		pAux->dwProximaEjecucion = lAux;
		pAux = pAux->pSiguiente;

	}

}



CMD_t *SiguienteOrden (void)
{

	/* la siguiente orden siempre es la apuntada por pListaDeOrdenes */
	if (pListaDeOrdenes == NULL)
		ErrorCritico ("Intento de leer orden de una lista vacia @ SiguienteOrden()");
	
	return pListaDeOrdenes->comando;

}



DWORD SiguienteTiempoDeEjecucion (void)
{

	/* la siguiente orden, que tiene el proximo tiempo de ejecucion, siempre es la apuntada 
	   por pListaDeOrdenes */
	if (pListaDeOrdenes == NULL)
		ErrorCritico ("Intento de leer orden de una lista vacia @ SiguienteTiempoDeEjecucion()");
	
	return pListaDeOrdenes->dwProximaEjecucion;

}



BOOL ListaDeOrdenesVacia (void)
{

	return pListaDeOrdenes == NULL;

}



ORDEN *AvanzarYSacarOrden (void)
{

	static ORDEN *pOrden = NULL;
		
	if (pOrden == NULL)
		pOrden = pListaDeOrdenes;
	else
		pOrden = pOrden->pSiguiente;

	return pOrden;

}


/* * * * * */


void InsertarProceso (HANDLE hProceso, LPSTR strOrden)
{

	PROCESO *pProximo, *pAux;

	
	/* buscar el ultimo elemento */
	if ((pProximo = pListaDeProcesos) != NULL)
		while (pProximo->pSiguiente)
			pProximo = pProximo->pSiguiente;
	
	/* reserver memoria para el proceso */
	if (!(pAux = (PROCESO *) malloc (sizeof (PROCESO))))
		ErrorCritico ("Insuficiente memoria @ InsertarProceso()");

	/* copiar los valores a los campos */
	if (!(pAux->strComando = (LPSTR) malloc ((strlen (strOrden) + 1) * sizeof (char))))
		ErrorCritico ("Insuficiente memoria @ InsertarProceso()");

	strcpy (pAux->strComando, strOrden);
	pAux->hProceso = hProceso;

	/* ajustar los punteros */
	if (!pProximo) /* se inserta en la primera posicion */
	{	
		
		pAux->pSiguiente = NULL;
		pListaDeProcesos = pAux;
	
	}
	else
	{	
		
		pAux->pSiguiente = pProximo->pSiguiente;
		pProximo->pSiguiente = pAux;

	}

}



void BorrarProceso (HANDLE hProceso)
{

	PROCESO *pAux = pListaDeProcesos, *pAnterior = NULL;


	/* buscar el proceso a borrar */
	while (pAux && pAux->hProceso != hProceso)
	{	
		
		pAnterior = pAux;
		pAux = pAux->pSiguiente;

	}

	/* si no se encuentra, no hacer nada */
	if (!pAux)
	{}
	else
	{

		if (pAnterior == NULL) /* si se borra el primer proceso */
			pListaDeProcesos = pAux->pSiguiente;
		else
			pAnterior->pSiguiente = pAux->pSiguiente;
		
		free (pAux->strComando);
		free (pAux);

	}

}



LPSTR OrdenDelProceso (HANDLE hProceso)
{
	
	PROCESO *pAux = pListaDeProcesos;


	/* buscar el proceso */
	while (pAux && pAux->hProceso != hProceso)
		pAux = pAux->pSiguiente;

	if (pAux)
		return pAux->strComando;
	else	
		return NULL;

}
