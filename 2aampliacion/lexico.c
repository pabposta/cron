#define _POSIX_SOURCE
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"


/* Este m�dulo s�lo deber�a ser usado por el m�dulo l�xico */

/* Variables y funciones PRIVADAS al m�dulo l�xico */

static token_t token[MAX_TOKENS]; 
static char *nombre[MAX_TOKENS]; 
static int num_tokens=0;

static char especial[]="<>|";

static int separador(char c) {
   if (strchr(especial,c) != NULL) return 1;
   else return 0;
}

/* Funciones p�blicas */
/* solo tiene sentido usarlas desde un analizador sint�ctico */

int trocea (char *s) {
   char c;
   int i=0;
   int len=strlen(s);

   num_tokens = 0;
   while (isspace(s[i])) i++;
  
   c=s[i];
   while (i<len) {

      if ( num_tokens == MAX_TOKENS ) {
	 /* El proximo token ya no cabe. Error */
	 num_tokens = 0;
	 return -1;
      }
      
      switch (c) {
      case '|':
	 token[num_tokens] = TOK_TUBO; c=s[++i];
	 break;
      case '>':
	 token[num_tokens] = TOK_MAYOR; c=s[++i];
	 break;
      case '<': 
	 token[num_tokens] = TOK_MENOR; c=s[++i];
	 break;
      /* ANYADIDO - para redireccion de error */
	  case '2':
		  if (s[i + 1] == '>') /* reconocer secuencia 2> */
		  {

			  token[num_tokens] = TOK_DOS;
			  i = i + 2; /* nos saltamos el > para no tener un TOK_MAYOR de sobra */
			  break;

		  } 
	  /* si el dos fuese argumento, al no haber break, se convierte en TOK_NOMBRE */
	  /* IMPORTANTE, no se debe anyadir nada entre estos dos casos */
	  default: 
	 token[num_tokens] = TOK_NOMBRE;
	 nombre[num_tokens] = &s[i++];
	 while (!separador(s[i]) && !isspace(s[i])) i++;
	 c=s[i];
	 s[i] = '\0';
	 if (!separador(c)) c=s[++i];
      }
      while (isspace(s[i])) c=s[++i];
      num_tokens++;
   }
   return 0;
}

token_t tipo_token (int i) {
  if ( i >= 0 && i < num_tokens) {
    return token[i];
  } else {
    return -1;
  }
}

char * token_str (int i) {
  if (i >= 0 && i < num_tokens) {
    return nombre[i];
  } else {
    return NULL;
  }
}

int numero_de_tokens() { return num_tokens;}
