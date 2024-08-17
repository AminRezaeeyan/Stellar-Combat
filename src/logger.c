#include "logger.h"
#include <stdarg.h>
#include <stdlib.h>

static FILE *logFile = NULL;

int initLogger(const char *filename)
{
    logFile = fopen(filename, "a");
    return logFile != NULL ? 0 : -1;
}

void closeLogger()
{
    if (logFile)
    {
        fclose(logFile);
        logFile = NULL;
    }
}

void logError(const char *file, int line, const char *format, ...)
{
    if (!logFile)
    {
        fprintf(stderr, "Logger not initialized\n");
        return;
    }

    fprintf(logFile, "[ERROR] [%s:%d] ", file, line);

    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fflush(logFile);
}
