#include "../include/teacher_data.h"

int encrypt_TeacherLoginData(char *og_login_data_filepath)
{
    FILE *og_login_data = fopen(og_login_data_filepath, "rb");
    if (og_login_data == NULL)
    {
        print_file_err();
        return -1;
    }
    char *unnecessary_line = malloc(256 * sizeof(char)); // "Name    Dept. Code Designation Code ..."
    if (unnecessary_line != NULL)
    {
        if (fgets(unnecessary_line, 256, og_login_data) == NULL)
        {
            free(unnecessary_line);
            fclose(og_login_data);
            return -1;
        }
        free(unnecessary_line);
    }

    printf("Enter the Department Code : ");
    char dept_code[16];
    if (scanf("%15s", dept_code) != 1)
    {
        fclose(og_login_data);
        return -1;
    }
    getchar();

    char encr_login_data_filepath[64];
    snprintf(encr_login_data_filepath, sizeof(encr_login_data_filepath), "%s_teachers.dat", dept_code);

    FILE *file_already_exists = fopen(encr_login_data_filepath, "rb");
    if (file_already_exists)
    {
        printf("File already exists.\nWant to overwrite?(y/n)\n");
        char c;
        if (scanf("%c", &c) != 1)
        {
            fclose(file_already_exists);
            fclose(og_login_data);
            return -4;
        }
        getchar();
        fclose(file_already_exists);
        if (tolower((unsigned char)c) == 'n')
        {
            fclose(og_login_data);
            return -4;
        }
    }

    FILE *encr_login_data = fopen(encr_login_data_filepath, "wb");
    if (encr_login_data == NULL)
    {
        print_file_err();
        fclose(og_login_data);
        return -3;
    }

    printf("%s created\n", encr_login_data_filepath);

    TeacherLoginDta ld;
    int rec_count = 0;

    char line_buf[128];
    while (fgets(line_buf, sizeof(line_buf), og_login_data) != NULL)
    {
        char *ptr[] = {ld.name, ld.dept_code, ld.designation_code, ld.joining_date, ld.password};

        int i = 0; 
        char *token = strtok(line_buf, ",");

        while (token != NULL && i < 5)
        {
            strncpy(ptr[i], token, 31);
            ptr[i][31] = '\0';

            token = strtok(NULL, ",");
            i++;
            if (token != NULL && i >= 5)
            {
                printf("More elements in teacher login data record %d (skipping extra)\n", rec_count + 1);
                break;
            }
        }

        if (i < 4)
        {
            printf("Fewer elements than expected in teacher login data record %d\n", rec_count + 1);
            fclose(og_login_data);
            fclose(encr_login_data);
            return -5;
        }

        rm_extra_spaces(ld.name);
        rm_extra_spaces(ld.dept_code);
        rm_extra_spaces(ld.designation_code);
        rm_extra_spaces(ld.joining_date);
        rm_extra_spaces(ld.password);
        rm_newl(ld.password, sizeof(ld.password));

        encrypt_affine((unsigned char *)ld.name, sizeof(ld.name), 5, 2);
        encrypt_affine((unsigned char *)ld.dept_code, sizeof(ld.dept_code), 5, 2);
        encrypt_affine((unsigned char *)ld.designation_code, sizeof(ld.designation_code), 5, 2);
        encrypt_affine((unsigned char *)ld.joining_date, sizeof(ld.joining_date), 5, 2);
        encrypt_affine((unsigned char *)ld.password, sizeof(ld.password), 5, 2);
        fwrite(&ld, sizeof(TeacherLoginDta), 1, encr_login_data);
        rec_count++;
    }

    fclose(og_login_data);
    fclose(encr_login_data);
    return rec_count;
}

