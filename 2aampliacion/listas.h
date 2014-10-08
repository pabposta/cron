#include <windows.h>
#include "sintactico.h"
#include "estructuras.h"


/* lista de ordenes a lanzar */
/* esta lista esta ordenada por el tiempo de la proxima ejecucion, siendo la siguiente orden a 
   ejecutar el primer elemento
   cada elemento tiene un puntero al siguiente y el ultimo a NULL
   ademas la lista guarda su numero de elementos
*/
void InsertarOrden (CMD_t orden);
void ReinsertarUltimaOrdenEjecutada (void);
void VaciarListaDeOrdenes (void);
void ActualizarTiempos (DWORD dwDeltaTiempo);
CMD_t *SiguienteOrden (void);
DWORD SiguienteTiempoDeEjecucion (void);
BOOL ListaDeOrdenesVacia (void);
ORDEN *AvanzarYSacarOrden (void);

/* lista de ordenes lanzadas */
/* la lista esta ordenada por el handle de los procesos, siendo el menor el primero
   cada elemento tiene un puntero al siguiente y el ultimo a NULL
   ademas la lista guarda su numero de elementos
*/
void InsertarProceso (HANDLE hProceso, LPSTR strOrden);
void BorrarProceso (HANDLE hProceso);
LPSTR OrdenDelProceso (HANDLE hProceso);