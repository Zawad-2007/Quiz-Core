#include"../include/stud_menu.h"

void handle_stud_menu(char *encr_ques_filepath, Question *ques_ptr, QuizResult *qr_ptr)
{
    int stud_menu_opt;
    printf("Enter option (1/2/3/4) : ");
    scanf("%d", &stud_menu_opt);
    getchar();

    switch(stud_menu_opt)
    {
        case 1:
        {
            printf("Enter filepath of encrypted questions :");
            fgets(encr_ques_filepath, 64, stdin);
            rm_newl(encr_ques_filepath, 64);

            QuestionHeader ques_head;

            int q_count = decryptAndload_mcq_question(encr_ques_filepath, ques_ptr, &ques_head);
            if (q_count < 100)
            {
                // Resize the memory block down to the exact count needed
                Question *temp = realloc(ques_ptr, q_count * sizeof(Question));
                if (temp != NULL)
                {
                    ques_ptr = temp;
                }
            }

            start_mcq_quiz(qr_ptr);
            run_mcq_quiz(ques_ptr, qr_ptr, q_count, &ques_head);
            storeAndencrypt_progress_in_file(qr_ptr);
            decryptAndload_progress_in_mem();
            free(ques_ptr);
            getchar();
            break;
        }
        case 2:
        {
            // Will take Input of name, roll.
            // Read every QuizResultStr on progress.dat
            // Compare QuizResultStr's information with given name and roll and will show every matching records
        }
    }
}