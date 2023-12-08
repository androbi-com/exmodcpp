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

# Workspace setup

I use VS Code with the following extensions:

* C/C++ (Microsoft)
* CMake
* CMake Tools
* C/C++ Advanced Lint (configured to use clang-17 and c++2b, see .vscode/settings.json)
* Copilot
  
gcc-11 is normally used as "active kit" in CMake Tools.

This repository includes a `.devcontainer` setup which permits to run a development
environment on a local Docker environment or on codespaces. It is the default C++ image
as provided by Microsoft (see https://github.com/devcontainers/images/tree/main/src/cpp)
with a recent `cmake` and `clang`. The extensions "Copilot" and "C/C++ Advanced Lint" 
are installed with the standard ones provided by Microsoft (C/C++, CMake, CMakeTools).

We also install `gnuplot-qt` and `xauth` for the `plot` application. As this application has a GUI 
you need to ssh into codespace using `gh` with X forwarding

    gh codespace ssh -- -XY

Check by running 

    ./build/plot/plot


