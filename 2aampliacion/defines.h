#define MAX_LINEA 1000 /* tamaño maximmo de una linea del fichero de configuracion */
#define MAX_PROCESOS 1000 /* numero maximo de procesos ejecutandose al mismo tiempo */

#define APPEND 1 /* para distinguir entre los dos tipos de redireccion (el otro es !APPEND) */

/* definicion de los tipos de mensaje del protocolo */
#define MSJ_CONFIGURACION 1
#define MSJ_PROCESO 2
#define MSJ_MANDATO 3
#define MSJ_CIERRE 4