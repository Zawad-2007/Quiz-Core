#ifndef UI_H
#define UI_H

#include<stdio.h>

#ifndef ERR_STR
#define ERR_STR "\033[31mFILE error\033[0m\n"
#endif

void clear_terminal();

void clear_screen();

void print_divider(char ch, int length);

void print_ascii_art(char filename[64]);

void print_banner();

void print_main_menu();

void print_student_menu();

void print_teacher_menu();

#endif