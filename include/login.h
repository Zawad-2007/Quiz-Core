#ifndef LOGIN_H
#define LOGIN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"affine_cipher.h"
#include"misc.h"

typedef struct
{
    char name[32];
    char roll[16];
    char password[16];
} LoginDta;

typedef struct
{
    char Dept_code[8];
    char section[8];
    char Lab_group_code[8];
    char batch_code[8];
} ClassData;

int encrypt_LoginData(char *og_login_data_filepath);

void decryptAndsave_LoginData(char *encr_login_data_filepath);

void decryptAndsave_LoginData(char *encr_login_data_filepath);

#endif