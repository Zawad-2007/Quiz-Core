// #include <stdio.h>
// #include <stdlib.h>
// #include "../include/affine_cipher.h"
// #include "../include/mathematics.h"
// #include "../include/misc.h"
// #include "../include/mcq.h"
// #include "../include/ui.h"
// #include "../include/stud_menu.h"

// int main()
// {
//     while(1)
//     {
//         clear_screen();
//         print_banner();
//         print_main_menu();

//         int main_menu_opt;
//         printf("Enter option (1/2/3/4):");
//         scanf("%d", &main_menu_opt);
//         getchar();

//         switch (main_menu_opt)
//         {
//         case 1:
//         {
//             char encr_ques_filepath[64];
//             Question *q = malloc(100 * sizeof(Question));
//             QuizResult qr;

//             clear_screen();
//             print_banner();
//             print_student_menu();

//             handle_stud_menu(encr_ques_filepath, q, &qr);
//             break;
//         }
//         case 2:
//         {
//             char og_ques_filepath[64];
//             printf("Enter filepath of original questions :");
//             fgets(og_ques_filepath, 64, stdin);
//             rm_newl(og_ques_filepath, 64);

//             char encr_ques_filepath[64];
//             printf("Enter filepath of encrypted questions :");
//             fgets(encr_ques_filepath, 64, stdin);
//             rm_newl(encr_ques_filepath, 64);

//             QuestionHeader ques_head;
//             printf("Enter quiz code : ");
//             fgets(ques_head.quiz_code, 16, stdin);
//             rm_newl(ques_head.quiz_code, 16);

//             printf("Enter allocated time for every question in seconds :  ");
//             fgets(ques_head.seconds_per_ques, 4, stdin);
//             rm_newl(ques_head.seconds_per_ques, 4);

//             QuizResult qr;
//             clear_screen();
//             print_banner();
//             encrypt_mcq_question(og_ques_filepath, encr_ques_filepath, &ques_head);
//             break;
//         }
//         case 3:
//         {
//             printf("Comming soon\n");
//             break;
//         }
//         case 4:
//         {
//             clear_screen();
//             print_banner();
//             printf("Thank you for trying quiz-core\n");
//             return 0;
//         }
//         }
//     }
    
// }