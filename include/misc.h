#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[32];
    char roll[16];
    char subject[32];
    int total_marks;
    int correct_ans;
    int wrong_ans;
    float accuracy;
} QuizResult;

void rm_newl(char *string, int lenght);

void store_progress_in_file(QuizResult *progress);

#endif