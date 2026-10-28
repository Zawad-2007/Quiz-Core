#include "../include/teacher_menu.h"

void handle_teacher_menu(TeacherLoginDta *teacher)
{
    while (1)
    {
        clear_screen();
        print_banner();
        printf("==================================================\n");
        printf("                 TEACHER DASHBOARD                \n");
        printf("  Logged in as: %s (%s, Dept: %s)\n",
               teacher->name, teacher->designation_code, teacher->dept_code);
        printf("==================================================\n");
        printf("1. Encrypt questions (create new quiz)\n");
        printf("2. Watch student progress\n");
        printf("3. Decrypt & export progress file\n");
        printf("4. Logout & return to main menu\n\n");

        int teacher_menu_opt = 0;
        printf("Enter option (1-4): ");
        if (scanf("%d", &teacher_menu_opt) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (teacher_menu_opt)
        {
        case 1:
        {
            clear_screen();
            print_banner();
            printf("==================================================\n");
            printf("             SELECT QUESTION TYPE TO ENCRYPT      \n");
            printf("==================================================\n");
            printf("1. Multiple Choice Questions (MCQ)\n");
            printf("2. True / False Questions\n");
            printf("3. Fill in the Blanks Questions\n");
            printf("4. Back to Teacher Dashboard\n\n");

            int qtype = 0;
            printf("Select type (1-4): ");
            if (scanf("%d", &qtype) != 1)
            {
                while (getchar() != '\n');
                continue;
            }
            getchar();

            if (qtype == 4)
            {
                break;
            }
            if (qtype < 1 || qtype > 4)
            {
                printf("Invalid selection!\n");
                printf("Press Enter to continue...");
                getchar();
                break;
            }

            char og_ques_filepath[64];
            printf("Enter filepath of original questions: ");
            if (fgets(og_ques_filepath, sizeof(og_ques_filepath), stdin) == NULL)
            {
                break;
            }
            rm_newl(og_ques_filepath, sizeof(og_ques_filepath));
            rm_extra_spaces(og_ques_filepath);

            char encr_ques_filepath[64];
            printf("Enter filepath for output encrypted file: ");
            if (fgets(encr_ques_filepath, sizeof(encr_ques_filepath), stdin) == NULL)
            {
                break;
            }
            rm_newl(encr_ques_filepath, sizeof(encr_ques_filepath));
            rm_extra_spaces(encr_ques_filepath);

            QuestionHeader ques_head;
            memset(&ques_head, 0, sizeof(QuestionHeader));

            printf("Enter quiz code (e.g. CSE-1107): ");
            if (fgets(ques_head.quiz_code, sizeof(ques_head.quiz_code), stdin) == NULL)
            {
                break;
            }
            rm_newl(ques_head.quiz_code, sizeof(ques_head.quiz_code));
            rm_extra_spaces(ques_head.quiz_code);

            printf("Enter allocated time per question in seconds (e.g. 30): ");
            if (fgets(ques_head.seconds_per_ques, sizeof(ques_head.seconds_per_ques), stdin) == NULL)
            {
                break;
            }
            rm_newl(ques_head.seconds_per_ques, sizeof(ques_head.seconds_per_ques));
            rm_extra_spaces(ques_head.seconds_per_ques);

            int result = 0;
            if (qtype == 1)
            {
                result = encrypt_mcq_question(og_ques_filepath, encr_ques_filepath, &ques_head);
            }
            else if (qtype == 2)
            {
                result = encrypt_tf_question(og_ques_filepath, encr_ques_filepath, &ques_head);
            }
            else if (qtype == 3)
            {
                result = encrypt_blank_question(og_ques_filepath, encr_ques_filepath, &ques_head);
            }

            if (result > 0)
            {
                printf("\n\033[32m[✓] Successfully encrypted %d questions to '%s'!\033[0m\n",
                       result, encr_ques_filepath);
            }
            else
            {
                printf("\n\033[31m[✗] Failed to encrypt questions. Please check the file path and format.\033[0m\n");
            }

            printf("Press Enter to continue...");
            getchar();
            break;
        }
        case 2:
        {
            clear_screen();
            print_banner();
            printf("==================================================\n");
            printf("               STUDENT PROGRESS LOG               \n");
            printf("==================================================\n");

            char progress_filepath[64] = "progress.dat";
            char input_path[64];
            printf("Enter filepath of progress file [default: progress.dat]: ");
            if (fgets(input_path, sizeof(input_path), stdin) != NULL)
            {
                rm_newl(input_path, sizeof(input_path));
                rm_extra_spaces(input_path);
                if (strlen(input_path) > 0)
                {
                    strncpy(progress_filepath, input_path, sizeof(progress_filepath) - 1);
                    progress_filepath[sizeof(progress_filepath) - 1] = '\0';
                }
            }

            show_all_progress_from_file(progress_filepath);
            printf("Press Enter to return to Teacher Dashboard...");
            getchar();
            break;
        }
        case 3:
        {
            clear_screen();
            print_banner();
            printf("==================================================\n");
            printf("             DECRYPT & EXPORT PROGRESS            \n");
            printf("==================================================\n");

            char encr_progress_filepath[64] = "progress.dat";
            char input_path[64];
            printf("Enter filepath of encrypted progress file [default: progress.dat]: ");
            if (fgets(input_path, sizeof(input_path), stdin) != NULL)
            {
                rm_newl(input_path, sizeof(input_path));
                rm_extra_spaces(input_path);
                if (strlen(input_path) > 0)
                {
                    strncpy(encr_progress_filepath, input_path, sizeof(encr_progress_filepath) - 1);
                    encr_progress_filepath[sizeof(encr_progress_filepath) - 1] = '\0';
                }
            }

            decrypt_and_save_progress(encr_progress_filepath);
            printf("Press Enter to return to Teacher Dashboard...");
            getchar();
            break;
        }
        case 4:
            printf("\nLogging out...\n");
            return;
        default:
            printf("Invalid option. Press Enter to continue...");
            getchar();
            break;
        }
    }
}
