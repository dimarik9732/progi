/* sh_exec.h */
#ifndef __SH_EXEC_H__
#define __SH_EXEC_H__

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include "sh_types.h"

extern int fake;

extern struct ztable ztable;

void addzt(pid_t pid);

void clrzt();

void printzt();

int e_exit();

int e_cd(int argc, char **argv);

int e_pwd(int fd);

int exeT(tree t);
#endif