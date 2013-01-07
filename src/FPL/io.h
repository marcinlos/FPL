/**
 * @file io.h
 */
#ifndef FPL_IO_H
#define FPL_IO_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Formatting function for FPL_float64.
 * TODO: Implement & write about flags
 */
char* FPL_sprintf(const char* fmt, char* buffer, FPL_float64 val);


#ifdef __cplusplus
}
#endif

#endif // FPL_IO_H
