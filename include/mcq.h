#ifndef MCQ_H
#define MCQ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "affine_cipher.h"

#define QUES_SIZE 256
#define OPT_SIZE 64
#define ANS_SIZE 8

typedef struct
{
    char ques[256]; 
    char opt_a[64];
    char opt_b[64];
    char opt_c[64];
    char opt_d[64];
    char ans[8];
} Question;

int encrypt_mcq_question(char original_filepath[], char encrypted_filepath[], QuestionHeader *q_head);

int decryptAndload_mcq_question(char encrypted_filepath[], Question *q, QuestionHeader *q_head_ptr);

void start_mcq_quiz(QuizResult *qr_ptr);

void run_mcq_quiz(Question *decrypted_q_ptr, QuizResult *qr_ptr, int ques_count, QuestionHeader *ques_head);

#endif