//
// Created by James on 10/4/2024.
//

#ifndef WORKING_WITH_THREADS_MAXDIFF_JLEMAHIE_H
#define WORKING_WITH_THREADS_MAXDIFF_JLEMAHIE_H

#define BUFLEN 10100
#define LINELEN 256
#define NUM_THREADS 4

typedef struct {
    int startIndex; // start index in values[] at which to begin searching
    int endIndex;   // end index int maxDiff;
    int maxDiff;    // result: the largest difference found between consec
                    // entries, between startIndex and endIndex (inclusive)
    int val1;       // the first entry in the max diff
    int val2;       // the second entry in the max diff
    int position;   // the position of the first entry forming the max diff
} SearchInfo;

char buffer[BUFLEN];
int values[BUFLEN];

int readFile(char *fileName, int *numCharsRead);

int convertValues(int bufferLength, int *numValues);

void *searchForMaxDiff(void *param);
#endif //WORKING_WITH_THREADS_MAXDIFF_JLEMAHIE_H
