#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// hash function
short unsigned int HashRs(char *str){
	const unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;

	for(; *str; str++)
	{
		hash = hash * a + (unsigned char)(*str);
		a *= b;
	}

	return (unsigned short)hash;

}

/*==================================================================
					WORK WITH LIST 									
==================================================================*/

// add
Node * addelem(Node *list, char *elem){

	Node * temp = (Node *) malloc (sizeof(Node));
	temp->line = elem;
	temp->count = 1;
	temp->next = list;
	temp->prev = NULL;
	if (list){
		list->prev = temp;
	}
	return temp;

}

// find
Node* find_in_list(Node* list, char* key){
	if (list == NULL || strcmp(list->line, key) == 0) return list;
	else return find_in_list(list->next, key);
}

// print
int print_list(Node* list){
	if (list == NULL) return 0;
	else {
		printf("%7d %s\n", list->count, list->line);
		print_list(list->next);
		return 0;
	} 
}

// remove
Node* rm_from_list(Node *list, char * key){

	Node * elem = find_in_list(list, key);
	
	if (!elem) return list;
	else {
		if (!elem->prev){
			Node *temp = elem->next;
			free(elem);
			if (temp) temp->prev = NULL;
			return temp;
		} else if(!elem->next){ 
					elem->prev->next = elem->next;
					free(elem);
					return list;
				}
				else {
						elem->prev->next = elem->next;
						elem->next->prev = elem->prev;
						free(elem);
						return list;
				}
	}
}

/*==================================================================
					WORK WITH HASHTABLE 							
==================================================================*/

// add
int add_to_hash(HASH hash_table, char *key){

	unsigned short i = HashRs(key);
	Node *f = find_in_list(hash_table[i],key);
	if (!f){
		hash_table[i] = addelem(hash_table[i],key);
	}
	else f->count += 1;
	
	return 0;

}

// find
Node * find_in_hash(HASH hash_table, char* key){
		
		unsigned short i = HashRs(key);
		Node *f = find_in_list(hash_table[i],key);
		return f; 
}

// print
int print_table (HASH hash_table){
	
	unsigned int i;

	for (i = 0; i <= 65535; i++){
		if (hash_table[i]) print_list(hash_table[i]);
	}

	return 0;
}

// remove
int rm_from_hash(HASH hash_table, char *key){

	unsigned int i = HashRs(key);
	hash_table[i] = rm_from_list(hash_table[i], key);
	return 0;
}