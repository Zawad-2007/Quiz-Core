#include "../include/auth.h"

int student_login(StudentLoginDta *student_out)
{
    char filepath[64] = "CSE-2025-B-B1.dat";
    char input_filepath[64];

    printf("Enter class code or student database file [default: CSE-2025-B-B1.dat]: ");
    if (fgets(input_filepath, sizeof(input_filepath), stdin) != NULL)
    {
        rm_newl(input_filepath, sizeof(input_filepath));
        rm_extra_spaces(input_filepath);
        if (strlen(input_filepath) > 0)
        {
            if (strstr(input_filepath, ".dat") == NULL)
            {
                snprintf(filepath, sizeof(filepath), "%s.dat", input_filepath);
            }
            else
            {
                strncpy(filepath, input_filepath, sizeof(filepath) - 1);
                filepath[sizeof(filepath) - 1] = '\0';
            }
        }
    }

    ClassData cd;
    int buf_size = 10;
    StudentLoginDta *ld_ptr = (StudentLoginDta *)malloc(buf_size * sizeof(StudentLoginDta));
    if (ld_ptr == NULL)
    {
        printf("\033[31mMemory allocation error\033[0m\n");
        return 0;
    }

    int n = decryptAndloadInmem_StudentLoginData(filepath, &cd, &ld_ptr, buf_size);
    if (n <= 0)
    {
        printf("\033[31mCould not load student database from '%s'\033[0m\n", filepath);
        free(ld_ptr);
        return 0;
    }

    char input_roll[16];
    char input_pass[16];

    printf("Enter Roll     : ");
    if (fgets(input_roll, sizeof(input_roll), stdin) == NULL)
    {
        free(ld_ptr);
        return 0;
    }
    rm_newl(input_roll, sizeof(input_roll));
    rm_extra_spaces(input_roll);

    printf("Enter Password : ");
    if (fgets(input_pass, sizeof(input_pass), stdin) == NULL)
    {
        free(ld_ptr);
        return 0;
    }
    rm_newl(input_pass, sizeof(input_pass));
    rm_extra_spaces(input_pass);

    for (int i = 0; i < n; i++)
    {
        if (strcmp(ld_ptr[i].roll, input_roll) == 0 && strcmp(ld_ptr[i].password, input_pass) == 0)
        {
            memcpy(student_out, &ld_ptr[i], sizeof(StudentLoginDta));
            printf("\n\033[32m[✓] Login Successful! Welcome, %s (Roll: %s)\033[0m\n",
                   student_out->name, student_out->roll);
            free(ld_ptr);
            return 1;
        }
    }

    printf("\n\033[31m[✗] Authentication failed: Incorrect Roll or Password\033[0m\n");
    free(ld_ptr);
    return 0;
}

int teacher_login(TeacherLoginDta *teacher_out)
{
    char filepath[64] = "CSE_teachers.dat";
    char input_dept[64];

    printf("Enter Department code or teacher file [default: CSE_teachers.dat]: ");
    if (fgets(input_dept, sizeof(input_dept), stdin) != NULL)
    {
        rm_newl(input_dept, sizeof(input_dept));
        rm_extra_spaces(input_dept);
        if (strlen(input_dept) > 0)
        {
            if (strstr(input_dept, ".dat") != NULL)
            {
                strncpy(filepath, input_dept, sizeof(filepath) - 1);
                filepath[sizeof(filepath) - 1] = '\0';
            }
            else
            {
                snprintf(filepath, sizeof(filepath), "%s_teachers.dat", input_dept);
            }
        }
    }

    int buf_size = 10;
    TeacherLoginDta *ld_ptr = (TeacherLoginDta *)malloc(buf_size * sizeof(TeacherLoginDta));
    if (ld_ptr == NULL)
    {
        printf("\033[31mMemory allocation error\033[0m\n");
        return 0;
    }

    int n = decryptAndloadInmem_TeacherLoginData(filepath, &ld_ptr, buf_size);
    if (n <= 0)
    {
        printf("\033[31mCould not load teacher database from '%s'\033[0m\n", filepath);
        free(ld_ptr);
        return 0;
    }

    char input_name[32];
    char input_pass[16];

    printf("Enter Teacher Name : ");
    if (fgets(input_name, sizeof(input_name), stdin) == NULL)
    {
        free(ld_ptr);
        return 0;
    }
    rm_newl(input_name, sizeof(input_name));
    rm_extra_spaces(input_name);

    printf("Enter Password     : ");
    if (fgets(input_pass, sizeof(input_pass), stdin) == NULL)
    {
        free(ld_ptr);
        return 0;
    }
    rm_newl(input_pass, sizeof(input_pass));
    rm_extra_spaces(input_pass);

    for (int i = 0; i < n; i++)
    {
        if (strcasecmp(ld_ptr[i].name, input_name) == 0 && strcmp(ld_ptr[i].password, input_pass) == 0)
        {
            memcpy(teacher_out, &ld_ptr[i], sizeof(TeacherLoginDta));
            printf("\n\033[32m[✓] Login Successful! Welcome, %s (%s, Dept: %s)\033[0m\n",
                   teacher_out->name, teacher_out->designation_code, teacher_out->dept_code);
            free(ld_ptr);
            return 1;
        }
    }

    printf("\n\033[31m[✗] Authentication failed: Incorrect Name or Password\033[0m\n");
    free(ld_ptr);
    return 0;
}

