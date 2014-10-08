#define _POSIX_SOURCE
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexico.h"
#include "sintactico.h"

/* cabezeras anyadidas */
#include "error.h"
#include "defines.h"

#include <stdio.h>

#define ST_INIT 0
#define ST_ARGS 1
#define ST_FENT 2
#define ST_FSAL 3
#define ST_PIPE 4
#define ST_FRED 5
#define ST_ERR  6
#define ST_FERR 7 /* ANYADIDO */


static void reset_cmd(CMD_t *cmd) { memset(cmd, 0, sizeof(CMD_t));}
static int es_numero(char *str) {
   int i;
   int len=strlen(str);
   for (i=0; i<len; i++) if (!isdigit(str[i])) return 0;
   return 1;
}

/* 
   retorna 0 en caso de funcionar correctamente, -1 en caso de error .
   En caso de éxito, retorna la estructura "cmd" rellenada con la orden
   a ejecutar.
*/

int obtener_orden (char *s, CMD_t *cmd) {
   int pipecnt=-1; // Cuenta de tuberías
   int argcnt=0; // Cuenta de argumentos por tubería
   int cnt; // Cuenta de tokens
   int ntoks; // Número de tokens
   int estado=ST_INIT;

   
   /* ANYADIDO - ver si la linea es un comentario (empieza por #) */
   if (s[0] == '#')
	   return 1;

   reset_cmd(cmd);
   strcpy (cmd->buffer, s);
   trocea (cmd->buffer);
   ntoks = numero_de_tokens();

   for (cnt=0; cnt<ntoks && estado!=ST_ERR; cnt++) {
      switch (estado) {
		case ST_INIT:
			estado = ST_ERR;
			cmd->dwModo = !APPEND; /* ANYADIDO - inicializar en modo de sobrescritura; si se encuentran dos >>, se cambia a modo append */
			if (tipo_token(cnt) == TOK_NOMBRE) {
				if (es_numero(token_str(cnt))) {
					cmd->time=1000 * atoi(token_str(cnt)); /* ANYADIDO - hemos cambiado esta linea con "1000 *" para tener el tiempo en ms */
					estado = ST_PIPE;
				} 
			}
			break;

		case ST_ARGS:
			switch (tipo_token(cnt)) {
				case TOK_NOMBRE:
					cmd->args[pipecnt][++argcnt] = token_str(cnt);
					break;
				case TOK_MAYOR:
					estado=ST_FSAL;break;
				case TOK_MENOR:
					estado=ST_FENT;break;
				case TOK_TUBO:
					estado=ST_PIPE;break;
				case TOK_DOS: /* ANYADIDO - estado de redireccion de error */
					estado = ST_FERR;
					break;
				default:
					estado=ST_ERR;break;
			}
			break;

		case ST_PIPE:
			estado=ST_ERR;
			if (tipo_token(cnt) == TOK_NOMBRE) {
				cmd->args[++pipecnt][argcnt=0]=token_str(cnt);
				estado=ST_ARGS;
			} 
			break;

		case ST_FENT:
			estado=ST_ERR;
			if (tipo_token(cnt) == TOK_NOMBRE && !cmd->fent) {
				cmd->fent=token_str(cnt);
				estado = ST_FRED;
			}
			 break;

		case ST_FSAL:
			estado=ST_ERR;
	 
			if (tipo_token(cnt) == TOK_NOMBRE && !cmd->fsal) {
				cmd->fsal=token_str(cnt);
				estado = ST_FRED;
			} 
			/* ANYADIDO - para detectar el modo append (" >> ") */
			else if (tipo_token (cnt) == TOK_MAYOR)
			{

				estado = ST_FSAL;
				cmd->dwModo = APPEND;

			}
			break;

		case ST_FRED:
			estado=ST_ERR;
			if ((tipo_token(cnt) == TOK_MAYOR) && (!cmd->fsal)) estado=ST_FSAL;
			if ((tipo_token(cnt) == TOK_MENOR) && (!cmd->fent)) estado=ST_FENT;
			if ((tipo_token(cnt) == TOK_DOS) && (!cmd->ferr)) estado=ST_FERR;
			break;

		/* ANYADIDO - estado de redireccion de error */
		case ST_FERR:
			estado = ST_ERR;
			if (tipo_token(cnt) == TOK_NOMBRE && !cmd->ferr) {
				cmd->ferr=token_str(cnt);
				estado = ST_FRED;
			}
			break;
	  }
   }
   
   // Miro estados finales
   if (estado != ST_ARGS && estado != ST_FRED) estado = ST_ERR;

   return (estado==ST_ERR ? -1 : 0);
}


/* Copia todos los campos de la estructura uno a uno y devuelve la copia. */

CMD_t *CopiarOrden (CMD_t origen)
{

	CMD_t *destino;
	int i, j;
	
	if (!(destino = (CMD_t *) malloc (sizeof (CMD_t))))
		ErrorCritico ("Insuficiente memoria @ CopiarOrden()");

	destino->time = origen.time;
	
	if (origen.fent)
	{
		
		destino->fent = (char *) malloc ((strlen (origen.fent) + 1) * sizeof (char));
		strcpy (destino->fent, origen.fent);

	}
	else
		destino->fent = NULL;

	if (origen.fsal)
	{

		destino->fsal = (char *) malloc ((strlen (origen.fsal) + 1) * sizeof (char));
		strcpy (destino->fsal, origen.fsal);
		destino->dwModo = origen.dwModo;

	}
	else
		destino->fsal = NULL;
	
	if (origen.ferr)
	{

		destino->ferr = (char *) malloc ((strlen (origen.ferr) + 1) * sizeof (char));
		strcpy (destino->ferr, origen.ferr);
		
	}
	else
		destino->ferr = NULL;

	for (i = 0; origen.args[i][0]; i++)
	{	
		
		for (j = 0; origen.args[i][j]; j++)
		{
		
			if (!(destino->args[i][j] = (char *) malloc ((strlen (origen.args[i][j]) + 1 ) * sizeof (char))))
				ErrorCritico ("Insuficiente memoria @ CopiarOrden");

			strcpy (destino->args[i][j], origen.args[i][j]);

		}

		destino->args[i][j] = NULL;

	}

	destino->args[i][0] = NULL;

	/* buffer */
	strcpy (destino->buffer, origen.buffer);

	return destino;

}