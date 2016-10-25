#ifndef __UPRINTF_H__
#define __UPRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

int vfuprintf(FILE *stream, const char *format, va_list args);
int fuprintf(FILE *stream, const char *format, ...);
int vuprintf(const char *format, va_list args);
int uprintf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __UPRINTF_H__ */
