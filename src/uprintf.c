#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

#include "uprintf.h"

#ifdef _MSC_VER
#define inline __inline
#endif

static inline int _vfuprintf(FILE *stream, const char *format, va_list args)
{
#if defined(_WIN32) || defined(WIN32)
  int rc;
  wchar_t *wfmt = NULL;

  if (stream == stdout || stream == stderr) {
    if (_isatty(_fileno(stream))) {
      _setmode(_fileno(stream), _O_U16TEXT);
      if (errno) {
        rc = errno;
        goto error;
      }
      int size = MultiByteToWideChar(CP_UTF8, 0, format, -1, NULL, 0);
      if (size == 0) {
        rc = GetLastError();
        goto error;
      }
      wfmt = malloc(sizeof(wchar_t) * size);
      if (!wfmt) {
        rc = errno;
        goto error;
      }
      size = MultiByteToWideChar(CP_UTF8, 0, format, -1, wfmt, size);
      if (size == 0) {
        rc = GetLastError();
        goto error;
      }
      rc = vfwprintf(stream, wfmt, args);
      free(wfmt);

      // The following is tested and not work. Don't know why.
      // _setmode(_fileno(stream), _O_U8TEXT);
      // rc = vfprintf(stream, format, args);
    } else {
      _setmode(_fileno(stream), _O_BINARY);
      rc = vfprintf(stream, format, args);
    }
  } else {
    rc = vfprintf(stream, format, args);
  }

  return rc;

error:
  assert(rc != 0);
  if (rc > 0)
    rc = -rc;
  if (wfmt)
    free(wfmt);
  return rc;
#else
  return vfprintf(stream, format, args);
#endif
}

static inline int _vuprintf(const char *format, va_list args)
{
  return _vfuprintf(stdout, format, args);
}

int vfuprintf(FILE *stream, const char *format, va_list args)
{
  return _vfuprintf(stream, format, args);
}

int fuprintf(FILE *stream, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  int rc = _vfuprintf(stream, format, args);
  va_end(args);
  return rc;
}

int vuprintf(const char *format, va_list args)
{
  return _vuprintf(format, args);
}

int uprintf(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  int rc = _vuprintf(format, args);
  va_end(args);
  return rc;
}
