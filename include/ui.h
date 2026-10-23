#ifndef UI_H
#define UI_H

#include<stdio.h>

#define ERR_STR "\033[31mFILE error\n"

void clear_terminal();

void clear_screen();

void print_divider(char ch, int length);

void print_ascii_art(char filename[20]);

void print_banner();

void print_main_menu();

void print_student_menu();

void print_teacher_menu();

#endif