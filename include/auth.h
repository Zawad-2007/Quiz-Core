#ifndef AUTH_H
#define AUTH_H

#include <strings.h>
#include "stud_data.h"
#include "teacher_data.h"
#include "ui.h"
#include "misc.h"

int student_login(StudentLoginDta *student_out);

int teacher_login(TeacherLoginDta *teacher_out);

int admin_login(void);

void handle_admin_menu(void);

#endif
