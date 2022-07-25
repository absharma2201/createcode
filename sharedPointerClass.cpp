#include <iostream>
using namespace std;

// Class representing a shared pointer
template <typename T>

class Shared_ptr
{
private:
	// Reference counter
	int m_counter = 0;

	// Shared pointer
	T* m_ptr;
public:
	// Constructor
	explicit Shared_ptr(T* ptr = nullptr)
	{
		m_ptr = ptr;
		if (ptr)
		{
			m_counter++;
		}
	}

	// Copy constructor
	Shared_ptr(Shared_ptr<T>& sp)
	{
		m_ptr = sp.m_ptr;
		m_counter = sp.m_counter;
		m_counter++;
	}

	// Reference count
	unsigned int use_count()
	{
	return m_counter;
	}

	// Get the pointer
	T* get()
	{
	return m_ptr;
	}

	// Overload * operator
	T& operator*()
	{
	return *m_ptr;
	}

	// Overload -> operator
	T* operator->()
	{
	return m_ptr;
	}

	// Destructor
	~Shared_ptr()
	{
		m_counter--;
		if (m_counter == 0)
		{
			delete m_ptr;
		}
	}

	friend ostream& operator<<(ostream& os,
							Shared_ptr<T>& sp)
	{
		os << "Address pointed : " << sp.get() << endl;
		os << sp.m_counter << endl;
		return os;
	}
};

int main()
{
	// ptr1 pointing to an integer.
	Shared_ptr<int> ptr1(new int(151));
	cout << "--- Shared pointers ptr1 ---\n";
	*ptr1 = 100;
	cout << " ptr1's value now: " << *ptr1 << endl;
	cout << ptr1;

	{

		Shared_ptr<int> ptr2 = ptr1;
		cout << "--- Shared pointers ptr1, ptr2 ---\n";
		cout << ptr1;
		cout << ptr2;
	}
	
	cout << "--- Shared pointers ptr1 ---\n";
	cout << ptr1;

	return 0;
}
