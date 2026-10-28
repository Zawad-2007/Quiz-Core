#include"../include/stud_data.h"


int encrypt_StudentLoginData(char *og_login_data_filepath)
{
    FILE *og_login_data = fopen(og_login_data_filepath, "rb");
    if(og_login_data==NULL)
    {
        print_file_err();
        return -1;
    }

    ClassData cd;
    if (fscanf(og_login_data, "%s %s %s %s", cd.Dept_code, cd.batch_code, cd.section, cd.Lab_group_code)!=4)
    {
        printf("\033[31mDept_code, batch_code, section and Lab_group_code not written\033[0m\n");
        return -2;
    }
        int c;
    while ((c = fgetc(og_login_data)) != '\n' && c != '\r')
        ; // clearing '\n' from buffer

    char encr_login_data_filepath[64];
    snprintf(encr_login_data_filepath, 64, "%s-%s-%s-%s.dat", cd.Dept_code, cd.batch_code, cd.section, cd.Lab_group_code);

    FILE *file_already_exists = fopen(encr_login_data_filepath, "rb");
    if(file_already_exists)
    {
        printf("File already exists.\nWant to overwrite?(y/n)\n");
        char c;
        scanf("%c", &c);
        getchar();
        if(tolower(c)=='n')
        {
            fclose(file_already_exists);
            return -4;
        }
        else
        {
            fclose(file_already_exists);
        }
    }

    printf("%s created\n", encr_login_data_filepath);

    FILE *encr_login_data = fopen(encr_login_data_filepath, "wb");
    if (encr_login_data == NULL)
    {
        print_file_err();
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
        char *token;

        token = strtok(line_buf, ",");
        if(token!=NULL)strcpy(ld.name, token);
        else continue;
        token = strtok(NULL, ",");
        if (token != NULL)strcpy(ld.roll, token);
        else continue;
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(ld.password, token);
        else continue;

        rm_extra_spaces(ld.name);
        rm_extra_spaces(ld.roll);
        rm_extra_spaces(ld.password);
        rm_newl(ld.password, 16);

        encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
        encrypt_affine((unsigned char *)ld.roll, sizeof(ld.roll), 5, 2);
        encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);
        fwrite(&ld, sizeof(StudentLoginDta), 1, encr_login_data);
        rec_count++;
    }

    if(rec_count==0)
    {
        printf("No records in the unencrypted file\n");
        printf("\033[32mBut you can append the data using this application :)\033[0m\n");
        return 0;
    }

    fclose(og_login_data);
    fclose(encr_login_data);

    return rec_count;  //This value will be used to show user the total No. of login data records
}

int decryptAndloadInmem_StudentLoginData(char *encr_login_data_filepath, ClassData *cd_ptr, StudentLoginDta **ld_ptr_addr,int ld_buf_size)
{/*Decrypts from an encrypted file and keeps them on given Struct Addresses*/
    StudentLoginDta *ld_ptr = *ld_ptr_addr;
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");

    if (encr_login_data == NULL)
    {
        print_file_err();
        return -1;
    }
    else
    {
        fread(cd_ptr, sizeof(ClassData), 1, encr_login_data);

        decrypt_affine((unsigned char *)cd_ptr->Dept_code, 8, 5, 2);
        decrypt_affine((unsigned char *)cd_ptr->batch_code, 8, 5, 2);
        decrypt_affine((unsigned char *)cd_ptr->section, 8, 5, 2);
        decrypt_affine((unsigned char *)cd_ptr->Lab_group_code, 8, 5, 2);       

        int i = 0;
        while (fread(ld_ptr + i, sizeof(StudentLoginDta), 1, encr_login_data) != 0)
        {
            decrypt_affine((unsigned char *)ld_ptr[i].name, 32, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].roll, 16, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].password, 16, 5, 2);
            i++;
            if(i>=ld_buf_size)
            {
                ld_buf_size = i + 5; // Expand memory for 5 more records
                StudentLoginDta *temp = realloc(ld_ptr, ld_buf_size * sizeof(StudentLoginDta));
                if (temp == NULL)
                {
                    fclose(encr_login_data);
                    return -2;
                }
                ld_ptr = temp;
                *ld_ptr_addr = temp;
            }
        }
        fclose(encr_login_data);
        return i;
    }
}

int decryptAndsave_StudentLoginData(char *encr_login_data_filepath)
{
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");

    if (encr_login_data == NULL)
    {
        print_file_err();
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
        print_file_err();
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

int append_encrypted_StudentLoginData(char *encr_login_data_filepath)
{
    FILE *file_exists = fopen(encr_login_data_filepath, "rb");
    if (!(file_exists))
    {
        print_file_err();
        return -1;
    }
    else
    {
        ClassData cd;
        if (fread(&cd, sizeof(ClassData), 1, file_exists) != 0)
        {
            decrypt_affine((unsigned char *)cd.Dept_code, 8, 5, 2);
            decrypt_affine((unsigned char *)cd.batch_code, 8, 5, 2);
            decrypt_affine((unsigned char *)cd.section, 8, 5, 2);
            decrypt_affine((unsigned char *)cd.Lab_group_code, 8, 5, 2);

            printf("Class Data :\n");
            printf("Department Code : %s\n", cd.Dept_code);
            printf("Batch Code      : %s\n", cd.batch_code);
            printf("Section         : %s\n", cd.section);
            printf("Lab Group Code  : %s\n", cd.Lab_group_code);

            fclose(file_exists);
        }
        else
        {
            printf("\033[31mFile does not contain ClassData header\033[0m\n");
            printf("\033[32mEncrypt the original file properly\033[0m\n");
            fclose(file_exists);
            return -2;
        }

        char c = 'y';
        while (1)
        {
            FILE *encr_login_data = fopen(encr_login_data_filepath, "ab");
            if (encr_login_data == NULL)
            {
                printf("\033[31mFile Error\033[0m\n");
                return -3;
            }
            StudentLoginDta ld;
            printf("Name     : ");
            fgets(ld.name, sizeof(ld.name), stdin);
            printf("Roll     : ");
            fgets(ld.roll, sizeof(ld.roll), stdin);
            printf("Password : ");
            fgets(ld.password, sizeof(ld.password), stdin);

            rm_newl(ld.name, sizeof(ld.name));
            rm_newl(ld.roll, sizeof(ld.roll));
            rm_newl(ld.password, sizeof(ld.password));

            encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
            encrypt_affine((unsigned char *)ld.roll, sizeof(ld.roll), 5, 2);
            encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);

            if (fwrite(&ld, sizeof(StudentLoginDta), 1, encr_login_data) == 0)
            {
                printf("\033[31mCannot append\033[0m\n");
                fclose(encr_login_data);
                return -3;
            }
            fclose(encr_login_data);

            printf("Want to add another data of a student?(y/n): ");
            if (scanf("%c", &c) != 1) break;
            getchar();

            if (tolower(c) == 'y')
            {
                continue;
            }
            else if (tolower(c) == 'n')
            {
                break;
            }
            else
            {
                printf("Wrong option.\n");
                break;
            }
        }
    }
    return 0;
}
