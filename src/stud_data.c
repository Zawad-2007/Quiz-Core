#include"../include/stud_data.h"

int encrypt_StudentLoginData(char *og_login_data_filepath)
{
    FILE *og_login_data = fopen(og_login_data_filepath, "rb");
    if(og_login_data==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    ClassData cd;
    fscanf(og_login_data, "%s %s %s %s", cd.Dept_code, cd.batch_code, cd.section, cd.Lab_group_code);
    int c;
    while ((c = fgetc(og_login_data)) != '\n' && c != '\r')
        ; // clearing '\n' from buffer

    char encr_login_data_filepath[64];
    snprintf(encr_login_data_filepath, 64, "%s-%s-%s-%s.dat", cd.Dept_code, cd.batch_code, cd.section, cd.Lab_group_code);

    FILE *file_already_exists = fopen(encr_login_data_filepath, "rb");
    if(file_already_exists!=NULL)
    {
        printf("File already exists.\nWant to overwrite?(y/n)\n");
        char c;
        scanf("%c", &c);
        getchar();
        if(tolower(c)=='n')
        {
            return -4;
        }
    }

    FILE *encr_login_data = fopen(encr_login_data_filepath, "wb");
    if (encr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -3;
    }
    encrypt_affine((unsigned char *)cd.Dept_code, 8, 5, 2);
    encrypt_affine((unsigned char *)cd.batch_code, 8, 5, 2);
    encrypt_affine((unsigned char *)cd.section, 8, 5, 2);
    encrypt_affine((unsigned char *)cd.Lab_group_code, 8, 5, 2);

    fwrite(&cd, sizeof(ClassData), 1, encr_login_data);

    char *unnecessary_line = malloc(256 * sizeof(char));  // "Name    Roll      Password"
    fgets(unnecessary_line, 256, og_login_data);       
    free(unnecessary_line);

    StudentLoginDta ld;

    int rec_count = 0;

    char line_buf[128];
    while(fgets(line_buf,sizeof(line_buf),og_login_data)!=0)
    {
        
        if (sscanf(line_buf, "%32[^ ] %16s %15s[^\r\n]", ld.name, ld.roll, ld.password) == 3)
        {
            rm_extra_spaces(ld.name);
            encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
            encrypt_affine((unsigned char *)ld.roll, sizeof(ld.roll), 5, 2);
            encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);
            fwrite(&ld, sizeof(StudentLoginDta), 1, encr_login_data);
        }
        else
        {
            return -4;//Not enough data of a student's login info
        }
        rec_count++;
    }

    fclose(og_login_data);
    fclose(encr_login_data);

    return rec_count;  //This value will be used to show user the total No. of login data records
}

int decryptAndloadInmem_StudentLoginData(char *encr_login_data_filepath, ClassData *cd_ptr, StudentLoginDta *ld_ptr,int ld_buf_size)
{/*Decrypts from an encrypted file and keeps them on given Struct Addresses*/
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");
    if(encr_login_data==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }
    else
    {
        fread(&cd_ptr, sizeof(ClassData), 1, encr_login_data);

        decrypt_affine((unsigned char *)cd_ptr->Dept_code, 8, 5, 2);
        decrypt_affine((unsigned char *)cd_ptr->batch_code, 8, 5, 2);
        decrypt_affine((unsigned char *)cd_ptr->section, 8, 5, 2);
        decrypt_affine((unsigned char *)cd_ptr->Lab_group_code, 8, 5, 2);       

        int i = 0;
        while (fread(&ld_ptr, sizeof(StudentLoginDta), 1, encr_login_data) != 0)
        {
            decrypt_affine((unsigned char *)ld_ptr[i].name, 32, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].roll, 16, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].password, 16, 5, 2);
            i++;
            if(i>=ld_buf_size)
            {
                StudentLoginDta *temp = realloc(ld_ptr, (i + 1) * sizeof(StudentLoginDta));
                if(temp==NULL)
                {
                    return -1;
                }
                else
                {
                    ld_ptr = temp;
                    //This mem address must be freed afterwards
                }
            }
        }
        return i;
    }
}

int decryptAndsave_StudentLoginData(char *encr_login_data_filepath)
{
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");

    if (encr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    
    ClassData cd;
    fread(&cd, sizeof(ClassData), 1, encr_login_data);

    decrypt_affine((unsigned char *)cd.Dept_code, 8, 5, 2);
    decrypt_affine((unsigned char *)cd.batch_code, 8, 5, 2);
    decrypt_affine((unsigned char *)cd.section, 8, 5, 2);
    decrypt_affine((unsigned char *)cd.Lab_group_code, 8, 5, 2);

     
    char decr_login_data_filepath[64];
    snprintf(decr_login_data_filepath, 64, "%s-%s-%s-%s.txt", cd.Dept_code, cd.batch_code, cd.section, cd.Lab_group_code);
    FILE *decr_login_data = fopen(decr_login_data_filepath, "wb");
    if (decr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    fprintf(decr_login_data, "%-32s%-16s%s\n", "Name", "Roll", "Password");

    StudentLoginDta ld;

    int rec_count = 0;
    while (fread(&ld, sizeof(StudentLoginDta), 1, encr_login_data) != 0)
    {
        decrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
        decrypt_affine((unsigned char *)ld.roll, sizeof(ld.roll), 5, 2);
        decrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);

        fprintf(decr_login_data, "%-32s%-16s%-16s\n", ld.name, ld.roll, ld.password);
        rec_count++;
    }
    return rec_count;
}

