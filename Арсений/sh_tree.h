/* sh_tree.h */
#ifndef __SH_TREE_H__
#define __SH_TREE_H__
	
#include "sh_types.h"
#include "sh_utils.h"

extern int err;
extern list curlist;

void printTree(tree t);

void clearTree(tree t);

void getnext();

tree buildT3();
tree buildT2();
tree buildT1();
tree buildT0();

#endif