int decryptAndloadInmem_TeacherLoginData(char *encr_login_data_filepath, TeacherLoginDta **ld_ptr_addr, int ld_buf_size)
{
    TeacherLoginDta *ld_ptr = *ld_ptr_addr;
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");
    if (encr_login_data == NULL)
    {
        print_file_err();
        return -1;
    }

    int i = 0;
    while (fread(ld_ptr + i, sizeof(TeacherLoginDta), 1, encr_login_data) != 0)
    {
        decrypt_affine((unsigned char *)ld_ptr[i].name, 32, 5, 2);
        decrypt_affine((unsigned char *)ld_ptr[i].dept_code, 8, 5, 2);
        decrypt_affine((unsigned char *)ld_ptr[i].designation_code, 16, 5, 2);
        decrypt_affine((unsigned char *)ld_ptr[i].joining_date, 32, 5, 2);
        decrypt_affine((unsigned char *)ld_ptr[i].password, 16, 5, 2);

        i++;
        if (i >= ld_buf_size)
        {
            ld_buf_size = i + 5;
            TeacherLoginDta *temp = (TeacherLoginDta *)realloc(ld_ptr, ld_buf_size * sizeof(TeacherLoginDta));
            if (temp == NULL)
            {
                fclose(encr_login_data);
                return -1;
            }
            ld_ptr = temp;
            *ld_ptr_addr = temp;
        }
    }
    fclose(encr_login_data);
    return i;
}

int decryptAndsave_TeacherLoginData(char *encr_login_data_filepath)
{
    FILE *encr_login_data = fopen(encr_login_data_filepath, "rb");
    if (encr_login_data == NULL)
    {
        print_file_err();
        return -1;
    }

    printf("Enter Department Code : ");
    char dept_code[8];
    if (scanf("%7s", dept_code) != 1)
    {
        fclose(encr_login_data);
        return -1;
    }
    getchar();

    char decr_login_data_filepath[64];
    snprintf(decr_login_data_filepath, sizeof(decr_login_data_filepath), "%s_teachers.txt", dept_code);
    FILE *decr_login_data = fopen(decr_login_data_filepath, "wb");
    if (decr_login_data == NULL)
    {
        print_file_err();
        fclose(encr_login_data);
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

    fclose(encr_login_data);
    fclose(decr_login_data);
    return rec_count;
}

int append_encrypted_TeacherLoginData(char *encr_login_data_filepath)
{
    FILE *file_exists = fopen(encr_login_data_filepath, "rb");
    if (!(file_exists))
    {
        print_file_err();
        return -1;
    }
    fclose(file_exists);

    char c = 'y';
    while (1)
    {
        FILE *encr_login_data = fopen(encr_login_data_filepath, "ab");
        if (encr_login_data == NULL)
        {
            printf("\033[31mCannot open file for appending\033[0m\n");
            return -3;
        }

        TeacherLoginDta ld;
        printf("Name              : ");
        if (fgets(ld.name, sizeof(ld.name), stdin) == NULL) { fclose(encr_login_data); break; }
        printf("Dept. Code        : ");
        if (fgets(ld.dept_code, sizeof(ld.dept_code), stdin) == NULL) { fclose(encr_login_data); break; }
        printf("Designation Code  : ");
        if (fgets(ld.designation_code, sizeof(ld.designation_code), stdin) == NULL) { fclose(encr_login_data); break; }
        printf("Joining Date      : ");
        if (fgets(ld.joining_date, sizeof(ld.joining_date), stdin) == NULL) { fclose(encr_login_data); break; }
        printf("Password          : ");
        if (fgets(ld.password, sizeof(ld.password), stdin) == NULL) { fclose(encr_login_data); break; }

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
            fclose(encr_login_data);
            return -3;
        }
        fclose(encr_login_data);

        printf("Want to add another teacher?(y/n): ");
        if (scanf("%c", &c) != 1) break;
        getchar();

        if (tolower((unsigned char)c) == 'y')
        {
            continue;
        }
        else if (tolower((unsigned char)c) == 'n')
        {
            break;
        }
        else
        {
            printf("Invalid option. Stopping.\n");
            break;
        }
    }
    return 0;
}
