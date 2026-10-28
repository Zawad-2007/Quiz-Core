#include "../include/progress.h"

void storeAndencrypt_progress_in_file(QuizResult *progress)
{
    FILE *progress_file = fopen("progress.dat", "ab");
    if (progress_file == NULL)
    {
        print_file_err();
        return;
    }

    QuizResultStr qr_str;
    char t_marks[8], c_ans[8], w_ans[8], ignored[8], acc[8];
    snprintf(t_marks, 8, "%d", progress->total_marks);
    snprintf(c_ans, 8, "%d", progress->correct_ans);
    snprintf(w_ans, 8, "%d", progress->wrong_ans);
    snprintf(ignored, 8, "%d", progress->ignored);
    snprintf(acc, 8, "%.2f", progress->accuracy);

    strncpy(qr_str.name, progress->name, sizeof(qr_str.name) - 1);
    qr_str.name[sizeof(qr_str.name) - 1] = '\0';

    strncpy(qr_str.roll, progress->roll, sizeof(qr_str.roll) - 1);
    qr_str.roll[sizeof(qr_str.roll) - 1] = '\0';

    strncpy(qr_str.subject, progress->subject, sizeof(qr_str.subject) - 1);
    qr_str.subject[sizeof(qr_str.subject) - 1] = '\0';

    strncpy(qr_str.quiz_code, progress->quiz_code, sizeof(qr_str.quiz_code) - 1);
    qr_str.quiz_code[sizeof(qr_str.quiz_code) - 1] = '\0';

    strncpy(qr_str.quiz_date, progress->quiz_date, sizeof(qr_str.quiz_date) - 1);
    qr_str.quiz_date[sizeof(qr_str.quiz_date) - 1] = '\0';

    strncpy(qr_str.total_marks, t_marks, sizeof(qr_str.total_marks) - 1);
    qr_str.total_marks[sizeof(qr_str.total_marks) - 1] = '\0';

    strncpy(qr_str.correct_ans, c_ans, sizeof(qr_str.correct_ans) - 1);
    qr_str.correct_ans[sizeof(qr_str.correct_ans) - 1] = '\0';

    strncpy(qr_str.wrong_ans, w_ans, sizeof(qr_str.wrong_ans) - 1);
    qr_str.wrong_ans[sizeof(qr_str.wrong_ans) - 1] = '\0';

    strncpy(qr_str.ignored, ignored, sizeof(qr_str.ignored) - 1);
    qr_str.ignored[sizeof(qr_str.ignored) - 1] = '\0';

    strncpy(qr_str.accuracy, acc, sizeof(qr_str.accuracy) - 1);
    qr_str.accuracy[sizeof(qr_str.accuracy) - 1] = '\0';

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
        print_file_err();
        return;
    }

    QuizResultStr progress;
    int record_count = 0;

    printf("\n%-24s %-12s %-16s %-14s %-12s %-6s %-6s %-6s %-8s %-8s\n",
           "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Pass", "Fail", "Ignore", "Accuracy");
    printf("----------------------------------------------------------------------------------------------------------------\n");

    while (fread(&progress, sizeof(QuizResultStr), 1, encr_progress_file) != 0)
    {
        decrypt_affine((unsigned char *)progress.name, 32, 5, 2);
        decrypt_affine((unsigned char *)progress.roll, 16, 5, 2);
        decrypt_affine((unsigned char *)progress.subject, 32, 5, 2);
        decrypt_affine((unsigned char *)progress.quiz_code, 16, 5, 2);
        decrypt_affine((unsigned char *)progress.quiz_date, 32, 5, 2);
        decrypt_affine((unsigned char *)progress.total_marks, 8, 5, 2);
        decrypt_affine((unsigned char *)progress.correct_ans, 8, 5, 2);
        decrypt_affine((unsigned char *)progress.wrong_ans, 8, 5, 2);
        decrypt_affine((unsigned char *)progress.ignored, 8, 5, 2);
        decrypt_affine((unsigned char *)progress.accuracy, 8, 5, 2);

        printf("%-24s %-12s %-16s %-14s %-12s %-6s %-6s %-6s %-8s %-8s%%\n",
               progress.name,
               progress.roll,
               progress.subject,
               progress.quiz_code,
               progress.quiz_date,
               progress.total_marks,
               progress.correct_ans,
               progress.wrong_ans,
               progress.ignored,
               progress.accuracy);
        record_count++;
    }

    fclose(encr_progress_file);

    int record_left = 256 - record_count;
    if (record_count < 256)
    {
        printf("\nRecords remaining before archival reset: %d\n", record_left);
    }
    else
    {
        printf("\033[31mRecord limit maxed out (256). Archiving old records...\033[0m\n");
        migrate_oldRecord_toNew();
        FILE *reset_record = fopen("progress.dat", "wb");
        if (reset_record != NULL)
        {
            fclose(reset_record);
        }
    }
}

