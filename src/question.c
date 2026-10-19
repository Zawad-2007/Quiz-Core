#include<stdio.h>
#include"../include/question.h"
#include "../include/affine_cipher.h"

#define qgets(buf,fstream) fgets(buf, sizeof(buf), fstream)

#pragma pack(1)

void encrypt_mcq_question(char original_filepath[], char encrypted_filepath[])
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
        return ;
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
        return;
    }

    fclose(unencrypted_ques);

    FILE *encrypted_ques = fopen(encrypted_filepath, "wb");
    for (int i = 0; i < ques_count; i++)
    {
        encrypt_affine((unsigned char *)q[i].ques, 50, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_a, 20, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_b, 20, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_c, 20, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_d, 20, 5, 2);
        encrypt_affine((unsigned char *)q[i].ans, 5, 5, 2);
        fwrite(q + i, sizeof(Question), 1, encrypted_ques);
    }

    fclose(encrypted_ques);
}

void decrypt_mcq_question(char encrypted_filepath[], char decrypted_filepath[])
{
    FILE *encrypted_ques = fopen(encrypted_filepath, "rb");
    FILE *decrypted_ques = fopen(decrypted_filepath, "wb");
    Question q[100];

    int i = 0;
    while (fread(q + i,sizeof(Question),1,encrypted_ques)!=0)
    {
        decrypt_affine((unsigned char *)q[i].ques, 50, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_a, 20, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_b, 20, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_c, 20, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_d, 20, 5, 2);
        decrypt_affine((unsigned char *)q[i].ans, 5, 5, 2);
        fwrite(q + i, sizeof(Question), 1, decrypted_ques);
        i++;
    }
}
