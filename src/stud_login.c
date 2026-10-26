#include"../include/stud_login.h"

int encrypt_StudentLoginData(char *og_login_data_filepath)
{
    FILE *og_login_data = fopen(og_login_data_filepath, "rb");
    if(og_login_data==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    ClassData *cd_ptr = malloc(sizeof(ClassData));
    fscanf(og_login_data, "%s %s %s %s", cd_ptr->Dept_code, cd_ptr->batch_code, cd_ptr->section, cd_ptr->Lab_group_code);
    int c;
    while ((c = fgetc(og_login_data)) != '\n' && c != '\r')
        ; // clearing '\n' from buffer

    char encr_login_data_filepath[64];
    snprintf(encr_login_data_filepath, 64, "%s-%s-%s-%s.dat", cd_ptr->Dept_code, cd_ptr->batch_code, cd_ptr->section, cd_ptr->Lab_group_code);

    FILE *encr_login_data = fopen(encr_login_data_filepath, "wb");
    if (encr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }
    encrypt_affine((unsigned char *)cd_ptr->Dept_code, 8, 5, 2);
    encrypt_affine((unsigned char *)cd_ptr->batch_code, 8, 5, 2);
    encrypt_affine((unsigned char *)cd_ptr->section, 8, 5, 2);
    encrypt_affine((unsigned char *)cd_ptr->Lab_group_code, 8, 5, 2);

    fwrite(cd_ptr,sizeof(ClassData),1,encr_login_data);

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
            return -2;//Not enough data of a student's login info
        }
        rec_count++;
    }

    fclose(og_login_data);
    fclose(encr_login_data);

    return rec_count;  //This value will be used to show user the total No. of login data records
}

int decryptAndloadInmem_StudentLoginData(char *encr_login_data_filepath)
{
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");
    if(encr_login_data==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }
    else
    {
        ClassData cd;
        fread(&cd, sizeof(ClassData), 1, encr_login_data);

        decrypt_affine((unsigned char *)cd.Dept_code, 8, 5, 2);
        decrypt_affine((unsigned char *)cd.batch_code, 8, 5, 2);
        decrypt_affine((unsigned char *)cd.section, 8, 5, 2);
        decrypt_affine((unsigned char *)cd.Lab_group_code, 8, 5, 2);
        printf("Department Code : %s\n", cd.Dept_code);
        printf("Batch Code      : %s\n", cd.batch_code);
        printf("Section         : %s\n", cd.section);
        printf("Lab Group Code  : %s\n", cd.Lab_group_code);

        printf("\n");
        printf("%-32s%-16s%-16s", "Name", "Roll", "Password");
        printf("\n");

        StudentLoginDta ld;

        int rec_count = 0;
        while (fread(&ld, sizeof(StudentLoginDta), 1, encr_login_data) != 0)
        {
            decrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
            decrypt_affine((unsigned char *)ld.roll, sizeof(ld.roll), 5, 2);
            decrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);
            rec_count++;

            printf("%-32s%-16s%-16s\n", ld.name, ld.roll, ld.password);
        }
        return rec_count;
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