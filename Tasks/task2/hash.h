#ifndef _HASH_H_
#define _HASH_H_

typedef struct Node {
	char *line;
	int count;
	struct Node *next;
	struct Node *prev; 
} Node;

typedef Node ** HASH;


// hash function
short unsigned int HashRs(char *str);

/*==================================================================
					WORK WITH LIST 									
==================================================================*/

// add
Node * addelem(Node *list, char *elem);

// find
Node* find_in_list(Node* list, char* key);

// print
int print_list(Node* list);

// remove
Node* rm_from_list(Node *list, char * key);

/*==================================================================
					WORK WITH HASHTABLE 							
==================================================================*/

// add
int add_to_hash(HASH hash_table, char *key);

// find
Node * find_in_hash(HASH hash_table, char* key);

// print
int print_table (HASH hash_table);

int rm_from_hash(HASH hash_table, char *key);

#endif