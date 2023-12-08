#include <iostream>

#include <stdexec/execution.hpp>
#include "exec/static_thread_pool.hpp"

using namespace stdexec;

int main() {
    exec::static_thread_pool ctx{1};
    scheduler auto sch = ctx.get_scheduler();                                    

    sender auto work =  schedule(sch) | 
        then([]{ 
            std::cout << "Hello world! Have an int." << std::endl;                   
            return 13;    
        }) | 
        then([](int arg) { return arg + 42; });                                            

    auto [i] = sync_wait(std::move(work)).value();  
    std::cout << "i:" << i << std::endl;                           
}