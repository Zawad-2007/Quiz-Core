#ifndef STUD_DATA_H
#define STUD_DATA_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"affine_cipher.h"
#include"misc.h"

typedef struct
{
    char name[32];
    char roll[16];
    char password[16];
} StudentLoginDta;

    typedef struct
{
    char Dept_code[8];
    char section[8];
    char Lab_group_code[8];
    char batch_code[8];
} ClassData;

int encrypt_StudentLoginData(char *og_login_data_filepath);

int decryptAndloadInmem_StudentLoginData(char *encr_login_data_filepath, ClassData *cd_ptr, StudentLoginDta **ld_ptr_addr, int ld_buf_size);

int decryptAndsave_StudentLoginData(char *encr_login_data_filepath);

int append_encrypted_StudentLoginData(char *encr_login_data_filepath);

#endif