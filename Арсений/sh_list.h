/* sh_list.h */
#ifndef __SH_LIST_H__
#define __SH_LIST_H__

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include "sh_types.h"

int listSize(list l);

void printList(list l);

void clearList(list l);

list buildList(char *s);

#endif