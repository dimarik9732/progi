#include <iostream>
#include <cstring>
using namespace std;

class Cat{
	char* name;
	int age;
	int strong;

	public:
		Cat(const char* n, int a, int s): age(a), strong(s){
		name = new char[strlen(n)];
		strcpy(name, n);
		}

		int print_inf(){
			cout << name << " " << age 
		}
};


int main(int argc, char const *argv[])
{
	cat1 = Cat("Alex", 10, 20);
	cat1.print_inf();
	return 0;
}