int admin_login(void)
{
    char password[32];
    printf("Enter Admin Password [default: admin]: ");
    if (fgets(password, sizeof(password), stdin) == NULL)
    {
        return 0;
    }
    rm_newl(password, sizeof(password));
    rm_extra_spaces(password);

    if (strcmp(password, "admin") == 0 || strcmp(password, "admin123") == 0)
    {
        printf("\n\033[32m[✓] Admin Login Successful!\033[0m\n");
        return 1;
    }

    printf("\n\033[31m[✗] Incorrect Admin Password!\033[0m\n");
    return 0;
}

void handle_admin_menu(void)
{
    while (1)
    {
        clear_screen();
        print_banner();
        printf("==================================================\n");
        printf("              ADMINISTRATOR DASHBOARD             \n");
        printf("==================================================\n");
        printf("1. Encrypt Student Login Data (from text file)\n");
        printf("2. Decrypt & Export Student Login Data to text file\n");
        printf("3. Append Student to Encrypted Database\n");
        printf("4. Encrypt Teacher Login Data (from text file)\n");
        printf("5. Decrypt & Export Teacher Login Data to text file\n");
        printf("6. Append Teacher to Encrypted Database\n");
        printf("7. Exit to Main Menu\n\n");

        int opt = 0;
        printf("Enter option (1-7): ");
        if (scanf("%d", &opt) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (opt)
        {
        case 1:
        {
            char path[64];
            printf("Enter filepath of student login text file (e.g. stud_data.txt): ");
            if (fgets(path, sizeof(path), stdin) != NULL)
            {
                rm_newl(path, sizeof(path));
                rm_extra_spaces(path);
                int count = encrypt_StudentLoginData(path);
                if (count >= 0)
                {
                    printf("\033[32mSuccessfully processed %d student records!\033[0m\n", count);
                }
            }
            printf("Press Enter to continue...\n");
            getchar();
            break;
        }
        case 2:
        {
            char path[64];
            printf("Enter filepath of encrypted student data (e.g. CSE-2025-B-B1.dat): ");
            if (fgets(path, sizeof(path), stdin) != NULL)
            {
                rm_newl(path, sizeof(path));
                rm_extra_spaces(path);
                int count = decryptAndsave_StudentLoginData(path);
                if (count >= 0)
                {
                    printf("\033[32mSuccessfully decrypted and exported %d records!\033[0m\n", count);
                }
            }
            printf("Press Enter to continue...\n");
            getchar();
            break;
        }
        case 3:
        {
            char path[64];
            printf("Enter filepath of encrypted student data to append (e.g. CSE-2025-B-B1.dat): ");
            if (fgets(path, sizeof(path), stdin) != NULL)
            {
                rm_newl(path, sizeof(path));
                rm_extra_spaces(path);
                append_encrypted_StudentLoginData(path);
            }
            printf("Press Enter to continue...\n");
            getchar();
            break;
        }
        case 4:
        {
            char path[64];
            printf("Enter filepath of teacher login text file (e.g. teacher_login.txt): ");
            if (fgets(path, sizeof(path), stdin) != NULL)
            {
                rm_newl(path, sizeof(path));
                rm_extra_spaces(path);
                int count = encrypt_TeacherLoginData(path);
                if (count >= 0)
                {
                    printf("\033[32mSuccessfully processed %d teacher records!\033[0m\n", count);
                }
            }
            printf("Press Enter to continue...\n");
            getchar();
            break;
        }
        case 5:
        {
            char path[64];
            printf("Enter filepath of encrypted teacher data (e.g. CSE_teachers.dat): ");
            if (fgets(path, sizeof(path), stdin) != NULL)
            {
                rm_newl(path, sizeof(path));
                rm_extra_spaces(path);
                int count = decryptAndsave_TeacherLoginData(path);
                if (count >= 0)
                {
                    printf("\033[32mSuccessfully decrypted and exported %d records!\033[0m\n", count);
                }
            }
            printf("Press Enter to continue...\n");
            getchar();
            break;
        }
        case 6:
        {
            char path[64];
            printf("Enter filepath of encrypted teacher data to append (e.g. CSE_teachers.dat): ");
            if (fgets(path, sizeof(path), stdin) != NULL)
            {
                rm_newl(path, sizeof(path));
                rm_extra_spaces(path);
                append_encrypted_TeacherLoginData(path);
            }
            printf("Press Enter to continue...\n");
            getchar();
            break;
        }
        case 7:
            return;
        default:
            printf("Invalid option. Press Enter to continue...\n");
            getchar();
            break;
        }
    }
}
