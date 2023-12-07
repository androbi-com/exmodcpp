#include <iostream>

#include <stdexec/execution.hpp>
#include "exec/static_thread_pool.hpp"

using namespace stdexec;
//using stdexec::sync_wait;


int main() {
    exec::static_thread_pool ctx{1};
    scheduler auto sch = ctx.get_scheduler();                                     // 1

    sender auto begin = schedule(sch);                                            // 2
    sender auto hi = then(begin, []{                                              // 3
        std::cout << "Hello world! Have an int." << std::endl;                    // 3
        return 13;                                                                // 3
    });                                                                           // 3
    sender auto add_42 = then(hi, [](int arg) { return arg + 42; });              // 4

    auto [i] = sync_wait(add_42).value();  
    std::cout << "i:" << i << std::endl;                           
}