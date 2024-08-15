#include "utils.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Check if a file exists
int fileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

// Read the entire content of a file into a string
char *readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        LOG_ERROR("Could not open file for reading: %s", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (!content)
    {
        LOG_ERROR("Memory allocation failed for reading file: %s", filename);
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

// Append a string to the end of a file
int appendToFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "a");
    if (!file)
    {
        LOG_ERROR("Could not open file for appending: %s", filename);
        return 0;
    }

    fprintf(file, "%s\n", content);
    fclose(file);
    return 1;
}

// Trim leading and trailing whitespace from a string
char *trimWhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator
    end[1] = '\0';

    return str;
}
