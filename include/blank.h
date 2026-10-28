#ifndef BLANK_H
#define BLANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "misc.h"
#include "affine_cipher.h"

#define BLANK_QUES_SIZE 256
#define BLANK_ANS_SIZE 64

typedef struct
{
    char ques[BLANK_QUES_SIZE];
    char ans[BLANK_ANS_SIZE];
} BlankQuestion;

int encrypt_blank_question(char original_filepath[], char encrypted_filepath[], QuestionHeader *q_head);

int decryptAndload_blank_question(char encrypted_filepath[], BlankQuestion *q, QuestionHeader *q_head_ptr);

void run_blank_quiz(BlankQuestion *decrypted_q_ptr, QuizResult *qr_ptr, int ques_count, QuestionHeader *ques_head);

#endif

