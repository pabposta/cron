#include "error.h"
#include "ficheros.h"
#include "inicializacion.h"
#include "principal.h"


int main (int argc, char *argv[])
{

	/* comprobar que esten los argumentos */
	if (argc != 3)
		ErrorCritico ("Uso: cron [fichero de configuracion] [fichero de salida]");

	/* asignar los ficheros */
	GuardarFicheroDeConfiguracion (argv[1]);
	GuardarFicheroDeSalida (argv[2]);
	/* vaciar el fichero de salida */
	VaciarFicheroDeSalida();

	/* inicializar la lista de ordenes a lanzar y la señal de recron */
	inicializar();

	/* bucle principal infinito */
	principal();

	return 0;

}