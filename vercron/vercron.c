#include <stdio.h>
#include <string.h>
#include "aviso.h"
#include "protocolo.h"
#include "defines.h"
#include "error.h"
#include "tabla.h"



int main (int argc, char *argv[])
{
	
	DWORD dwTipoDeMensaje;
	char cBuffer[MAX_LINEA];
	
		
	/* avisar a cron de que se quiere hacer un vercron */
	if (AvisarCron (argc, argv))
		ErrorCritico ("no se puede abrir el fichero de vercron @ main()");

	AbrirTubo();

	/* recibir toda la información de estado del cron hasta que llegue el mensaje de cierre*/
	while ((dwTipoDeMensaje = RecibirCabezera()) != MSJ_CIERRE)
	{
		
		if (dwTipoDeMensaje == MSJ_CONFIGURACION)
		{
			
			AnyadirConfiguracion (RecibirCadena());

		}
		else if (dwTipoDeMensaje == MSJ_PROCESO)
		{

			itoa (RecibirEntero(), cBuffer, 10);
			strcat (cBuffer, " ");
			strcat (cBuffer, RecibirCadena());
			AnyadirProceso (cBuffer);

		}
		else if (dwTipoDeMensaje == MSJ_MANDATO)
		{

			AnyadirMandato (RecibirCadena());

		}
	
	}
	 
	CerrarTubo();

	/* sacar los resultados por pantalla */
	ImprimirTabla();

	return 0;

}