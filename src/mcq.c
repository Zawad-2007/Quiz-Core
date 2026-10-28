
#include"../include/mcq.h"


#define qgets(buf,fstream) fgets(buf, sizeof(buf), fstream)

int encrypt_mcq_question(char original_filepath[], char encrypted_filepath[], QuestionHeader *q_head)
{ /*Encrypts quetsion written in this format:
 Question
 opt_a
 opt_b
 opt_c
 opt_d
 ans

 Stored in Struct named Question and then encrypted using affine cipher
 encrypted questions are stored in "encrypted_filepath" in binary format
 */
    int ques_count=0;
    FILE *unencrypted_ques = fopen(original_filepath, "rb");
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

    Question q[100];
    int i = 0;

    while (qgets(q[i].ques, unencrypted_ques)!=NULL)
    {
        qgets(q[i].opt_a, unencrypted_ques);
        qgets(q[i].opt_b, unencrypted_ques);
        qgets(q[i].opt_c, unencrypted_ques);
        qgets(q[i].opt_d, unencrypted_ques);
        qgets(q[i].ans, unencrypted_ques);
        i++;
    }
    ques_count = i;

    if(ques_count>=100)
    {
        printf("\033[31mMaximum Question limit(100) crossed\nAborting encryption...\033[0m\n");
        fclose(unencrypted_ques);
        fclose(encrypted_ques);
        return -1;
    }

    fclose(unencrypted_ques);

    
    encrypt_affine((unsigned char *)q_head->quiz_code, 16, 5, 2);
    encrypt_affine((unsigned char *)q_head->seconds_per_ques, 4, 5, 2);
    fwrite(q_head, sizeof(QuestionHeader), 1, encrypted_ques);

    for (int i = 0; i < ques_count; i++)
    {
        encrypt_affine((unsigned char *)q[i].ques, QUES_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_a, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_b, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_c, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].opt_d, OPT_SIZE, 5, 2);
        encrypt_affine((unsigned char *)q[i].ans, ANS_SIZE, 5, 2);
        fwrite(q + i, sizeof(Question), 1, encrypted_ques);
    }

    fclose(encrypted_ques);
    return ques_count;
}
int decryptAndload_mcq_question(char encrypted_filepath[], Question *q, QuestionHeader* q_head_ptr)
{/**/
    FILE *encrypted_ques = fopen(encrypted_filepath, "rb");
    if(encrypted_ques==NULL)
    {
        print_file_err();
        return -1;
    }

    if (fread(q_head_ptr, sizeof(QuestionHeader), 1, encrypted_ques) != 1)
    {
        fclose(encrypted_ques);
        return -1;
    }
    decrypt_affine((unsigned char *)q_head_ptr->quiz_code, 16, 5, 2);
    decrypt_affine((unsigned char *)q_head_ptr->seconds_per_ques, 4, 5, 2);

    int i = 0;
    while (fread(q + i,sizeof(Question),1,encrypted_ques)!=0)
    {
        decrypt_affine((unsigned char *)q[i].ques, QUES_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_a, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_b, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_c, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].opt_d, OPT_SIZE, 5, 2);
        decrypt_affine((unsigned char *)q[i].ans, ANS_SIZE, 5, 2);
        i++;
    }
    fclose(encrypted_ques);
    return i;
}

void start_mcq_quiz(QuizResult *qr_ptr)
{
    printf("Name                : ");
    fgets(qr_ptr->name, 32, stdin);
    rm_newl(qr_ptr->name, 32);

    printf("Roll                : ");
    fgets(qr_ptr->roll, 16, stdin);
    rm_newl(qr_ptr->roll, 16);

    printf("Subject of the exam : ");
    fgets(qr_ptr->subject, 32, stdin);
    rm_newl(qr_ptr->subject, 32);

    time_t cur_time = time(NULL);
    char *cur_time_str = ctime(&cur_time);
    rm_newl(cur_time_str, 32);
    strcpy(qr_ptr->quiz_date, cur_time_str);
}

void run_mcq_quiz(Question *decrypted_q_ptr, QuizResult *qr_ptr, int ques_count, QuestionHeader *ques_head)
{
    strcpy(qr_ptr->quiz_code, ques_head->quiz_code);

    int sec_per_q;
    sscanf(ques_head->seconds_per_ques, "%d", &sec_per_q);    // atoi only converts the first character to integer

    time_t quiz_time = sec_per_q * ques_count; 
    time_t quiz_start_time = time(NULL);
    time_t cur_time;

    printf("Total questions : %d\n", ques_count);
    qr_ptr->total_marks = ques_count;
    qr_ptr->correct_ans = 0;
    qr_ptr->wrong_ans = 0;
    qr_ptr->ignored = 0;

    for (int i = 0; i < ques_count; i++)
    {
        printf("\nQ%d: %s", i + 1, decrypted_q_ptr[i].ques);
        if (strchr(decrypted_q_ptr[i].ques, '\n') == NULL) printf("\n");
        printf("%s%s%s%s", decrypted_q_ptr[i].opt_a, decrypted_q_ptr[i].opt_b, decrypted_q_ptr[i].opt_c, decrypted_q_ptr[i].opt_d);
        if (strchr(decrypted_q_ptr[i].opt_d, '\n') == NULL) printf("\n");

        char ans_by_stud[8];
        printf("Ans (A/B/C/D) [Enter 0 to ignore] : ");

        if (fgets(ans_by_stud, sizeof(ans_by_stud), stdin) == NULL)
        {
            break; // Handle EOF safely
        }

        //Captures time immediately after user inputs the answer
        cur_time = time(NULL);

        // time limit check
        if (difftime(cur_time, quiz_start_time) > quiz_time)
        {
            printf("\n\033[31mGiven time for quiz has expired\033[0m\n");
            break;
        }

        // Validate answer safely
        if (ans_by_stud[0] == '0' || ans_by_stud[0] == '\n')
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
            printf("Correct Answer : %c\n", (char)toupper((unsigned char)decrypted_q_ptr[i].ans[0]));
            qr_ptr->wrong_ans++;
        }
    }

    qr_ptr->ignored += qr_ptr->total_marks - (qr_ptr->correct_ans + qr_ptr->wrong_ans + qr_ptr->ignored);  //Adds questions not answered because of time limit
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
