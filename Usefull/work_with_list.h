#ifndef _WORK_WITH_LIST_
#define _WORK_WITH_LIST_
	
	typedef struct Node {
	
	char *data; //данные
	struct Node *next; //ссылка на следующий элемент
		
	} Node;

	Node * get_last(Node *list);
	Node * get_nth(Node *list, int n);
	Node * getNode(char *elem);
	int free_list(Node *list);
	int print_list(Node *list);

#endif