void migrate_oldRecord_toNew()
{
    char date[12];
    get_date_str(date, sizeof(date));

    char new_file_filepath[32] = {0};
    snprintf(new_file_filepath, sizeof(new_file_filepath), "progress_%s.dat", date);

    FILE *old_file = fopen("progress.dat", "rb");
    if (old_file == NULL)
    {
        return;
    }

    FILE *new_file = fopen(new_file_filepath, "wb");
    if (new_file == NULL)
    {
        fclose(old_file);
        return;
    }

    QuizResultStr buffer;
    while (fread(&buffer, sizeof(QuizResultStr), 1, old_file) != 0)
    {
        fwrite(&buffer, sizeof(QuizResultStr), 1, new_file);
    }

    fclose(old_file);
    fclose(new_file);
    printf("Archived old records to %s\n", new_file_filepath);
}

void show_all_progress_from_file(char *encr_progress_filepath)
{
    FILE *progress = fopen(encr_progress_filepath, "rb");
    if (progress == NULL)
    {
        print_file_err();
        return;
    }

    QuizResultStr qr_str;
    int i = 0;

    printf("\n%-24s %-12s %-16s %-14s %-12s %-6s %-6s %-6s %-8s %-8s\n",
           "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Pass", "Fail", "Ignore", "Accuracy");
    printf("----------------------------------------------------------------------------------------------------------------\n");

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

        printf("%-24s %-12s %-16s %-14s %-12s %-6s %-6s %-6s %-8s %-8s%%\n",
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

    fclose(progress);
    printf("----------------------------------------------------------------------------------------------------------------\n");
    printf("Total records: %d\n\n", i);
}

void show_student_progress_from_file(char *encr_progress_filepath, char *student_roll)
{
    FILE *progress = fopen(encr_progress_filepath, "rb");
    if (progress == NULL)
    {
        print_file_err();
        return;
    }

    QuizResultStr qr_str;
    int i = 0;

    printf("\n%-24s %-12s %-16s %-14s %-12s %-6s %-6s %-6s %-8s %-8s\n",
           "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Pass", "Fail", "Ignore", "Accuracy");
    printf("----------------------------------------------------------------------------------------------------------------\n");

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

        // Check if student roll matches (or if roll is empty, show all)
        if (student_roll == NULL || strlen(student_roll) == 0 || strcmp(qr_str.roll, student_roll) == 0)
        {
            printf("%-24s %-12s %-16s %-14s %-12s %-6s %-6s %-6s %-8s %-8s%%\n",
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
    }

    fclose(progress);
    printf("----------------------------------------------------------------------------------------------------------------\n");
    if (i == 0)
    {
        printf("\033[33mNo quiz attempts found for student roll '%s'.\033[0m\n\n", student_roll);
    }
    else
    {
        printf("Found %d quiz attempt(s) for roll '%s'.\n\n", i, student_roll);
    }
}

int decrypt_and_save_progress(char *encr_progress_filepath)
{
    FILE *encr_progress = fopen(encr_progress_filepath, "rb");
    if (encr_progress == NULL)
    {
        print_file_err();
        return 1;
    }

    char decr_progress_filepath[64] = "progress_";
    char date[16];
    get_date_str(date, sizeof(date));
    strcat(decr_progress_filepath, date);
    strcat(decr_progress_filepath, ".txt");

    FILE *decr_progress = fopen(decr_progress_filepath, "w");
    if (decr_progress == NULL)
    {
        print_file_err();
        fclose(encr_progress);
        return 1;
    }

    QuizResultStr qr_str_buf;

    fprintf(decr_progress, "%-32s%-16s%-16s%-32s%-32s%-8s%-8s%-8s%-8s%s\n",
            "Name", "Roll", "Subject", "Quiz Code", "Quiz Date", "Total", "Correct", "Wrong", "Ignored", "Accuracy");

    int rec_count = 0;
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
        rec_count++;
    }

    fclose(encr_progress);
    fclose(decr_progress);

    printf("\033[32mSuccessfully exported %d record(s) to '%s'\033[0m\n", rec_count, decr_progress_filepath);
    return 0;
}
