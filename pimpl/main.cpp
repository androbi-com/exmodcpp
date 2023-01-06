// note we only need to include "widget.hpp" in main.cpp
#include "widget.hpp"

// example main
int main()
{
    // create a new instance of the class
    Widget w;
    w.doSomething();
    auto w2 = w;
    w2.doSomething();
    Widget w3;
    w3 = std::move(w2);
    w3.doSomething();
    //segfaults:
    //w2.doSomething();
}