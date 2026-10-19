#ifndef QUESTION_H
#define QUESTION_H

typedef struct
{
    char ques[50];
    char opt_a[20];
    char opt_b[20];
    char opt_c[20];
    char opt_d[20];
    char ans[5];
} Question;

void encrypt_question(char original_filepath[], char encrypted_filepath[]);

void decrypt_question(char original_filepath[], char decrypted_filepath[]);

#endif