#ifndef MISC_H
#define MISC_H

#include <string.h>
#include "affine_cipher.h"

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

typedef struct
{
    char name[32];
    char roll[16];
    char subject[32];
    char total_marks[8];
    char correct_ans[8];
    char wrong_ans[8];
    char accuracy[8];
} QuizResultStr;

void rm_newl(char *string, int lenght);

void storeAndencrypt_progress_in_file(QuizResult *progress);

void decryptAndload_progress_in_mem();

#endif