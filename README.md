# C++ examples

Some C++ examples. Each subdirectory corresponds to a different example 
and produces an executable in `build`. 

* hello - a "hello world" could not be missing here, use it as a base for new examples
* plot - a small example for doing numerics with std::array, we also use the sciplot
         library to display a function using gnuplot. Install gnuplot-qt for the UI to work.  

## coroutines

* coro_task - a simple example of a coroutine-based task 

## Pointer to implementation - pImpl

* pimpl - see Item 22 in Effective Modern C++ by Scott Meyers

## stdexec - std::execution proposal for C++26

Some examples for the [P2300 proposal](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2300r7.html). 
See also https://www.hpcwire.com/2022/12/05/new-c-sender-library-enables-portable-asynchrony/

* stdexec_hello - a hello world with stdexec
  https://github.dev/NVIDIA/stdexec
* stdexec_mandel - a mandelbrot example using "bulk"
* stdexec_partasks - a parallel tasks example using "when_all"
  
## type erasure

As presented in https://youtu.be/qn6OqefuH08. Also see
https://meetingcpp.com/mcpp/slides/2021/Type%20Erasure%20-%20A%20Design%20Analysis9268.pdf

* type_erasure -  type erasure as presented by Klaus Igelberger

# Setup

I use VS Code with the following extensions:

* C/C++ (Microsoft)
* CMake
* CMake Tools
* C/C++ Advanced Lint (configured to use clang-15 and c++20, see .vscode/settings.json)
  
The clang-12 compiler is used as "active kit" in CMake Tools.