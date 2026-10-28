#ifndef TF_H
#define TF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "misc.h"
#include "affine_cipher.h"

#define TF_QUES_SIZE 256
#define TF_ANS_SIZE 8

typedef struct
{
    char ques[TF_QUES_SIZE];
    char ans[TF_ANS_SIZE];
} TFQuestion;

int encrypt_tf_question(char original_filepath[], char encrypted_filepath[], QuestionHeader *q_head);

int decryptAndload_tf_question(char encrypted_filepath[], TFQuestion *q, QuestionHeader *q_head_ptr);

void run_tf_quiz(TFQuestion *decrypted_q_ptr, QuizResult *qr_ptr, int ques_count, QuestionHeader *ques_head);

#endif

