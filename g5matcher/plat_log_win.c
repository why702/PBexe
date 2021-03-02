#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "plat_log.h"

#define MAX_BUFLEN 1024

#ifdef EGIS_DBG
LOG_LEVEL g_log_level = LOG_VERBOSE;
#else
LOG_LEVEL g_log_level = LOG_INFO;
#endif

void output_log(LOG_LEVEL level, const char *tag, const char *file_name,
		const char *func_name, int line, const char *format, ...)
{
	char buffer[MAX_BUFLEN];
	char out_buffer[MAX_BUFLEN];
	char *filename;
	va_list vl;

	if (format == NULL) return;
	if (g_log_level > level) return;

	va_start(vl, format);
	vsprintf_s(buffer, MAX_BUFLEN, format, vl);
	va_end(vl);
	filename = strrchr(file_name, '\\');
	filename = filename ? filename + 1 : file_name;

	if (level > LOG_INFO) {
		sprintf_s(out_buffer, MAX_BUFLEN, "ERROR! %s\t%s\t[%s:%d] %s\n", tag,
			func_name, filename, line, buffer);
	} else {
		sprintf_s(out_buffer, MAX_BUFLEN, "%s\t%s\t[%s:%d] %s\n", tag,
			func_name, filename, line, buffer);
	}
	OutputDebugString(out_buffer);
}
void output_algo_log(LOG_LEVEL level, const char *tag, const char *file_name,
	const char *func, int line, const char *format, ...)
{
	//char buffer[MAX_BUFLEN];
	//char out_buffer[MAX_BUFLEN];
	//char *filename;
	//va_list vl;
	//
	//if (format == NULL) return;
	//if (g_log_level > level) return;
	//
	//va_start(vl, format);
	//vsprintf_s(buffer, MAX_BUFLEN, format, vl);
	//va_end(vl);
	//filename = strrchr(file_name, '\\');
	//filename = filename ? filename + 1 : file_name;
	//
	//if (level > LOG_INFO) {
	//	sprintf_s(out_buffer, MAX_BUFLEN, "ERROR! %s\t%s\t[%s:%d] %s\n", tag,
	//		func, filename, line, buffer);
	//}
	//else {
	//	sprintf_s(out_buffer, MAX_BUFLEN, "%s\t%s\t[%s:%d] %s\n", tag,
	//		func, filename, line, buffer);
	//}
	//OutputDebugString(out_buffer);
}
