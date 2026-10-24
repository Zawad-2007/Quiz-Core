#ifndef PROGRESS_H
#define PROGRESS_H

#include"misc.h"  //already contains all necessary libraries


void storeAndencrypt_progress_in_file(QuizResult *progress);

void decryptAndload_progress_in_mem();

void migrate_oldRecord_toNew();

void show_all_progress_from_file(char *encr_progress_filepath);

void decrypt_and_save_progress(char *encr_progress_filepath);
#endif