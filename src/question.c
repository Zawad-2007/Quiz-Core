#include<stdio.h>
#include<string.h>
#include"../include/question.h"
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
    FILE *unencrypted_ques = fopen(original_filepath, "r");
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
        printf("\033[31mQuestion limit crossed\nAborting encryption...\033[0m");
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
void decryptAndload_mcq_question(char encrypted_filepath[], char decrypted_filepath[], Question *q)
{
    FILE *encrypted_ques = fopen(encrypted_filepath, "rb");
    FILE *decrypted_ques = fopen(decrypted_filepath, "wb");


    int i = 0;
    while (fread(q + i,sizeof(Question),1,encrypted_ques)!=0)
    {
        decrypt_affine((unsigned char *)q[i].ques, QUES_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_a, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_b, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_c, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_d, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].ans, ANS_SIZE, 5, 2);
        fwrite(q + i, sizeof(Question), 1, decrypted_ques);
        i++;
    }
    fclose(encrypted_ques);
    fclose(decrypted_ques);
}
