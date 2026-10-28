#ifndef TEACHER_DATA_H
#define TEACHER_DATA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "affine_cipher.h"
#include "misc.h"

typedef struct
{
    char name[32];
    char dept_code[8];
    char designation_code[16];
    char joining_date[32];
    char password[16];
} TeacherLoginDta;

int encrypt_TeacherLoginData(char *og_login_data_filepath);

int decryptAndloadInmem_TeacherLoginData(char *encr_login_data_filepath, TeacherLoginDta **ld_ptr_addr, int ld_buf_size);

int decryptAndsave_TeacherLoginData(char *encr_login_data_filepath);

int append_encrypted_TeacherLoginData(char *encr_login_data_filepath);

#endif
