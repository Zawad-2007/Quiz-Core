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

    QuizResult qr;
    clear_screen();
    print_banner();
    encrypt_mcq_question(og_ques_filepath, encr_ques_filepath);

    clear_screen();
    print_banner();
    start_mcq_quiz(&qr);

    Question *decrypted_q = malloc(100 * sizeof(Question));
    int q_count = decryptAndload_mcq_question(encr_ques_filepath, decrypted_q);

    if (q_count < 100)
    {
        // Resize the memory block down to the exact count needed
        Question *temp = realloc(decrypted_q, q_count * sizeof(Question));
        if (temp != NULL)
        {
            decrypted_q = temp;
        }
    }

    run_mcq_quiz(decrypted_q, &qr, q_count);
    storeAndencrypt_progress_in_file(&qr);
    decryptAndload_progress_in_mem();
    free(decrypted_q);
}