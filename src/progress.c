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
        return;
    }

    QuizResultStr *progress = malloc(sizeof(QuizResultStr));

    int record_count = 0;
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

    record_count = i; // record count
    int record_left = 256 - record_count;

    if (record_count < 256)
    {
        printf("Records will be reset after %d quiz\n", record_left);
    }
    else if (record_count == 256)
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
    if (record_count >= 256)
    { // resets the record in the end if the maximum limit crosses
        FILE *reset_record = fopen("progress.dat", "wb");
        if (reset_record == NULL)
        {
            printf("\033[31mFile Error\033[0m\n");
            return;
        }
        fclose(reset_record);
    }
    free(progress);
}

void migrate_oldRecord_toNew()
{
    char date[12];
    get_date_str(date, sizeof(date));

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

void show_all_progress_from_file(char *encr_progress_filepath)
{
    FILE *progress = fopen(encr_progress_filepath, "rb");
    QuizResultStr qr_str;
    int i = 0;

    printf("%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s\n", "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Correct", "Wrong", "Ignored", "Accuracy");

    while (fread(&qr_str, sizeof(QuizResultStr), 1, progress) != 0)
    {
        decrypt_affine((unsigned char *)qr_str.name, sizeof(qr_str.name), 5, 2);
        decrypt_affine((unsigned char *)qr_str.roll, sizeof(qr_str.roll), 5, 2);
        decrypt_affine((unsigned char *)qr_str.subject, sizeof(qr_str.subject), 5, 2);
        decrypt_affine((unsigned char *)qr_str.quiz_code, sizeof(qr_str.quiz_code), 5, 2);
        decrypt_affine((unsigned char *)qr_str.quiz_date, sizeof(qr_str.quiz_date), 5, 2);
        decrypt_affine((unsigned char *)qr_str.total_marks, sizeof(qr_str.total_marks), 5, 2);
        decrypt_affine((unsigned char *)qr_str.correct_ans, sizeof(qr_str.correct_ans), 5, 2);
        decrypt_affine((unsigned char *)qr_str.wrong_ans, sizeof(qr_str.wrong_ans), 5, 2);
        decrypt_affine((unsigned char *)qr_str.ignored, sizeof(qr_str.ignored), 5, 2);
        decrypt_affine((unsigned char *)qr_str.accuracy, sizeof(qr_str.accuracy), 5, 2);

        printf("%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s%%\n",
               qr_str.name,
               qr_str.roll,
               qr_str.subject,
               qr_str.quiz_code,
               qr_str.quiz_date,
               qr_str.total_marks,
               qr_str.correct_ans,
               qr_str.wrong_ans,
               qr_str.ignored,
               qr_str.accuracy);
        i++;
    }
    printf("Total records : %d\n", i);
}

int decrypt_and_save_progress(char *encr_progress_filepath)
{
    FILE *encr_progress = fopen(encr_progress_filepath, "rb");

    char decr_progress_filepath[64]="progress";
    char date[16];
    get_date_str(date, sizeof(date));
    strcat(decr_progress_filepath, date);
    strcat(decr_progress_filepath, ".txt");

    FILE *decr_progress = fopen(decr_progress_filepath, "wb");
    QuizResultStr qr_str_buf;

    fprintf(decr_progress, "%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s\n", "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Correct", "Wrong", "Ignored", "Accuracy");
    while (fread(&qr_str_buf, sizeof(QuizResultStr), 1, encr_progress) != 0)
    {
        decrypt_affine((unsigned char *)qr_str_buf.name, sizeof(qr_str_buf.name), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.roll, sizeof(qr_str_buf.roll), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.subject, sizeof(qr_str_buf.subject), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.quiz_code, sizeof(qr_str_buf.quiz_code), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.quiz_date, sizeof(qr_str_buf.quiz_date), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.total_marks, sizeof(qr_str_buf.total_marks), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.correct_ans, sizeof(qr_str_buf.correct_ans), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.wrong_ans, sizeof(qr_str_buf.wrong_ans), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.ignored, sizeof(qr_str_buf.ignored), 5, 2);
        decrypt_affine((unsigned char *)qr_str_buf.accuracy, sizeof(qr_str_buf.accuracy), 5, 2);
        fprintf(decr_progress, "%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s%%\n",
                qr_str_buf.name,
                qr_str_buf.roll,
                qr_str_buf.subject,
                qr_str_buf.quiz_code,
                qr_str_buf.quiz_date,
                qr_str_buf.total_marks,
                qr_str_buf.correct_ans,
                qr_str_buf.wrong_ans,
                qr_str_buf.ignored,
                qr_str_buf.accuracy);
    }
    if (encr_progress == NULL || decr_progress == NULL)
    {
        return 1;
    }
    else    
    {
        return 0;
    }
}