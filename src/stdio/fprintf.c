#include "../internal/internal.h"

int fprintf(FILE *stream, const char *fmt, ...)
{
	int ret = 0;
	va_list argptr;
	va_start(argptr, fmt);
	ret = __printf_inter(stream, NULL, 0, __printf_buffer, fmt, argptr);
	va_end(argptr);
	return ret;
}
