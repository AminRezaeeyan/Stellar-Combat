#ifndef RECORDS_H
#define RECORDS_H

#include "logger.h"
#include "graphics.h"
#include "utils.h"
#include <stdlib.h>

#define RECORDS_FILE_PATH "../resources/records.csv"

typedef struct
{
    int score;
    char timestamp[20];
} Record;

void addRecord(Record record);
Record createRecord(int score);
int readRecords(Record *records, int maxRecords);
int getHighestScore(void);
void displayRecords(void);

#endif