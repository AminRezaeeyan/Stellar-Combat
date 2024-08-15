#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

int initLogger(const char *filename);
void closeLogger();
void logError(const char *file, int line, const char *format, ...);

#define LOG_ERROR(format, ...) logError(__FILE__, __LINE__, format, ##__VA_ARGS__)

#endif // LOGGER_H
