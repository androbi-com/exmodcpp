# C++ examples

Some C++ examples. Each subdirectory corresponds to a different example 
and produces an executable in `build`. 

    mkdir build
    cd build
    cmake [-D CMAKE_C_COMPILER=nvc -D CMAKE_CXX_COMPILER=nvc++] ..
    cmake --build .

* hello - a "hello world" could not be missing here, use it as a base for new examples
* plot - a small example for doing numerics with std::array, we also use the sciplot
         library to display a function using gnuplot. Install gnuplot-qt for the UI to work.  

## Coroutines

* coro_task - a simple example of a coroutine-based task 

## Pointer to implementation - pImpl

* pimpl - see Item 22 in Effective Modern C++ by Scott Meyers

## Examples with stdexec - std::execution proposal for C++26

Some examples for the [P2300 proposal](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2300r7.html). 
See also https://www.hpcwire.com/2022/12/05/new-c-sender-library-enables-portable-asynchrony/

* stdexec_hello - a hello world with stdexec - https://github.dev/NVIDIA/stdexec
* stdexec_mandel - a mandelbrot example using "bulk"
* stdexec_partasks - a parallel tasks example using "when_all"

## Thread examples

* threads_condvar thread communication with [std::condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)
* youtube see https://www.youtube.com/watch?v=ywJ4cq67-uc&ab_channel=CppNow
  
## Type erasure

As presented in https://youtu.be/qn6OqefuH08. Also see
https://meetingcpp.com/mcpp/slides/2021/Type%20Erasure%20-%20A%20Design%20Analysis9268.pdf

* type_erasure -  type erasure as presented by Klaus Igelberger

# Workspace setup

I typically use VS Code with the following extensions:

* C/C++ (Microsoft)
* CMake
* CMake Tools
* C/C++ Advanced Lint (configured to use clang-17 and c++2b, see .vscode/settings.json)
* Copilot
* PDF Viewer (mathematic.vscode-pdf)
  
As a C++ compiler gcc-11 is currently used as "active kit" in CMake Tools. 

This repository includes a `.devcontainer` setup which permits to run a development
environment on a local Docker environment or on [codespaces](https://github.com/codespaces). 
It is the default C++ image as provided by Microsoft (see https://github.com/devcontainers/images/tree/main/src/cpp)
with a recent `cmake` and `clang`. The extensions "PDF Viewer", "Copilot" and 
"C/C++ Advanced Lint" are installed with the standard ones provided by 
Microsoft (C/C++, CMake, CMakeTools).

For scientific projects graphical output is important. This can be achieved in two ways:

* install X capabilities in the codespace image and connecting with ssh to the codespace
  using X forwarding.
* use PDF files as output and display these files with an extension

In the first case, install `gnuplot-qt` and `xauth` (see Dockerfile) and add the following 
feature in `devcontainer.json` which enables a `sshd` server on the codespace

    "features": {
      "ghcr.io/devcontainers/features/sshd:1": {
        "version": "latest"
      }
    },

The line 

    canvas.show();
    
in `plot/main.cpp` opens a X window that will be displayed on your X-client after 
opening a ssh connection with X forwarding

    gh codespace ssh -- -XY

Once the connection is established, run

    ./build/plot/plot

In the second case (default here), we only install `gnuplot-nox` and no X related stuff,
this results in a smaller image. Comment the above mentioned line in this case. The `plot`
application creates a file `sine-function.pdf` which you can display with a suitable 
extension (PDF Viewer is installed).