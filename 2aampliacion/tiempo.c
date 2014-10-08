#include <windows.h>

DWORD CalcularDiferenciaDeTiempos (SYSTEMTIME comienzo, SYSTEMTIME fin)
{

	DWORD dwTiemposEnMs[2];

	
	dwTiemposEnMs[1] = dwTiemposEnMs[0] = 0;
	
	/* calcular los tiempos en milisegundos */
	dwTiemposEnMs[0] += comienzo.wMilliseconds;
	dwTiemposEnMs[0] += 1000 * comienzo.wSecond;
	dwTiemposEnMs[0] += 1000 * 60 * comienzo.wMinute;
	dwTiemposEnMs[0] += 1000 * 60 * 60 * comienzo.wHour;

	dwTiemposEnMs[1] += fin.wMilliseconds;
	dwTiemposEnMs[1] += 1000 * fin.wSecond;
	dwTiemposEnMs[1] += 1000 * 60 * fin.wMinute;
	dwTiemposEnMs[1] += 1000 * 60 * 60 * fin.wHour;

	if (dwTiemposEnMs[0] > dwTiemposEnMs[1]) /* ha cambiado el dia */
		return 1000 * 60 * 60 * 24 - dwTiemposEnMs[0] + dwTiemposEnMs[1];
	else
		return dwTiemposEnMs[1] - dwTiemposEnMs[0];

}