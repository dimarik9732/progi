#include <iostream>
#include <cstring>
using namespace std;

struct Node
{
	int data;
	Node *next;
	Node *prev;  
};

class LinkedList{
	Node *first, *last;
	int length;

	public:

		LinkedList(){
			first = NULL;
			last = NULL;
			length = 0;
		}

		LinkedList(int a){
			first = new Node;
			first->next = NULL;
			first->prev = NULL;
			first->data = a;
			last = first;
			length = 1;
		}

		LinkedList(LinkedList & b){
			if (b.first)
			{
				this->first = new Node;
				this->first->data = (b.first)->data;
				this->first->prev = NULL;	
			}
			Node *previous = this->first;
			Node *current = this->first;
			Node *b_current = b->first->next;
			while (b_current->next)
			{
				current->next = new Node;
				current = current->next;
				current->data = b_current->data;
				current->prev = previous;
				previous = previous->next;
				b_current = b_current->next;
			}
			current->next = NULL;
		}

		


};


int main(int argc, char const *argv[])
{
	Cat cat1("Alex", 10, 20);
	cat1.print_inf();
	return 0;
}