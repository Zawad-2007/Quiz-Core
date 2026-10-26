#include"../include/teacher_data.h"

int encrypt_TeacherLoginData(char *og_login_data_filepath)
{
    FILE *og_login_data = fopen(og_login_data_filepath, "rb");
    if (og_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }
    char *unnecessary_line = malloc(256 * sizeof(char)); // "Name    Dept. Code Designation Code ..."
    fgets(unnecessary_line, 256, og_login_data);
    free(unnecessary_line);

    printf("Enter the Department Code properly : ");
    char dept_code[16];
    scanf("%s", dept_code);
    getchar();

    char encr_login_data_filepath[64];
    snprintf(encr_login_data_filepath,64, "%s_teachers.dat", dept_code);

    FILE *encr_login_data = fopen(encr_login_data_filepath, "wb");
    FILE *file_already_exists = fopen(encr_login_data_filepath, "rb");
    if (file_already_exists)
    {
        printf("File already exists.\nWant to overwrite?(y/n)\n");
        char c;
        scanf("%c", &c);
        getchar();
        if (tolower(c) == 'n')
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

    TeacherLoginDta ld;
    int rec_count = 0;

    char line_buf[128];
    while (fgets(line_buf, sizeof(line_buf), og_login_data) != 0)
    {
        char *ptr[] = {ld.name, ld.dept_code, ld.designation_code, ld.joining_date, ld.password};

        int i = 0; //get the first token
        char *token = strtok(line_buf, ",");

        
        while (token != NULL && i < 5)
        {//loop condition : valid token && array index check
            strcpy(ptr[i], token);

            i++;
            token = strtok(NULL, ","); //grabbing the next token at the end of iteration
        }

        
        if (i != 5)
        {
            printf("More or less element in the teacher login data of index %d\n", rec_count + 1);
            return -5;
        }

        rm_extra_spaces(ld.name);
        rm_extra_spaces(ld.dept_code);
        rm_extra_spaces(ld.designation_code);
        rm_extra_spaces(ld.joining_date);
        rm_extra_spaces(ld.password);
        rm_newl(ld.password, 16);

        encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
        encrypt_affine((unsigned char *)ld.dept_code, sizeof(ld.dept_code), 5, 2);
        encrypt_affine((unsigned char *)ld.designation_code, sizeof(ld.designation_code), 5, 2);
        encrypt_affine((unsigned char *)ld.joining_date, sizeof(ld.joining_date), 5, 2);
        encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);
        fwrite(&ld, sizeof(TeacherLoginDta), 1, encr_login_data);
        rec_count++;
    }
    return rec_count;
}

int decryptAndloadInmem_TeacherLoginData(char *encr_login_data_filepath, TeacherLoginDta *ld_ptr, int ld_buf_size)
{
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");
    if (encr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }
    else
    {
        int i = 0;
        while (fread(&ld_ptr, sizeof(TeacherLoginDta), 1, encr_login_data) != 0)
        {
            decrypt_affine((unsigned char *)ld_ptr[i].name, 32, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].dept_code, 8, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].designation_code, 16, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].joining_date, 32, 5, 2);
            decrypt_affine((unsigned char *)ld_ptr[i].password, 16, 5, 2);

            i++;
            if (i >= ld_buf_size)
            {
                TeacherLoginDta *temp = realloc(ld_ptr, (i + 1) * sizeof(TeacherLoginDta));
                if (temp == NULL)
                {
                    return -1;
                }
                else
                {
                    ld_ptr = temp;
                    // This mem address must be freed afterwards
                }
            }
        }
        return i;
    }
}

int decryptAndsave_TeacherLoginData(char *encr_login_data_filepath)
{
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");

    if (encr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    printf("Enter Department Code : ");
    char dept_code[8];
    scanf("%s", dept_code);
    getchar();

    char decr_login_data_filepath[64];
    snprintf(decr_login_data_filepath, 64, "%s_teachers.txt", dept_code);
    FILE *decr_login_data = fopen(decr_login_data_filepath, "wb");
    if (decr_login_data == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return -1;
    }

    fprintf(decr_login_data, "%-32s%-16s%-20s%-32s%s\n", "Name", "Dept Code", "Designation Code", "Joining Date", "Password");

    TeacherLoginDta ld;

    int rec_count = 0;
    while (fread(&ld, sizeof(TeacherLoginDta), 1, encr_login_data) != 0)
    {
        decrypt_affine((unsigned char *)ld.name, 32, 5, 2);
        decrypt_affine((unsigned char *)ld.dept_code, 8, 5, 2);
        decrypt_affine((unsigned char *)ld.designation_code, 16, 5, 2);
        decrypt_affine((unsigned char *)ld.joining_date, 32, 5, 2);
        decrypt_affine((unsigned char *)ld.password, 16, 5, 2);

        fprintf(decr_login_data, "%-32s%-16s%-20s%-32s%s\n", ld.name, ld.dept_code, ld.designation_code, ld.joining_date, ld.password);
        rec_count++;
    }
    return rec_count;
}

int append_encrypted_TeacherLoginData(char *encr_login_data_filepath)
{
    FILE *file_exists = fopen(encr_login_data_filepath, "rb");
    if (!(file_exists))
    {
        printf("\033[31mFile does not exist\033[0m\n");
        return -1;
    }
    else
    {
        char c = 'y';
        while (1)
        {
            FILE *encr_login_data = fopen(encr_login_data_filepath, "ab");
            TeacherLoginDta ld;
            printf("Name              : ");
            fgets(ld.name, sizeof(ld.name), stdin);
            printf("Dept. Code        : ");
            fgets(ld.dept_code, sizeof(ld.dept_code), stdin);
            printf("Designation Code  : ");
            fgets(ld.designation_code, sizeof(ld.designation_code), stdin);
            printf("Joining Date      : ");
            fgets(ld.joining_date, sizeof(ld.joining_date), stdin);
            printf("Password : ");
            fgets(ld.password, sizeof(ld.password), stdin);

            rm_newl(ld.name, sizeof(ld.name));
            rm_newl(ld.dept_code, sizeof(ld.dept_code));
            rm_newl(ld.designation_code, sizeof(ld.designation_code));
            rm_newl(ld.joining_date, sizeof(ld.joining_date));
            rm_newl(ld.password, sizeof(ld.password));

            encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
            encrypt_affine((unsigned char *)ld.dept_code, sizeof(ld.dept_code), 5, 2);
            encrypt_affine((unsigned char *)ld.designation_code, sizeof(ld.designation_code), 5, 2);
            encrypt_affine((unsigned char *)ld.joining_date, sizeof(ld.joining_date), 5, 2);
            encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);

            if (fwrite(&ld, sizeof(TeacherLoginDta), 1, encr_login_data) == 0)
            {
                printf("\033[31mCannot append\033[0m\n");
                return -3;
            }

            printf("Want to add another data of a student?(y/n)");
            scanf("%c", &c);
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
                continue;
            }
        }
    }
    return 0;
}
