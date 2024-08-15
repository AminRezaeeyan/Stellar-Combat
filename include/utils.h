#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

// File handling functions
int fileExists(const char *filename);
char *readFile(const char *filename);
int appendToFile(const char *filename, const char *content);

// String manipulation functions
char *trimWhitespace(char *str);

#endif // UTILS_H
