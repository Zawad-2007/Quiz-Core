#include "../include/misc.h"
#include<string.h>
#include "../include/affine_cipher.h"

void rm_newl(char *string, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        if (string[i] == '\n')
        {
            string[i] = '\0';
            return;
        }
    }
}

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
    char t_marks[8], c_ans[8], w_ans[8], acc[8];
    snprintf(t_marks, 8, "%d", progress->total_marks);
    snprintf(c_ans, 8, "%d", progress->correct_ans);
    snprintf(w_ans, 8, "%d", progress->wrong_ans);
    snprintf(acc, 8, "%.2f", progress->accuracy);

    strcpy(qr_str.name, progress->name);
    strcpy(qr_str.roll, progress->roll);
    strcpy(qr_str.subject, progress->subject);
    strcpy(qr_str.total_marks, t_marks);
    strcpy(qr_str.correct_ans, c_ans);
    strcpy(qr_str.wrong_ans, w_ans);
    strcpy(qr_str.accuracy, acc);

    encrypt_affine((unsigned char *)qr_str.name, 32, 5, 2);
    encrypt_affine((unsigned char *)qr_str.roll, 16, 5, 2);
    encrypt_affine((unsigned char *)qr_str.subject, 32, 5, 2);
    encrypt_affine((unsigned char *)qr_str.total_marks, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.correct_ans, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.wrong_ans, 8, 5, 2);
    encrypt_affine((unsigned char *)qr_str.accuracy, 8, 5, 2);

    fwrite(&qr_str, sizeof(QuizResultStr), 1, progress_file);

    fclose(progress_file);
}

void decryptAndload_progress_in_mem()
{
    FILE *encr_progress_file = fopen("progress.dat", "rb");
    if(encr_progress_file==NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return;
    }

    QuizResultStr progress[256];

    int r_count = 0;
    int i = 0;
    while (fread(progress + i, sizeof(QuizResultStr), 1, encr_progress_file) != 0)
    {
        decrypt_affine((unsigned char *)progress[i].name, 32, 5, 2);
        decrypt_affine((unsigned char *)progress[i].roll, 16, 5, 2);
        decrypt_affine((unsigned char *)progress[i].subject, 32, 5, 2);
        decrypt_affine((unsigned char *)progress[i].total_marks, 8, 5, 2);
        decrypt_affine((unsigned char *)progress[i].correct_ans, 8, 5, 2);
        decrypt_affine((unsigned char *)progress[i].wrong_ans, 8, 5, 2);
        decrypt_affine((unsigned char *)progress[i]. accuracy, 8, 5, 2);
        i++;
    }

    r_count = i;  //record count
    int record_left = 256 - r_count;
    
    if(r_count<256)
    {
        printf("Records will be reset after %d quiz\n", record_left);
    }
    else if (r_count == 256)
    {
        printf("\033[31mRecord limit maxed out\033[0m\n\033[32mThe progress record will be reset\033[0m\n");
    }

        printf("%-32s%-16s%-32s%-8s%-8s%-8s%s%%\n", "Name", "Roll", "Subject", "Total", "Correct", "Wrong", "Accuracy");
        printf("%-32s%-16s%-32s%-8s%-8s%-8s%s%%\n",
               progress[r_count - 1].name,
               progress[r_count - 1].roll,
               progress[r_count - 1].subject,
               progress[r_count - 1].total_marks,
               progress[r_count - 1].correct_ans,
               progress[r_count - 1].wrong_ans,
               progress[r_count - 1].accuracy);
        if (r_count >= 256)
        {//resets the record in the end if the maximum limit crosses
            FILE *reset_record = fopen("progress.dat", "wb");
            fclose(reset_record);
        }
}
