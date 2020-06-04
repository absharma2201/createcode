#include<iostream>
using namespace std;

class A {
	public:
		static int x;
		A(){
		
		}
		~A(){
		
		}
		void * operator new(size_t sz){
			++x;
		}

		void operator delete(void *k){
			--x;
		}

};

int A::x = 0;

int main(){

	A a,b;

	cout<<A::x<<endl;
	A *ptr = new A;

	cout<<A::x<<endl;

	delete ptr;
	cout<<A::x<<endl;
}
