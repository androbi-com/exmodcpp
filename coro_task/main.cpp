#include <iostream>
#include <coroutine>

// #define DEBUG_
class Task {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit Task(handle_type h) : handle_(h) {
#ifdef DEBUG_
        std::cout << "Task::Task(handle_type) " << (handle_ ? "non-null" : "null") << "\n";
#endif
    }

    bool resumeTask() {
        if (!handle_.done()) {
#ifdef DEBUG_
            std::cout << "Task::resumeTask()\n";
#endif
            // this calls the resume() method on the coroutine handle
            handle_.resume();
        } else {
#ifdef DEBUG_
            std::cout << "Task::resumeTask(): coroutine already done\n";
#endif
        }
        return !handle_.done();
    }

    ~Task() {
        if (handle_) {
#ifdef DEBUG_
            std::cout << "~Task(): destroying handle\n";
#endif
            handle_.destroy();
        }
    }

private:
    handle_type handle_;
};

struct Task::promise_type {
 Task get_return_object() noexcept {
#ifdef DEBUG_
        std::cout << "promise_type::get_return_object()\n";
#endif
        return Task{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept {
#ifdef DEBUG_
        std::cout << "promise_type::initial_suspend()\n";
#endif
        return {};
    }
    std::suspend_always final_suspend() noexcept {
#ifdef DEBUG_
        std::cout << "promise_type::final_suspend()\n";
#endif
        return {};
    }
    void return_void() noexcept {}
    void unhandled_exception() noexcept {
        std::terminate();
    }
};

Task foo() {
    std::cout << "foo(): started coroutine\n";
    co_await std::suspend_always{};
    std::cout << "foo(): resumed coroutine\n";
    co_return;
}

int main() {
    std::cout << "main(): starting\n";
    Task c = foo();
    // c is now in a suspended state
    std::cout << "main(): returned from foo()\n";
    std::cout << "main(): hit return to resumeTask() #1\n";
    std::cin.get();
    c.resumeTask();
    std::cout << "main(): hit return to resumeTask() #2\n";
    std::cin.get();
    c.resumeTask();
    std::cout << "main(): exiting\n";
}