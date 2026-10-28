#include "../include/tf.h"

#define qgets(buf, fstream) fgets(buf, sizeof(buf), fstream)

int encrypt_tf_question(char original_filepath[], char encrypted_filepath[], QuestionHeader *q_head)
{
    /* Encrypts True/False questions written in format:
       Question statement
       Answer (T or F / True or False)
    */
    int ques_count = 0;
    FILE *unencrypted_ques = fopen(original_filepath, "r");
    if (unencrypted_ques == NULL)
    {
        print_file_err();
        return -1;
    }

    FILE *encrypted_ques = fopen(encrypted_filepath, "wb");
    if (encrypted_ques == NULL)
    {
        print_file_err();
        fclose(unencrypted_ques);
        return -1;
    }

    TFQuestion q[100];
    memset(q, 0, sizeof(q));

    int i = 0;
    while (i < 100 && qgets(q[i].ques, unencrypted_ques) != NULL)
    {
        rm_newl(q[i].ques, TF_QUES_SIZE);
        rm_extra_spaces(q[i].ques);
        if (strlen(q[i].ques) == 0)
        {
            continue; // Skip empty lines between questions
        }

        if (qgets(q[i].ans, unencrypted_ques) == NULL)
        {
            printf("\033[31mFormat error: Missing answer for question %d\033[0m\n", i + 1);
            fclose(unencrypted_ques);
            fclose(encrypted_ques);
            return -1;
        }
        rm_newl(q[i].ans, TF_ANS_SIZE);
        rm_extra_spaces(q[i].ans);

        // Standardize answer to 'T' or 'F'
        if (toupper((unsigned char)q[i].ans[0]) == 'T')
        {
            strcpy(q[i].ans, "T");
        }
        else if (toupper((unsigned char)q[i].ans[0]) == 'F')
        {
            strcpy(q[i].ans, "F");
        }

        i++;
    }
    ques_count = i;

    if (ques_count == 0)
    {
        printf("\033[31mNo questions found in file\033[0m\n");
        fclose(unencrypted_ques);
        fclose(encrypted_ques);
        return -1;
    }

    if (ques_count >= 100)
    {
        printf("\033[31mMaximum question limit (100) reached\033[0m\n");
    }

    fclose(unencrypted_ques);

    encrypt_affine((unsigned char *)q_head->quiz_code, 16, 5, 2);
    encrypt_affine((unsigned char *)q_head->seconds_per_ques, 4, 5, 2);
    fwrite(q_head, sizeof(QuestionHeader), 1, encrypted_ques);

    for (int j = 0; j < ques_count; j++)
    {
        encrypt_affine((unsigned char *)q[j].ques, TF_QUES_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[j].ans, TF_ANS_SIZE, 5, 2);
        fwrite(&q[j], sizeof(TFQuestion), 1, encrypted_ques);
    }

    fclose(encrypted_ques);
    return ques_count;
}

int decryptAndload_tf_question(char encrypted_filepath[], TFQuestion *q, QuestionHeader *q_head_ptr)
{
    FILE *encrypted_ques = fopen(encrypted_filepath, "rb");
    if (encrypted_ques == NULL)
    {
        print_file_err();
        return -1;
    }

    if (fread(q_head_ptr, sizeof(QuestionHeader), 1, encrypted_ques) != 1)
    {
        printf("\033[31mError reading question header\033[0m\n");
        fclose(encrypted_ques);
        return -1;
    }

    decrypt_affine((unsigned char *)q_head_ptr->quiz_code, 16, 5, 2);
    decrypt_affine((unsigned char *)q_head_ptr->seconds_per_ques, 4, 5, 2);

    int i = 0;
    while (fread(q + i, sizeof(TFQuestion), 1, encrypted_ques) != 0)
    {
        decrypt_affine((unsigned char *)q[i].ques, TF_QUES_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].ans, TF_ANS_SIZE, 5, 2);
        i++;
    }

    fclose(encrypted_ques);
    return i;
}

void run_tf_quiz(TFQuestion *decrypted_q_ptr, QuizResult *qr_ptr, int ques_count, QuestionHeader *ques_head)
{
    strcpy(qr_ptr->quiz_code, ques_head->quiz_code);

    int sec_per_q = 30;
    sscanf(ques_head->seconds_per_ques, "%d", &sec_per_q);
    if (sec_per_q <= 0)
    {
        sec_per_q = 30;
    }

    time_t quiz_time = sec_per_q * ques_count;
    time_t quiz_start_time = time(NULL);
    time_t cur_time;

    printf("\n==================================================\n");
    printf("Quiz Code        : %s\n", ques_head->quiz_code);
    printf("Total Questions  : %d\n", ques_count);
    printf("Time Allocated   : %ld seconds (%d sec/question)\n", (long)quiz_time, sec_per_q);
    printf("==================================================\n\n");

    qr_ptr->total_marks = ques_count;
    qr_ptr->correct_ans = 0;
    qr_ptr->wrong_ans = 0;
    qr_ptr->ignored = 0;

    for (int i = 0; i < ques_count; i++)
    {
        printf("\nQ%d: %s\n", i + 1, decrypted_q_ptr[i].ques);
        printf("[T]rue  /  [F]alse\n");

        char ans_by_stud[16];
        printf("Ans (T/F) [Enter 0 to ignore]: ");

        if (fgets(ans_by_stud, sizeof(ans_by_stud), stdin) == NULL)
        {
            break;
        }

        cur_time = time(NULL);

        if (difftime(cur_time, quiz_start_time) > quiz_time)
        {
            printf("\n\033[31mAllocated time for the quiz has expired!\033[0m\n");
            break;
        }

        rm_newl(ans_by_stud, sizeof(ans_by_stud));
        rm_extra_spaces(ans_by_stud);

        if (ans_by_stud[0] == '0' || strlen(ans_by_stud) == 0)
        {
            printf("\033[33mIgnored\033[0m\n");
            qr_ptr->ignored++;
        }
        else if (toupper((unsigned char)ans_by_stud[0]) == toupper((unsigned char)decrypted_q_ptr[i].ans[0]))
        {
            printf("\033[32mCorrect!\033[0m\n");
            qr_ptr->correct_ans++;
        }
        else
        {
            printf("\033[31mIncorrect answer!\033[0m\n");
            printf("Correct Answer : %s (%s)\n",
                   decrypted_q_ptr[i].ans,
                   toupper((unsigned char)decrypted_q_ptr[i].ans[0]) == 'T' ? "True" : "False");
            qr_ptr->wrong_ans++;
        }
    }

    qr_ptr->ignored += qr_ptr->total_marks - (qr_ptr->correct_ans + qr_ptr->wrong_ans + qr_ptr->ignored);
    qr_ptr->accuracy = (qr_ptr->total_marks > 0) ? ((qr_ptr->correct_ans * 100.00f) / qr_ptr->total_marks) : 0.0f;

    printf("\n==================================================\n");
    printf("                  QUIZ RESULT                     \n");
    printf("==================================================\n");
    printf("Total Questions : %d\n", qr_ptr->total_marks);
    printf("Correct Answers : %d\n", qr_ptr->correct_ans);
    printf("Wrong Answers   : %d\n", qr_ptr->wrong_ans);
    printf("Ignored         : %d\n", qr_ptr->ignored);
    printf("Accuracy        : %.2f%%\n", qr_ptr->accuracy);
    printf("==================================================\n\n");
}

