#include <stdio.h>

int scanf(const char* fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	int rc = vscanf(fmt, v);
	va_end(v);
	return rc;
}
