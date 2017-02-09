/* sh_utils.h */
#ifndef __SH_UTILS_H__
#define __SH_UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include "sh_types.h"

int find(char c, char *s);

int cmp(char *s1, char *s2);

int leng(char *s);

char *strcopy(char *s);

char *readStr();

#endif