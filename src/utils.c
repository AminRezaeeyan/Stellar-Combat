#include "utils.h"

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

    // Trim leading spaces
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

void writeCSV(const char *filename, int numFields, ...)
{
    FILE *file = fopen(filename, "a");
    if (!file)
    {
        LOG_ERROR("Failed to open the csv file");
        return;
    }

    va_list args;
    va_start(args, numFields);

    for (int i = 0; i < numFields; i++)
    {
        const char *field = va_arg(args, const char *);
        fprintf(file, "%s", field);
        if (i < numFields - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    va_end(args);
    fclose(file);
}

char *generateTimestamp()
{
    static char buffer[20];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", t);
    return buffer;
}
