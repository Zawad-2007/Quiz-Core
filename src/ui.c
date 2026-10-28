#include"../include/ui.h"

void clear_terminal()
{/*Clears the whole terminal window*/
    printf("\033[H\033[2J\033[3J"); // esc seq.->what it does
                                    //"\033[H"->Takes cursor to home postion(top left position of visible terminal window, not always the first line's first column)
                                    //"\033[J"->Clears from cursor position to end
                                    //"\033[2J"->Pushes text in current screen backwards(scroll-back history)
                                    //"\033[3J"->Clears whole scroll-back history of running terminal tab
}

void clear_screen()
{/*Effectively removes all text from the start of the program*/
    printf("\033[H\033[J");
    fflush(stdout); // Flushes the stdout buffer (used for safety)
}

void print_divider(char ch, int length)
{
    for (int i = 0; i < length; i++)
    {
        putchar(ch);
    }
    putchar('\n');
}
void print_ascii_art(char filename[64])
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return;
    }
    int c = fgetc(fp);
    while (c != '*' && c != EOF)
    {
        putchar(c);
        c = fgetc(fp);
    }
    fclose(fp);
}

void print_banner()
{
    printf("\033[36m\033[1m");
    print_divider('=', 70);
    print_ascii_art("ascii-arts/ascii-art-quizcore.txt");
    print_divider('=', 70);
    printf("\033[33m"
           "       A CLI quizing application for both students and teachers\n"
           "\033[0m");
    printf("\n");
}

void print_main_menu()
{
    printf("  1. Login as Student\n");
    printf("  2. Login as Teacher\n");
    printf("  3. Login as Admin\n");
    printf("  4. Exit\n\n");
}

void print_student_menu()
{
    printf("1. Start a quiz\n");
    printf("2. Watch progress\n");
    printf("3. Exit to main menu\n");
}

void print_teacher_menu()
{
    printf("1. Encrypt a question\n");
    printf("2. Watch progress of students\n");
    printf("3. Decrypt progress file\n");
    printf("4. Exit to main menu\n");
}