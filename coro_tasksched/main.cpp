#include <iostream>
#include <coroutine>
#include <string>
#include <queue>
#include <stack>
#include <fmt/core.h> // for fmt::format, substitute with std::format when compiler support exists

//#define DEBUG_

class Task {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    // constructor from coroutine handle
    explicit Task(handle_type h) : handle_(h) {
#ifdef DEBUG_
        std::cout << "Task::Task(handle_type) " << (handle_ ? "non-null" : "null") << "\n";
#endif
    }
    // no copy
    Task(const Task&) = delete;
    // no assignment
    Task(Task&&) = delete;
    // no copy assignment
    Task& operator=(const Task&) = delete;
    // no move assignment
    Task& operator=(Task&&) = delete;

    bool resumeTask() {
        if (!handle_.done()) {
            //std::cout << "Task::resumeTask()\n";
            // this calls the resume() method on the coroutine handle
            handle_.resume();
        }
        return !handle_.done();
    }

    auto get_handle() {
        return handle_;
    }
    // destroy coroutine handle on destruction
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

// the promise_type contains "plugins" to control the coroutine
struct Task::promise_type {
    // return the Task object to be used as the coroutine handle
    Task get_return_object() noexcept {
#ifdef DEBUG_
        std::cout << "promise_type::get_return_object()\n";
#endif
        return Task{handle_type::from_promise(*this)};
    }
    // returns a suspend_always object to suspend the coroutine on initial startup
    std::suspend_always initial_suspend() noexcept {
#ifdef DEBUG_
        std::cout << "promise_type::initial_suspend()\n";
#endif
        return {};
    }
    // returns a suspend_always object to suspend the coroutine when it co_returns
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

class Scheduler {

    std::queue<std::coroutine_handle<>> _handles;
    //std::stack<std::coroutine_handle<>> _handles;

public: 

    // add a task to the scheduler
    void emplace(std::coroutine_handle<> task) {
#ifdef DEBUG_
        std::cout << fmt::format("emplace task: {}\n", task.address());
#endif
        _handles.push(task);
    }

    // run all tasks in the scheduler
    void schedule() {
        while(!_handles.empty()) {
            //auto task = _handles.front();

            // get a task from the top of the stack and resume it
            auto task_handle = _handles.front();
            _handles.pop();
#ifdef DEBUG_
            std::cout << fmt::format("schedule task: {}\n", task_handle.address());
#endif
            task_handle.resume();

            // if the task is not yet done, push it back to the stack
            if(!task_handle.done()) { 
                _handles.push(task_handle);
            }
        }
    }

    auto suspend() {
        return std::suspend_always{};
    }
};

Task TaskA(Scheduler& sch) { //NOLINT
  std::cout << "Hello from TaskA\n";
  co_await sch.suspend();
  std::cout << "Executing the TaskA\n";
  co_await sch.suspend();
  std::cout << "TaskA is finished\n";
}

Task TaskB(Scheduler& sch) { //NOLINT
  std::cout << "Hello from TaskB\n";
  co_await sch.suspend();
  std::cout << "Executing the TaskB\n";
  co_await sch.suspend();
  std::cout << "TaskB is finished\n";
}

int main() {
    std::cout << "main(): starting\n";
    /*
    // create coroutine objects
    Task a = foo("pepe");
    Task b = foo("pepa");
    // initial startup
    a.resume();
    b.resume();
 
    // second resume = actually run
    a.resume();
    b.resume();
    */
    Scheduler sch;
    std::cout << "create tasks\n";
    auto taskA = TaskA(sch);
    auto taskB = TaskB(sch);
    std::cout << "add tasks to scheduler\n";
    sch.emplace(taskA.get_handle());
    sch.emplace(taskB.get_handle());

    std::cout << "Start scheduling...\n";

    sch.schedule();


    std::cout << "main(): exiting\n";
}