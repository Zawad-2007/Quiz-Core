#include"../include/teacher_menu.h"

void handle_teacher_menu()
{
    int teacher_menu_opt;
    printf("Enter option : ");
    scanf("%d", &teacher_menu_opt);
    getchar();

    switch(teacher_menu_opt)
    {
        case 1:
        {
            char og_ques_filepath[64];
            printf("Enter filepath of original questions :");
            fgets(og_ques_filepath, 64, stdin);
            rm_newl(og_ques_filepath, 64);

            char encr_ques_filepath[64];
            printf("Enter filepath of encrypted questions :");
            fgets(encr_ques_filepath, 64, stdin);
            rm_newl(encr_ques_filepath, 64);

            QuestionHeader ques_head;
            printf("Enter quiz code : ");
            fgets(ques_head.quiz_code, 16, stdin);
            rm_newl(ques_head.quiz_code, 16);
            printf("Enter allocated time for every question in seconds :  ");
            fgets(ques_head.seconds_per_ques, 4, stdin);
            rm_newl(ques_head.seconds_per_ques, 4);

            clear_screen();
            print_banner();
            encrypt_mcq_question(og_ques_filepath, encr_ques_filepath, &ques_head);
            printf("Press Enter to exit\n");
            getchar();
            break;
        }
        case 2:
        {
            char progress_filepath[64];
            printf("Enter filepath of progress file : ");

            fgets(progress_filepath, 64, stdin);
            rm_newl(progress_filepath, 64);

            show_all_progress_from_file(progress_filepath);
            printf("Press Enter to exit\n");
            getchar();
            break;
        }
        case 3:
        {
            char encr_progress_filepath[64];
            printf("Enter the filepath of encrypted progress file : ");
            fgets(encr_progress_filepath, 64, stdin);
            rm_newl(encr_progress_filepath, 64);

            decrypt_and_save_progress(encr_progress_filepath);
        }
    }
}