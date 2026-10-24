#ifndef PROGRESS_H
#define PROGRESS_H

#include <string.h>
#include "affine_cipher.h"
#include"misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void storeAndencrypt_progress_in_file(QuizResult *progress);

void decryptAndload_progress_in_mem();

void migrate_oldRecord_toNew();

#endif