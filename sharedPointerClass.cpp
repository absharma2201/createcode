#include <iostream>
using namespace std;
typedef unsigned int uint;

template<class T>
class my_shared_ptr
{
private:
	T * ptr = nullptr;
	uint * refCount = nullptr;

public:
	my_shared_ptr() : ptr(nullptr), refCount(new uint(0)) // default constructor
	{
	}

	my_shared_ptr(T * ptr) : ptr(ptr), refCount(new uint(1)) // constructor
	{
	}

	/*** Copy Semantics ***/
	my_shared_ptr(const my_shared_ptr & obj) // copy constructor
	{
		this->ptr = obj.ptr; // share the underlying pointer
		this->refCount = obj.refCount;
		if (nullptr != obj.ptr)
		{
			(*this->refCount)++; // if the pointer is not null, increment the refCount
		}
	}

	my_shared_ptr& operator=(const my_shared_ptr & obj) // copy assignment
	{
		__cleanup__(); // cleanup any existing data
		
		// Assign incoming object's data to this object
		this->ptr = obj.ptr; // share the underlying pointer
		this->refCount = obj.refCount;
		if (nullptr != obj.ptr)
		{
			(*this->refCount)++; // if the pointer is not null, increment the refCount
		}
	}

	/*** Move Semantics ***/
	my_shared_ptr(my_shared_ptr && dyingObj) // move constructor
	{
		this->ptr = dyingObj.ptr; // share the underlying pointer
		this->refCount = dyingObj.refCount;

		dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
	}

	my_shared_ptr& operator=(my_shared_ptr && dyingObj) // move assignment
	{
		__cleanup__(); // cleanup any existing data
		
		this->ptr = dyingObj.ptr; // share the underlying pointer
		this->refCount = dyingObj.refCount;

		dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
	}

	uint get_count() const
	{
		return *refCount; // *this->refCount
	}

	T* get() const
	{
		return this->ptr;
	}

	T* operator->() const
	{
		return this->ptr;
	}

	T& operator*() const
	{
		return this->ptr;
	}

	~my_shared_ptr() // destructor
	{
		__cleanup__();
	}

private:
	void __cleanup__()
	{
		(*refCount)--;
		if (*refCount == 0)
		{
			if (nullptr != ptr)
				delete ptr;
			delete refCount;
		}
	}
};


class Box
{
public:
    int length, width, height;
    Box() : length(0), width(0), height(0)
    {
    }
};int main()
{
    my_shared_ptr<Box> obj;
    cout << obj.get_count() << endl; // prints 0    
    my_shared_ptr<Box> box1(new Box());
    cout << box1.get_count() << endl; // prints 1
    my_shared_ptr<Box> box2(box1); // calls copy constructor
    cout << box1.get_count() << endl; // prints 2
    cout << box2.get_count() << endl; // also prints 2
 
    return 0;
}
