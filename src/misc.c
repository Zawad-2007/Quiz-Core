#include "../include/misc.h"
#include<string.h>

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

void store_progress_in_file(QuizResult *progress)
{
    FILE *file_exists = fopen("progress.txt", "rb");

    if (!(file_exists))
    {
        FILE *fp = fopen("progress.txt", "wb");
        if (fp != NULL)
        {
            fprintf(fp, "%-32s%-16s%-32s%-8s%-8s%-8s%-12s\n",
                    "Name", "Roll", "Subject", "Total", "Correct", "Wrong", "Accuracy");
            fclose(fp);
        }
    }
    else
    {
        fclose(file_exists);
    }

    FILE *progress_file = fopen("progress.txt", "ab");
    if (progress_file == NULL)
    {
        printf("\033[31mFile Error\033[0m\n");
        return;
    }

    fprintf(progress_file, "%-32s%-16s%-32s%-8d%-8d%-8d%.2f%%\n",
            progress->name,
            progress->roll,
            progress->subject,
            progress->total_marks,
            progress->correct_ans,
            progress->wrong_ans,
            progress->accuracy);

    fclose(progress_file);
}
