#include "../include/misc.h"
#include <errno.h>

void print_file_err(void)
{
    printf(ERR_STR);
    if (errno != 0)
    {
        printf("\033[31m%s\033[0m\n", strerror(errno));
    }
}


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

void rm_extra_spaces(char *str)
{
    int len = strlen(str);
    int n = len;
    while (len > 0 && isspace((unsigned char)str[len - 1]))
    {
        str[len - 1] = '\0';
        len--;
    }
    int space_count = 0;//counts number of spaces from beginning of the string
    for (int i = 0; i < n; i++)
    {
        if(isspace(str[i]))
        {
            space_count++;
        }
        else
        {
            break;
        } 
    }

    for (int i = 0; i < n-space_count; i++)
    {
        str[i] = str[i + space_count];
    }
    for (int i = n - space_count; i < n; i++)
    {
        str[i] = '\0';
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