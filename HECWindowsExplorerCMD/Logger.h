
#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LogDebug(fmt, ...) LogWrite(_T("[D][" LOGTAG "]" fmt), ##__VA_ARGS__)
#define LogError(fmt, ...) LogWrite(_T("<E>[" LOGTAG "]<%s(%d)>" fmt), (__FUNCTION__), __LINE__, ##__VA_ARGS__)

extern int LogWrite(const TCHAR* fmt, ...);
extern int FileWrite(char* func, int line);
#ifdef __cplusplus
}
#endif
#endif
