#include "../include/stud_menu.h"

void handle_stud_menu(StudentLoginDta *student)
{
    while (1)
    {
        clear_screen();
        print_banner();
        printf("==================================================\n");
        printf("                 STUDENT DASHBOARD                \n");
        printf("  Logged in as: %s (Roll: %s)\n", student->name, student->roll);
        printf("==================================================\n");
        printf("1. Start a quiz\n");
        printf("2. Watch my progress\n");
        printf("3. Logout & return to main menu\n\n");

        int stud_menu_opt = 0;
        printf("Enter option (1/2/3) : ");
        if (scanf("%d", &stud_menu_opt) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (stud_menu_opt)
        {
        case 1:
        {
            clear_screen();
            print_banner();
            printf("==================================================\n");
            printf("                SELECT QUIZ TYPE                  \n");
            printf("==================================================\n");
            printf("1. Multiple Choice Questions (MCQ)\n");
            printf("2. True / False Questions\n");
            printf("3. Fill in the Blanks Questions\n");
            printf("4. Back to Student Dashboard\n\n");

            int quiz_type = 0;
            printf("Select quiz type (1-4): ");
            if (scanf("%d", &quiz_type) != 1)
            {
                while (getchar() != '\n');
                continue;
            }
            getchar();

            if (quiz_type == 4)
            {
                break;
            }
            if (quiz_type < 1 || quiz_type > 4)
            {
                printf("Invalid selection!\n");
                printf("Press Enter to continue...");
                getchar();
                break;
            }

            char encr_ques_filepath[64];
            printf("\nEnter filepath of encrypted questions: ");
            if (fgets(encr_ques_filepath, sizeof(encr_ques_filepath), stdin) == NULL)
            {
                break;
            }
            rm_newl(encr_ques_filepath, sizeof(encr_ques_filepath));
            rm_extra_spaces(encr_ques_filepath);
            if (strlen(encr_ques_filepath) == 0)
            {
                print_file_err();
                printf("Press Enter to continue...");
                getchar();
                break;
            }

            FILE *f_check = fopen(encr_ques_filepath, "rb");
            if (f_check == NULL)
            {
                print_file_err();
                printf("Press Enter to continue...");
                getchar();
                break;
            }
            fclose(f_check);

            QuizResult qr;
            memset(&qr, 0, sizeof(QuizResult));
            strncpy(qr.name, student->name, sizeof(qr.name) - 1);
            strncpy(qr.roll, student->roll, sizeof(qr.roll) - 1);

            printf("Enter subject of the exam: ");
            char subject_buf[32];
            if (fgets(subject_buf, sizeof(subject_buf), stdin) != NULL)
            {
                rm_newl(subject_buf, sizeof(subject_buf));
                rm_extra_spaces(subject_buf);
                if (strlen(subject_buf) > 0)
                {
                    strncpy(qr.subject, subject_buf, sizeof(qr.subject) - 1);
                }
                else
                {
                    strcpy(qr.subject, "General");
                }
            }
            else
            {
                strcpy(qr.subject, "General");
            }

            time_t cur_time = time(NULL);
            char *cur_time_str = ctime(&cur_time);
            if (cur_time_str != NULL)
            {
                rm_newl(cur_time_str, 32);
                strncpy(qr.quiz_date, cur_time_str, sizeof(qr.quiz_date) - 1);
            }

            QuestionHeader ques_head;

            if (quiz_type == 1)
            {
                Question *ques_ptr = malloc(100 * sizeof(Question));
                if (ques_ptr == NULL)
                {
                    printf("\033[31mMemory allocation error\033[0m\n");
                    break;
                }
                int q_count = decryptAndload_mcq_question(encr_ques_filepath, ques_ptr, &ques_head);
                if (q_count > 0)
                {
                    clear_screen();
                    print_banner();
                    run_mcq_quiz(ques_ptr, &qr, q_count, &ques_head);
                    storeAndencrypt_progress_in_file(&qr);
                    printf("\033[32mProgress recorded successfully to progress.dat!\033[0m\n");
                }
                free(ques_ptr);
            }
            else if (quiz_type == 2)
            {
                TFQuestion *tf_ques_ptr = malloc(100 * sizeof(TFQuestion));
                if (tf_ques_ptr == NULL)
                {
                    printf("\033[31mMemory allocation error\033[0m\n");
                    break;
                }
                int q_count = decryptAndload_tf_question(encr_ques_filepath, tf_ques_ptr, &ques_head);
                if (q_count > 0)
                {
                    clear_screen();
                    print_banner();
                    run_tf_quiz(tf_ques_ptr, &qr, q_count, &ques_head);
                    storeAndencrypt_progress_in_file(&qr);
                    printf("\033[32mProgress recorded successfully to progress.dat!\033[0m\n");
                }
                free(tf_ques_ptr);
            }
            else if (quiz_type == 3)
            {
                BlankQuestion *blank_ques_ptr = malloc(100 * sizeof(BlankQuestion));
                if (blank_ques_ptr == NULL)
                {
                    printf("\033[31mMemory allocation error\033[0m\n");
                    break;
                }
                int q_count = decryptAndload_blank_question(encr_ques_filepath, blank_ques_ptr, &ques_head);
                if (q_count > 0)
                {
                    clear_screen();
                    print_banner();
                    run_blank_quiz(blank_ques_ptr, &qr, q_count, &ques_head);
                    storeAndencrypt_progress_in_file(&qr);
                    printf("\033[32mProgress recorded successfully to progress.dat!\033[0m\n");
                }
                free(blank_ques_ptr);
            }

            printf("\nPress Enter to return to Student Dashboard...");
            getchar();
            break;
        }
        case 2:
        {
            clear_screen();
            print_banner();
            printf("==================================================\n");
            printf("          MY QUIZ PERFORMANCE HISTORY             \n");
            printf("  Student: %s (Roll: %s)\n", student->name, student->roll);
            printf("==================================================\n");

            show_student_progress_from_file("progress.dat", student->roll);

            printf("Press Enter to return to Student Dashboard...");
            getchar();
            break;
        }
        case 3:
            printf("\nLogging out...\n");
            return;
        default:
            printf("Invalid option. Press Enter to continue...");
            getchar();
            break;
        }
    }
}
