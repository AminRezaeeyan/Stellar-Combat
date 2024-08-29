#ifndef UTILS_H
#define UTILS_H

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

// Number functions
int countDigits(int n);

// File handling functions
int fileExists(const char *filename);
int appendToFile(const char *filename, const char *content);
void writeCSV(const char *filename, int numFields, ...);

// String manipulation functions
char *trimWhitespace(char *str);

// Timestamp functions
char *generateTimestamp();

#endif
