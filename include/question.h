#ifndef QUESTION_H
#define QUESTION_H

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

int encrypt_mcq_question(char original_filepath[], char encrypted_filepath[]);

void decryptAndload_mcq_question(char encrypted_filepath[], Question *q);

#endif