#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
using namespace std::chrono_literals;

// example from https://en.cppreference.com/w/cpp/thread/condition_variable
 
//NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables) 
std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;
//NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables) 
 
void worker_thread()
{   
    std::cout << "worker_thread() is starting, now sleep 1.5s\n";
    std::this_thread::sleep_for(500ms); // main() locks m at 1000ms
 
    // Wait until main() sends data
    std::cout << "worker_thread() tries to lock\n";
    std::unique_lock lk(m);
    std::cout << "worker_thread() has aquired lock\n";
    // Constructs a unique_lock with m as the associated mutex. Locks the associated mutex by calling m.lock().
    if (!ready) {
        std::cout << "worker_thread() is waiting...\n";
        cv.wait(lk, []{ return ready; });
        // Atomically unlocks lock, blocks the current executing thread, and adds it to 
        // the list of threads waiting on *this. The thread will be unblocked when 
        // notify_all() or notify_one() is executed. It may also be unblocked spuriously. 
        // When unblocked, regardless of the reason, lock is reacquired and wait exits.
    }    
    // after the wait, we own the lock.
    std::cout << "worker_thread() is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "worker_thread() signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one(); 
}
 
int main()
{
    std::thread worker(worker_thread);
    std::cout << "main() is starting, now sleep 1s\n";
    std::this_thread::sleep_for(1000ms);
    std::cout << "main() is preparing data\n";
    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard lk(m); 
        std::this_thread::sleep_for(1000ms);
        // RAII: When a lock_guard object is created, it attempts to take ownership of the mutex 
        // it is given. When control leaves the scope in which the lock_guard object was created, 
        // the lock_guard is destructed and the mutex is released.
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();
 
    // wait for the worker
    {
        std::unique_lock lk(m);
        cv.wait(lk, []{ return processed; });
    }
    std::cout << "Back in main(), data = " << data << '\n';
 
    worker.join();
}