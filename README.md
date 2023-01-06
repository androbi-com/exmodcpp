# C++ examples

I will collect some of the C++ examples that I have made while learning 
"modern" C++ in this repository. 

I comment more than usual here, so as to emphasize certain points. Each subdirectory 
corresponds to a different example and produces an executable in `build`. 

* coro_task - a simple example of a coroutine-based task scheduler
* hello - a "hello world" could not be missing here, use it as a base for new examples
* pimpl - see Item 22 in Effective Modern C++ by Scott Meyers
* plot - a small example for doing numerics with std::array, we also use the sciplot
  library to display a function using gnuplot. Install gnuplot-qt for the UI to work.  
* type_erasure -  type erasure as presented by Klaus Igelberger

# Setup

I use VS Code with the following extensions:

* C/C++ (Microsoft)
* CMake
* CMake Tools
* C/C++ Advanced Lint (configured to use clang-15 and c++20, see .vscode/settings.json)
  
The clang-12 compiler is used as "active kit" in CMake Tools.