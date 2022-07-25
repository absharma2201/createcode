/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <pthread.h>

using namespace std;

class Logger {
    static Logger * instance;
    static int count;
    static pthread_mutex_t lock;
    Logger () {
        count++;
        cout<<"count"<<count<<endl;
    }
    public :
    static Logger* getLogger();
    
    void logLine(const string& msg){
        cout<<"Log: "<<msg<<endl;
    }
};
Logger* Logger::instance = nullptr;
int Logger::count = 0;
pthread_mutex_t Logger::lock;

Logger* Logger::getLogger() {
    if(instance ==  nullptr) {
        pthread_mutex_lock(&lock);
        if(instance == nullptr) {
                instance = new Logger();
        }
        pthread_mutex_unlock(&lock);
    }
   
    return instance;
}

void * user1Log(void * args) {
    Logger* a = Logger::getLogger();
    a->logLine("Abhinav");
    pthread_exit(0);
}

void * user2Log(void * args) {
    Logger* b = Logger::getLogger();
    b->logLine("Megha");
    pthread_exit(0);
}

int main()
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, &user1Log, NULL);
    pthread_create(&thread2, NULL, &user2Log, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
