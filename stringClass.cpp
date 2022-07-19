#define __STDC_WANT_LIB_EXT1__ 1
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

class my_string
{
private:
    char * buf = nullptr;
    unsigned int size = 0;

public:
    my_string() : buf(nullptr), size(0) // default constructor
    {
    }

    my_string(const char * const buffer) // constructor
    {
        size = strlen(buffer);
        buf = new char[size + 1]; // + 1 for the keeping the null character
        strncpy(buf, buffer, size); // copy from the incoming buffer to character buffer of the new object
    }

    my_string(const my_string & obj) // copy constructor
    {
        size = obj.size;
        buf = new char[size + 1]; // + 1 for the keeping the null character
        strncpy(buf, obj.buf, size); // copy from the incoming buffer to character buffer of the new object
    }

    my_string& operator=(const my_string & obj) // copy assignment
    {
        __cleanup__(); // cleanup any existing data

        // Copy data from the newly assigned my_string object
        size = obj.size;
        buf = new char[size + 1]; // + 1 for the keeping the null character
        strncpy(buf, obj.buf, size); // copy from the incoming buffer to character buffer of the new object
        return *this;
    }

    my_string(my_string && dyingObj) // move constructor
    // && is a reference operator defined in the C++11 standard 
    // which means "dyingObj" is an r-value reference.
    // Compiler calls this constructor when the object passed in the argument
    // is about to die due to scope end or such
    {
        __cleanup__(); // cleanup any existing data

        // Copy data from the incoming object
        size = dyingObj.size;
        
        // Transfer ownership of underlying char buffer from incoming object to this object
        buf = dyingObj.buf;
        dyingObj.buf = nullptr;        
    }

    my_string& operator=(my_string && dyingObj) // move assignment
    {
        __cleanup__(); // cleanup any existing data

        // Copy data from the incoming object
        size = dyingObj.size;

        // Transfer ownership of underlying char buffer from incoming object to this object
        buf = dyingObj.buf;
        dyingObj.buf = nullptr;

        return *this;
    }

    my_string operator+(const my_string & obj) // concatenation
    {
        my_string s; // create a new string named 's'
        s.size = this->size + obj.size;
        s.buf = new char[s.size + 1]; // allocate memory to keep the concatenated string
        strncpy(s.buf, this->buf, this->size); // copy the 1st string
        strncpy(s.buf + this->size, obj.buf, obj.size);

        return s;
    }

    unsigned int length()
    {
        return size;
    }

    const char * c_str() const
    {
        return buf;
    }

    ~my_string() // destructor
    {
        __cleanup__();
    }

private:
    void __cleanup__()
    {
        if (buf != nullptr)
        {
            delete[] buf;
        }
        size = 0;
    }
};

std::ostream& operator<<(std::ostream& cout, const my_string & obj)
{
    cout << obj.c_str();
    return cout;
}

int main()
{
    my_string a("FirstName");
    my_string b("LastName");
    my_string c = a + b;
    std::cout << c;
    return 0;
}

