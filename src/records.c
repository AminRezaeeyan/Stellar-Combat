#include "records.h"

void addRecord(Record record)
{
    char scoreStr[12];
    sprintf(scoreStr, "%d", record.score);
    writeCSV(RECORDS_FILE_PATH, 2, scoreStr, record.timestamp);
}

Record createRecord(int score)
{
    Record record;
    record.score = score;
    strcpy(record.timestamp, generateTimestamp());
    return record;
}

int readRecords(Record *records, int maxRecords)
{
    FILE *file = fopen(RECORDS_FILE_PATH, "r");
    if (!file)
    {
        perror("Failed to open the csv file");
        return 0;
    }

    int totalLines = 0;
    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        totalLines++;
    }

    int startLine = totalLines > maxRecords ? totalLines - maxRecords : 0;
    rewind(file);

    int count = 0;
    int currentLine = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (currentLine >= startLine)
        {
            char *token = strtok(line, ",");
            records[count].score = atoi(token);

            token = strtok(NULL, ",");
            strcpy(records[count].timestamp, token);

            count++;
        }
        currentLine++;
    }

    fclose(file);
    return count;
}

int getHighestScore()
{
    int maxRecords = 1000;
    Record *records = (Record *)malloc(maxRecords * sizeof(Record));
    if (!records)
    {
        LOG_ERROR("Failed to allocate memory");
        return -1;
    }

    int numRecords = readRecords(records, maxRecords);
    if (numRecords == 0)
    {
        free(records);
        return -1;
    }

    int highestScore = records[0].score;
    for (int i = 1; i < numRecords; i++)
    {
        if (records[i].score > highestScore)
        {
            highestScore = records[i].score;
        }
    }

    free(records);
    return highestScore;
}

void displayRecords()
{
    clearScreen(COLOR_WHITE);
    renderBackground();

    Font fontLG = FONT_BREE_SERIF_LG;
    Font fontMD = FONT_BREE_SERIF_MD;
    Font fontSM = FONT_BREE_SERIF_SM;

    char highScore[10];
    Record records[10];
    sprintf(highScore, "%d", getHighestScore());
    int numRecords = readRecords(records, 10);

    renderText("Records", fontLG, COLOR_GOLD, 20, 20);
    renderText("Press any key to go back to the menu", fontSM, COLOR_WHITE, WINDOW_WIDTH - 320, WINDOW_HEIGHT - 50);
    renderText("Displaying your latest game records", fontSM, COLOR_WHITE, 10, WINDOW_HEIGHT - 50);

    if (numRecords > 0)
    {
        renderText("HighScore:", fontMD, COLOR_BLUE, 420, 100);
        renderText(highScore, fontMD, COLOR_WHITE, 620, 100);
        renderText("index", fontMD, COLOR_GOLD, 20, 200);
        renderText("score", fontMD, COLOR_GOLD, 200, 200);
        renderText("timestamp", fontMD, COLOR_GOLD, 450, 200);

        for (int i = 0; i < numRecords; i++)
        {
            char index[10];
            char score[10];
            char timestamp[20];

            sprintf(index, "%d", i + 1);
            sprintf(score, "%d", records[i].score);
            strcpy(timestamp, records[i].timestamp);

            renderText(index, fontMD, COLOR_BLUE, 20, 250 + i * 50);
            renderText(score, fontMD, COLOR_WHITE, 200, 250 + i * 50);
            renderText(timestamp, fontMD, COLOR_WHITE, 450, 250 + i * 50);

            renderLine(0, 255 + i * 50, WINDOW_WIDTH, 255 + i * 50, COLOR_BLUE);
        }
    }
    else
    {
        renderText("No records found", fontMD, COLOR_RED, 250, 300);
        renderText("Play and enjoy the game,", fontMD, COLOR_RED, 190, 400);
        renderText("then come back to see your records!", fontMD, COLOR_RED, 110, 500);
    }

    presentScreen();
    waitForKey();
}