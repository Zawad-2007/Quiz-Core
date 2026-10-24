#include"../include/progress.h"

void storeAndencrypt_progress_in_file(QuizResult *progress)
{
    FILE *file_exists = fopen("progress.dat", "rb");

    if (!(file_exists))
    {
        FILE *fp = fopen("progress.dat", "wb");
        if (fp != NULL)
        {
            fclose(fp);
        }
    }
    else
    {
        fclose(file_exists);
    }

    FILE *progress_file = fopen("progress.dat", "ab");
    if (progress_file == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return;
    }

    QuizResultStr qr_str;
    char t_marks[8], c_ans[8], w_ans[8], ignored[8], acc[8];
    snprintf(t_marks, 8, "%d", progress->total_marks);
    snprintf(c_ans, 8, "%d", progress->correct_ans);
    snprintf(w_ans, 8, "%d", progress->wrong_ans);
    snprintf(ignored, 8, "%d", progress->ignored);
    snprintf(acc, 8, "%.2f", progress->accuracy);

    strcpy(qr_str.name, progress->name);
    strcpy(qr_str.roll, progress->roll);
    strcpy(qr_str.subject, progress->subject);
    strcpy(qr_str.quiz_code, progress->quiz_code);
    strcpy(qr_str.quiz_date, progress->quiz_date);
    strcpy(qr_str.total_marks, t_marks);
    strcpy(qr_str.correct_ans, c_ans);
    strcpy(qr_str.wrong_ans, w_ans);
    strcpy(qr_str.ignored, ignored);
    strcpy(qr_str.accuracy, acc);

    encrypt_affine((unsigned char *)qr_str.name, 32, 5, 2);
    encrypt_affine((unsigned char *)qr_str.roll, 16, 5, 2);
    encrypt_affine((unsigned char *)qr_str.subject, 32, 5, 2);
    encrypt_affine((unsigned char *)qr_str.quiz_code, 16, 5, 2);
    encrypt_affine((unsigned char *)qr_str.quiz_date, 32, 5, 2);
    encrypt_affine((unsigned char *)qr_str.total_marks, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.correct_ans, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.wrong_ans, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.ignored, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.accuracy, 8, 5, 2);

    fwrite(&qr_str, sizeof(QuizResultStr), 1, progress_file);

    fclose(progress_file);
}

void decryptAndload_progress_in_mem()
{
    FILE *encr_progress_file = fopen("progress.dat", "rb");
    if (encr_progress_file == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        fclose(encr_progress_file);
        return;
    }

    QuizResultStr *progress = malloc(sizeof(QuizResultStr));

    int r_count = 0;
    int i = 0;
    while (fread(progress, sizeof(QuizResultStr), 1, encr_progress_file) != 0)
    {
        decrypt_affine((unsigned char *)progress->name, 32, 5, 2);
        decrypt_affine((unsigned char *)progress->roll, 16, 5, 2);
        decrypt_affine((unsigned char *)progress->subject, 32, 5, 2);
        decrypt_affine((unsigned char *)progress->quiz_code, 16, 5, 2);
        decrypt_affine((unsigned char *)progress->quiz_date, 32, 5, 2);
        decrypt_affine((unsigned char *)progress->total_marks, 8, 5, 2);
        decrypt_affine((unsigned char *)progress->correct_ans, 8, 5, 2);
        decrypt_affine((unsigned char *)progress->wrong_ans, 8, 5, 2);
        decrypt_affine((unsigned char *)progress->ignored, 8, 5, 2);
        decrypt_affine((unsigned char *)progress->accuracy, 8, 5, 2);
        i++;
    }

    fclose(encr_progress_file);

    r_count = i; // record count
    int record_left = 256 - r_count;

    if (r_count < 256)
    {
        printf("Records will be reset after %d quiz\n", record_left);
    }
    else if (r_count == 256)
    {
        printf("\033[31mRecord limit maxed out\033[0m\n");
        printf("\033[32mThe progress record will be reset\nPrevious record will be saved in a file\033[0m\n");
        migrate_oldRecord_toNew();
    }

    printf("%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s\n", "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Correct", "Wrong", "Ignored", "Accuracy");
    printf("%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s%%\n",
           progress->name,
           progress->roll,
           progress->subject,
           progress->quiz_code,
           progress->quiz_date,
           progress->total_marks,
           progress->correct_ans,
           progress->wrong_ans,
           progress->ignored,
           progress->accuracy);
    if (r_count >= 256)
    { // resets the record in the end if the maximum limit crosses
        FILE *reset_record = fopen("progress.dat", "wb");
        if (reset_record == NULL)
        {
            printf("\033[31mFile Error\033[0m\n");
            fclose(reset_record);
            return;
        }
        fclose(reset_record);
    }
    free(progress);
}

void migrate_oldRecord_toNew()
{
    time_t cur_time = time(NULL);

    struct tm *l_time_struct_ptr = localtime(&cur_time);

    int year = l_time_struct_ptr->tm_year + 1900; // tm_year counts from 1900
    int month = l_time_struct_ptr->tm_mon + 1;    // tm_mon's month index starts from 0
    int day = l_time_struct_ptr->tm_mday;         // tm_mday's day index starts from 1, so no additional increment

    char date[12];

    snprintf(date, sizeof(date), "%02d-%02d-%4d", day, month, year);

    char new_file_filepath[18] = {0};
    strcpy(new_file_filepath, date);
    strcat(new_file_filepath, ".dat");

    FILE *old_file = fopen("progress.dat", "rb");
    FILE *new_file = fopen(new_file_filepath, "wb");

    QuizResultStr buffer;

    while (fread(&buffer, sizeof(QuizResultStr), 1, old_file) != 0)
    {
        fwrite(&buffer, sizeof(QuizResultStr), 1, new_file);
    }

    fclose(old_file);
    fclose(new_file);
}