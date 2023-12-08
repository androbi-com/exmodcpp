#include <iostream>
#include <coroutine>

class Task {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit Task(handle_type h) : handle_(h) {
        std::cout << "Task::Task(handle_type) " << (handle_ ? "non-null" : "null") << "\n";
    }

    bool resume() {
        if (!handle_.done()) {
            std::cout << "Task::resume()\n";
            handle_.resume();
        } else {
            std::cout << "Task::resume(): coroutine already done\n";
        }
        return !handle_.done();
    }

    ~Task() {
        if (handle_) {
            std::cout << "~Task(): destroying handle\n";
            handle_.destroy();
        }
    }

private:
    handle_type handle_;
};

struct Task::promise_type {
 Task get_return_object() noexcept {
        std::cout << "promise_type::get_return_object()\n";
        return Task{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept {
        std::cout << "promise_type::initial_suspend()\n";
        return {};
    }
    std::suspend_always final_suspend() noexcept {
        std::cout << "promise_type::final_suspend()\n";
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
    std::cout << "main(): hit return to resume() #1\n";
    std::cin.get();
    c.resume();
    std::cout << "main(): hit return to resume() #2\n";
    std::cin.get();
    c.resume();
    std::cout << "main(): exiting\n";
}