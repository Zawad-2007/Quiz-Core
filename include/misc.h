#ifndef MISC_H
#define MISC_H

#include <string.h>
#include "affine_cipher.h"

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<time.h>
#include<ctype.h>

typedef struct
{
    char name[32];
    char roll[16];
    char subject[32];
    char quiz_code[16];
    char quiz_date[32];
    int total_marks;
    int correct_ans;
    int wrong_ans;
    int ignored;
    float accuracy;
} QuizResult;

typedef struct
{
    char name[32];
    char roll[16];
    char subject[32];
    char quiz_code[16];
    char quiz_date[32];
    char total_marks[8];
    char correct_ans[8];
    char wrong_ans[8];
    char ignored[8];
    char accuracy[8];
} QuizResultStr;

typedef struct
{
    char quiz_code[16];
    char seconds_per_ques[4];
} QuestionHeader;

void rm_newl(char *string, int lenght);

void rm_extra_spaces(char *str);

void get_date_str(char *d_str, int size);

#endif