/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <math.h>

using namespace std;

struct node {
    int data;
    struct node * next;
};

struct hList {
    struct node *head;
};

struct hTable {
  int cap;
  struct hList *hlistArr[];
};

struct node * newNode(int data) {
    struct node * newN = (struct node *)malloc(sizeof(struct node));
    newN->data = data;
    newN->next = nullptr;
    return newN;
}

struct hTable * initialise(int capacity) {
    struct hTable * htable = (struct hTable*)malloc(sizeof(struct hTable));
    htable->cap = capacity;
    
    for (auto i =0; i < htable->cap; i++) {
        htable->hlistArr[i] = (struct hList*)malloc((sizeof(struct hList)));
        htable->hlistArr[i]->head = NULL;
    }
    return htable;
}

int hashFunc(int data, int capacity){
    return data % capacity;//&(pow(2, capacity) -1);
}

void insert(struct hTable * htable, int data) {
    int index = hashFunc(data, htable->cap);
    struct node * temp = newNode(data);
    if(htable->hlistArr[index]->head == NULL) {
        htable->hlistArr[index]->head = temp;
    } else {
        temp->next = htable->hlistArr[index]->head;
        htable->hlistArr[index]->head = temp;
    }
}

void display(struct hTable * htable, int data) {
    int index = hashFunc(data, htable->cap);
    struct node * temp;
    if(htable->hlistArr[index]->head == NULL) {
        std::cout<<"Not found"<<std::endl;
    } else {
        temp = htable->hlistArr[index]->head;
        while (temp) {
            if(temp->data == data) {
                std::cout<<"Found"<<std::endl;
                break;
            }
            std::cout<<"looked: "<<temp->data<<std::endl;
            temp = temp->next;
        }
    }
}

int main()
{
    struct hTable * htable = initialise(5);
    insert(htable, 0);
    insert(htable, 5);
    insert(htable, 10);
    insert(htable, 1);
    display(htable, 0);
    display(htable, 1);

    return 0;
}
