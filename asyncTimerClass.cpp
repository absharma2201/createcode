/******************************************************************************
Asynchoronous Timer Class
*******************************************************************************/

#include <iostream>
#include <chrono>
#include <functional>
#include <thread>


/**
 *  Create asynchronous timers which execute specified
 *  functions in set time interval.
 *  
 *  @param func         Function which sould be executed
 *  @param interval     Interval of time in which function will be executed
 *                                      (in milliseconds)
 */
class Timer {
private:
        // Function to be executed fater interval
        std::function<void(void)> m_func;
        // Timer interval in milliseconds
        long m_interval;
        long m_timeout;

        // Thread timer is running into
        std::thread m_thread;
        // Status if timer is running
        bool m_running = false;
public:
        Timer() {}

        Timer(std::function<void(void)> func, const long &interval) {
                m_func = func;
                m_interval = interval;
        }

        /**
         * Starting the timer.
         */
        void start() {
                m_running = true;
                m_thread = std::thread([&]() {
                        while (m_running) {
                                auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_interval);
                                m_func();
                                std::this_thread::sleep_until(delta);
                        }
                });
                m_thread.detach();
        }
        
        void start2() {
                m_running = true;
                m_thread = std::thread([&]() {
                        //while (m_running) {
                                auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_timeout);
                                m_func();
                                std::this_thread::sleep_until(delta);
                                std::cout << "Stoping Timer 4" << std::endl;
                                stop2();
                        //}
                });
                m_thread.detach();
        }

        /*
         *  Stopping the timer and destroys the thread.
         */
        void stop() {
                m_running = false;
                m_thread.~thread();
                std::cout << "Stoping Timer :"<<m_interval << std::endl;
        }
        void stop2() {
                m_running = false;
                m_thread.~thread();
                std::cout << "Stoping Timer :"<<m_timeout << std::endl;
        }
        /*
         *  Restarts the timer. Needed if you set a new
         *  timer interval for example.
         */

        void restart() {
                stop();
                start();
        }
        
        /*
         *  Check if timer is running.
         *  
         *  @returns boolean is running
         */
        bool isRunning() {
                return m_running;
        }


        /*
        *  Set the method of the timer after
        *  initializing the timer instance.
        *
        *  @returns boolean is running
        *  @return  Timer reference of this
        */
        Timer *setFunc(std::function<void(void)> func) {
                m_func = func;
                return this;
        }

        /*
         *  Returns the current set interval in milliseconds.
         *
         *  @returns long interval
         */
        long getInterval() {
                return m_interval;
        }

        /*
        *  Set a new interval for the timer in milliseconds.
        *  This change will be valid only after restarting
        *  the timer.
        *
        *  @param interval new interval
        *  @return  Timer reference of this
        */
        Timer *setInterval(const long &interval) {
                m_interval = interval;
                return this;
        }
        Timer *setTimeout(const long &timeout) {
                m_timeout = timeout;
                return this;
        }

        ~Timer() {
                stop();
        }


};

using namespace std;


int main() {


        /*
                EXAMPLE 1
                Initialize a timer which will count up
                the integer every second and output the
                result.
        */


        /*
                EXAMPLE 2
                Count up an integer and another one down
                every second. If they are at the same value,
                stop the timer from inside.
                This is only possible, when you pass over the
                function with the timer instance reference into
                after initializing the timer instance.
        */

        int iGoUp = 0, iGoDown =20;
        Timer t2;

        t2.setFunc([&](){
                if (++iGoUp == --iGoDown)
                        t2.stop();
                cout << iGoUp << " - " << iGoDown << endl;
        })
                ->setInterval(1000)
                ->start();
                
        Timer t3;

        t3.setFunc([&](){
                if (++iGoUp == --iGoDown)
                        t3.stop();
                cout << iGoUp << " - " << iGoDown << endl;
        })
                ->setInterval(2000)
                ->start();
        Timer t4;

        t4.setFunc([&](){
                cout << "Starting Timer 4" << endl;
        })
                ->setTimeout(5000)
                ->start2();

        
        cin.clear();
        cin.get();
        return 0;
}
