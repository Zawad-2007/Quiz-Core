#include"../include/login.h"

int encrypt_LoginData(char *og_login_data_filepath)
{
    FILE *og_login_data = fopen(og_login_data_filepath, "rb");
    if(og_login_data==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    ClassData *cd_ptr = malloc(sizeof(ClassData));
    fscanf(og_login_data, "%s %s %s %s", cd_ptr->Dept_code, cd_ptr->batch_code, cd_ptr->section, cd_ptr->Lab_group_code);
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

    LoginDta ld;

    int i = 0;

    char line_buf[128];
    while(fgets(line_buf,sizeof(line_buf),og_login_data)!=0)
    {
        
        if (sscanf(line_buf, "%32[^ ] %16s %32s[^\n]", ld.name, ld.roll, ld.password) == 3)
        {
            rm_extra_spaces(ld.name);
            encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
            encrypt_affine((unsigned char *)ld.roll, sizeof(ld.roll), 5, 2);
            encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);
            fwrite(&ld, sizeof(LoginDta), 1, encr_login_data);
        }
        else
        {
            printf("There is more or less data than needed in record No. %d\n", i+1);
            return -2;
        }
        i++;
    }

    fclose(og_login_data);
    fclose(encr_login_data);

    return i;
}