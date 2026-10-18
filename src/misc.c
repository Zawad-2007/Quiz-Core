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