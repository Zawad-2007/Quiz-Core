#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../include/mcq.h"
#include "../include/misc.h"
#include "../include/affine_cipher.h"

#define qgets(buf,fstream) fgets(buf, sizeof(buf), fstream)

int encrypt_mcq_question(char original_filepath[], char encrypted_filepath[])
{ /*Encrypts quetsion written in this format:
 Question
 opt_a
 opt_b
 opt_c
 opt_d
 ans

 Stored in Struct named Question and then encrypted using affine cipher
 encrypted questions are stored in "encrypted_filepath" in binary format
 */
    int ques_count=0;
    FILE *unencrypted_ques = fopen(original_filepath, "rb");
    Question q[100];

    if (unencrypted_ques == NULL)
    {
        printf("\033[31mFILE error\033[0m\n");
        return -1;
    }
    else
    {
        int i = 0;

        while (qgets(q[i].ques, unencrypted_ques)!=NULL)
        {
            qgets(q[i].opt_a, unencrypted_ques);
            qgets(q[i].opt_b, unencrypted_ques);
            qgets(q[i].opt_c, unencrypted_ques);
            qgets(q[i].opt_d, unencrypted_ques);
            qgets(q[i].ans, unencrypted_ques);
            i++;
        }
        ques_count = i;
    }

    if(ques_count>=100)
    {
        printf("\033[31mMaximum Question limit(100) crossed\nAborting encryption...\033[0m");
        return -1;
    }

    fclose(unencrypted_ques);

    FILE *encrypted_ques = fopen(encrypted_filepath, "wb");
    for (int i = 0; i < ques_count; i++)
    {
        encrypt_affine((unsigned char *)q[i].ques, QUES_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_a, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_b, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_c, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_d, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].ans, ANS_SIZE, 5, 2);
        fwrite(q + i, sizeof(Question), 1, encrypted_ques);
    }

    fclose(encrypted_ques);
    return ques_count;
}
int decryptAndload_mcq_question(char encrypted_filepath[], Question *q)
{/**/
    FILE *encrypted_ques = fopen(encrypted_filepath, "rb");
    if(encrypted_ques==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    int i = 0;
    while (fread(q + i,sizeof(Question),1,encrypted_ques)!=0)
    {
        decrypt_affine((unsigned char *)q[i].ques, QUES_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_a, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_b, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_c, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_d, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].ans, ANS_SIZE, 5, 2);
        i++;
    }
    fclose(encrypted_ques);
    return i;
}

void start_mcq_quiz(QuizResult *qr_ptr)
{
    printf("Name                : ");
    fgets(qr_ptr->name, 32, stdin);
    rm_newl(qr_ptr->name, 32);

    printf("Roll                : ");
    fgets(qr_ptr->roll, 16, stdin);
    rm_newl(qr_ptr->roll, 16);

    printf("Subject of the exam : ");
    fgets(qr_ptr->subject, 32, stdin);
    rm_newl(qr_ptr->subject, 32);
}

void run_mcq_quiz(Question *decrypted_q_ptr, QuizResult *qr_ptr, int ques_count)
{
    printf("Total questions : %d\n", ques_count);
    qr_ptr->total_marks = ques_count;

    for (int i = 0; i < ques_count; i++)
    {
        printf("%s\n", decrypted_q_ptr[i].ques);
        printf("%s%s%s%s\n", decrypted_q_ptr[i].opt_a, decrypted_q_ptr[i].opt_b, decrypted_q_ptr[i].opt_c, decrypted_q_ptr[i].opt_d);
        char *ans_by_stud = calloc(5, sizeof(char));
        if (ans_by_stud == NULL)
        {
            printf("\033[31mMemory Error\033[0m\n");
            return;
        }
        printf("Ans (A/B/C/D) :");
        fgets(ans_by_stud, 5, stdin);
        if (ans_by_stud[0] == decrypted_q_ptr[i].ans[0])
        {
            printf("Correct!\n");
            qr_ptr->correct_ans++;
        }
        else
        {
            printf("Incorrect answer!\n");
            printf("Correct Answer :%s\n", decrypted_q_ptr[i].ans);
        }
        free(ans_by_stud);
    }

    qr_ptr->wrong_ans = qr_ptr->total_marks - qr_ptr->correct_ans;
    qr_ptr->accuracy = (qr_ptr->correct_ans * 100.00) / qr_ptr->total_marks;
}
