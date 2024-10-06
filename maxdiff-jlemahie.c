//
// Created by James on 10/4/2024.
//

#include "maxdiff-jlemahie.h"
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


int readFile(char *fileName, int *numCharsRead){
    //creating line variable
    char line[LINELEN];
    //indicator variable for while loop
    bool cont_read = true;

    //opening file
    FILE *fp = fopen(fileName, "r"); // filename is the char* passed to your readFile() function
    if (fp == NULL) {
        printf("ERROR: cannot open file '%s'\n", fileName);
        return 8;
    }

    //starting while loop to read in file
    while(cont_read){
        //reading line into file
        char *chp = fgets(line, LINELEN, fp);
        //if return value is null break out of loop because end of file
        if(chp == NULL){
            cont_read = false;
        //if line ends in new line character strip off new line character and concatenate it to the buffer.
        } else if (line[strlen(line)-1] == '\n'){
            line[strlen(line)-1] = '\0';
            strcat(buffer, line);
        //if line is full just concatenate it with the buffer.
        } else {
            strcat(buffer, line);
        }
    }
    //setting number characters read to the length of the buffer
    *numCharsRead = strlen(buffer);
    return 0;
}

int convertValues(int bufferLength, int *numValues){
    //declaring varaibles needed in method
    int placeValue, hundredsInt, tensInt, onesInt, threeDigitNum, twoDigitNum;
    int copyIndex = 0;
    int insertIndex = 0;
    char hundredsChar, tensChar, onesChar;

    //while there are still characters to read in the buffer
    while(copyIndex < bufferLength){
        //creating a string fo three characters
        char *numberStr = malloc(4 * sizeof(char));
        //going through the next three characters in the buffer and copying them over into the number str
        for(placeValue = 0; placeValue < 3; placeValue++){
            if (buffer[copyIndex] == '.'){
                placeValue--;
            } else {
                if (copyIndex < bufferLength){
                    numberStr[placeValue] = buffer[copyIndex];
                }
            }
            copyIndex++;
        }
        //converting numberStr to an integer and then writing the three digit number to the values array
        threeDigitNum = atoi(numberStr);
        values[insertIndex] = threeDigitNum;
        insertIndex++;
    }
    //assigning the number of values equal to the insert index.
    *numValues = insertIndex;
    return 0;
}

void *searchForMaxDiff(void *param){
    //creating searchinfo struct
    SearchInfo *info = (SearchInfo *) param;
    //setting max difference to 0 so any difference will be greater
    info->maxDiff = 0;
    //declaring local variables needed for the method
    int i, diff;

    //will go until every pair of consecutive integers have been checked
    for (i=info->startIndex; i<info->endIndex - 1; ++i) {
        //finding difference of pairs of integers
        diff = abs(values[i] - values[i + 1]);
        //if the difference is larger than the max difference update info's stats
        if (diff > info->maxDiff) {
            info->maxDiff = diff;
            info->val1 = values[i];
            info->val2 = values[i + 1];
            info->position = i;
        }

    }
    //exiting thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    //declaring all necessary variables for the main method
    SearchInfo searchInfo[NUM_THREADS];
    pthread_t tid[NUM_THREADS];
    SearchInfo winner = searchInfo[0];
    int i, pos, chunkSize, rc, numChars, cv, numValues;

    //checking if terminal call is corrent
    if (argc < 2) {
        printf("ERROR: need a filename\n");
        return 8;
    }
    //checling if reading file is correct
    rc = readFile(argv[1], &numChars);
    if (rc != 0)
        return 8;
    //checking if converting values is correct.
    cv = convertValues(numChars, &numValues);
    if (cv != 0){
        return 8;
    }

    //creating the search regions for threads
    chunkSize = numValues / NUM_THREADS;
    pos = 0;
    for (i=0; i<NUM_THREADS; ++i) {
        searchInfo[i].startIndex = pos;
        searchInfo[i].endIndex = searchInfo[i].startIndex + chunkSize;
        pos = pos + chunkSize + 1;
    }

    //adjusting the last chunk just incase
    if (searchInfo[NUM_THREADS-1].endIndex < numValues-1)
        searchInfo[NUM_THREADS-1].endIndex = numValues - 1;
    else if (searchInfo[NUM_THREADS-1].endIndex > numValues-1)
        searchInfo[NUM_THREADS-1].endIndex = numValues - 1;

    //creating threads
    for (i=0; i<NUM_THREADS; ++i)
        pthread_create(&tid[i], NULL, searchForMaxDiff, &searchInfo[i]);

    //waiting for threads to join back together
    for (i=0; i<NUM_THREADS; ++i)
        pthread_join(tid[i], NULL);

    winner = searchInfo[0];
    for (i=1; i<NUM_THREADS; ++i) {
        if (searchInfo[i].maxDiff > winner.maxDiff)
            winner = searchInfo[i];
    }

    printf("The maximum difference between consecutive values is %d.\n", winner.maxDiff);
    printf("This difference is between the values %d and %d.\n", winner.val1, winner.val2);
    printf("This pair can be found at index %d and %d in the values vector.\n", winner.position, (winner.position +1));
    return 0;
}