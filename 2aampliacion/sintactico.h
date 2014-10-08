#ifndef __SINTACTICO_H
#define __SINTACTICO_H

#include <windows.h>

#define MAX_LINE_LEN 4096
#define MAX_PIPES 50
#define MAX_ARGS 100

typedef struct {
   int time;
   char buffer[MAX_LINE_LEN];
   char *args[MAX_PIPES][MAX_ARGS];
   char *fent;
   char *fsal;
   char *ferr; /* ANYADIDO - fichero de redireccion de error */
   DWORD dwModo; /* ANYADIDO - modo append o sobrescritura */
} CMD_t;

int obtener_orden (char *s, CMD_t *cmd);
CMD_t *CopiarOrden (CMD_t origen);

#endif


