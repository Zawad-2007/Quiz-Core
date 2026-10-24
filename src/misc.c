#include "../include/misc.h"


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

void get_date_str(char *d_str, int size)
{
    time_t cur_time = time(NULL);

    struct tm *l_time_struct_ptr = localtime(&cur_time);

    int year = l_time_struct_ptr->tm_year + 1900; // tm_year counts from 1900
    int month = l_time_struct_ptr->tm_mon + 1;    // tm_mon's month index starts from 0
    int day = l_time_struct_ptr->tm_mday;         // tm_mday's day index starts from 1, so no additional increment

    snprintf(d_str, size, "%02d-%02d-%4d", day, month, year);
}