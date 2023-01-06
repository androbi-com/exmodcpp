#include <iostream>
#include <coroutine>

class Task {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit Task(handle_type h) : handle_(h) {}

    bool resume() {
        if (!handle_.done())
            handle_.resume();
        return !handle_.done();
    }

    ~Task() {
        if (handle_)
            handle_.destroy();
    }

private:
    handle_type handle_;
};

struct Task::promise_type {
 Task get_return_object() noexcept {
        return Task{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept {
        return {};
    }
    std::suspend_always final_suspend() noexcept {
        return {};
    }
    void return_void() noexcept {}
    void unhandled_exception() noexcept {
        std::terminate();
    }
};

Task foo() {
    std::cout << "Started coroutine foo\n";
    co_await std::suspend_always{};
    std::cout << "Resumed coroutine foo\n";
    co_return;
}

int main() {
    Task c = foo();
    // c is now in a suspended state
    std::cout << "Returned from foo()\n";
    c.resume();
    // c has now completed execution
}