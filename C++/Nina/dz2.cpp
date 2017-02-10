#include <iostream>
using namespace std;

struct Node {
	int elem;
	Node *prev, *next;
};

class list {
	Node *head, *tail;
	int lenght;

	public:
		// Конструктор
		list(): head(NULL), tail(NULL), lenght(0) {}

		// Деструктор
		~list() {
			while (head != NULL) {
				cout << "Работает деструктор" << endl;
				Node *temp = head->next;
				delete head; 
				head = temp;		
			}
		}

		// Доб-е эл-та в начало
		int push_front(int x) {
			lenght++;

			Node *temp = new Node;
			temp->prev = NULL;
			temp->elem = x;
			temp->next = head;
			
			if (head != NULL)
				head->prev = temp;
			else
				tail = temp;

			head = temp;
			return 0;
		}		

		// Доб-е эл-та в конец
		int push_back(int x) {
			lenght++;

			Node *temp = new Node;
			temp->prev = tail;
			temp->elem = x;
			temp->next = NULL;
			
			if (tail != NULL)
				tail->next = temp;
			else
				head = temp;
			
			tail = temp;
			return 0;
		}

		// Чтение первого эл-та списка
		int front() const {
			cout << head->elem << endl;
		}

		// Чтение посл-го эл-та списка
		int back() const {
			cout << tail->elem << endl;
		}

		// Удаление первого эл-та списка
		int pop_front() {
			if (lenght == 0)
				return 0;
			
			lenght--;

			Node *temp = head;
			head = head->next;
			
			if (lenght == 0)
				tail = NULL;
			else
				head->prev = NULL;
			
			delete temp;
		}

		// Удаление посл-го эл-та списка
		int pop_back() {
			if (lenght == 0)
				return 0;

			lenght--;

			Node *temp = tail;
			tail = tail->prev;

			if (lenght == 0)
				head = NULL;
			else
				tail->next = NULL;

			delete temp;
		
		}

		// Доб-е эл-та х перед позицией р
		int insert(int p, int x) {
			if ((lenght < p) || (p == 0)) {
				cout << "В списке нет " << p << "-го элемента" << endl;
				return 0;
			}		
			
			if (p == 1) {
				push_front(x);
				return 0;
			}
		
			int count = 1;
			Node *temp = head;
		       	while (count != p) {
				temp = temp->next;
				count++;
			}
		
			Node *newelem = new Node;
			newelem->elem = x;
			temp->prev->next = newelem;
			newelem->prev = temp->prev;
			newelem->next = temp;
			temp->prev = newelem;

			lenght++;
		
			return 0;
		}

		// Удаление эл-та из позиции р
		int erase(int p) {
			if ((lenght < p) || (p == 0)) {
				cout << "В списке нет " << p << "-го элемента" << endl;
				return 0;
			}		
		
			if (p == 1) {
				pop_front();
				return 0;
			}
			if (p == lenght) {
				pop_back();
				return 0;
			}
	
			int count = 1;
			Node *temp = head;
		       	while (count != p) {
				temp = temp->next;
				count++;
			}
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;

			lenght--;

			return 0;
		}
		
		// Проверка списка на пустоту
		int empty() const {
			if (lenght == 0) 
				cout << "Пустой" << endl;
			else
				cout << "Непустой" << endl;
			return 0;
		}

		// Текущее число эл-в
		int size() const {
			cout << lenght << endl;
		}


		// Ф-я печати списка
		int print() const {
			if (head == NULL) {
				cout << "Пустой" << endl;
				return 0;
			}

			Node *temp = head;
			while (temp != NULL) {
				cout << temp->elem << " ";
				temp = temp->next;
			}
			cout << endl;
			return 0;
		}

};

int main() {
	list list1;
	
	list1.push_front(1);
	list1.push_back(2);
	list1.insert(2, 3);

	list1.print();
	list1.size();

	list1.erase(2);
	list1.pop_front();
	list1.pop_back();

	list1.empty();
//	list1.insert(1, 10);
//	list1.pop_front();
//	list1.pop_back();
//	list1.print();
//	list1.empty();
//	list1.size();
//	list1.push_front(5);
//	list1.erase(1);
//	list1.insert(1, 10);
//	list1.pop_back();
//	list1.push_front(3);
//	list1.pop_front();
//	list1.insert(3, 11);
//	list1.push_front(9);
//	list1.push_back(4);
//	list1.erase(2);
//	list1.erase(2);
//	list1.pop_back();
//	list1.print();
//	list1.front();
//	list1.back();
//	list1.print();


	return 0;
}
