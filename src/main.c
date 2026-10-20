#include <stdio.h>
#include <stdlib.h>
#include "../include/affine_cipher.h"
#include "../include/mathematics.h"
#include "../include/misc.h"
#include "../include/mcq.h"
#include "../include/ui.h"

int main()
{
    clear_screen();
    print_banner();
    char og_ques_filepath[64];
    printf("Enter filepath of original questions :");
    fgets(og_ques_filepath, 64, stdin);
    rm_newl(og_ques_filepath, 64);

    char encr_ques_filepath[64];
    printf("Enter filepath of encrypted questions :");
    fgets(encr_ques_filepath, 64, stdin);
    rm_newl(encr_ques_filepath, 64);

    QuizResult q;
    clear_screen();
    print_banner();
    int q_count = encrypt_mcq_question(og_ques_filepath, encr_ques_filepath);
    printf("Total questions : %d\n", q_count);
    q.total_marks = q_count;

    clear_screen();
    print_banner();
    printf("Name                : ");
    fgets(q.name, 32, stdin);
    rm_newl(q.name, 32);

    printf("Roll                : ");
    fgets(q.roll, 16, stdin);
    rm_newl(q.roll, 16);

    printf("Subject of the exam : ");
    fgets(q.subject, 32, stdin);
    rm_newl(q.subject, 32);

    Question decrypted_q[q_count];
    decryptAndload_mcq_question(encr_ques_filepath, decrypted_q);
    for (int i = 0; i < q_count; i++)
    {
        printf("%s\n", decrypted_q[i].ques);
        printf("%s%s%s%s\n", decrypted_q[i].opt_a, decrypted_q[i].opt_b, decrypted_q[i].opt_c, decrypted_q[i].opt_d);
        char *ans_by_stud = calloc(5, sizeof(char));
        if (ans_by_stud == NULL)
        {
            printf("\033[31mMemory Error\033[0m\n");
            return 1;
        }
        printf("Ans (A/B/C/D) :");
        fgets(ans_by_stud, 5, stdin);
        if (ans_by_stud[0] == decrypted_q[i].ans[0])
        {
            printf("Correct!\n");
            q.correct_ans++;
        }
        else
        {
            printf("Incorrect answer!\n");
            printf("Correct Answer :%s\n", decrypted_q[i].ans);
        }
        free(ans_by_stud);
    }

    q.wrong_ans = q.total_marks - q.correct_ans;
    q.accuracy = (q.correct_ans * 100.00) / q.total_marks;
    store_progress_in_file(&q);
}