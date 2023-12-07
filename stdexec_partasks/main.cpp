#include <stdexec/execution.hpp>
#include "exec/static_thread_pool.hpp"
#include <iostream>


using namespace stdexec;

int compute_task(int i) {
    std::cout << "compute_task(" << i << ")" << std::endl;
    return i*i;
}

int main() {
    exec::static_thread_pool ctx{1};
    scheduler auto sch = ctx.get_scheduler();                                     // 1

    sender auto work = when_all(
        schedule(sch) | then([]{ return compute_task(1); }),
        schedule(sch) | then([]{ return compute_task(2); })
    );

    auto [a,b] = sync_wait(std::move(work)).value();  
    std::cout << "a:" << a << " b: " << b << std::endl;                           
}