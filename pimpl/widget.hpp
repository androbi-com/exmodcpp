#include <memory>
#include <string>

// example for class that uses pimpl idiom
// see Item 22 in Effective Modern C++ by Scott Meyers
class Widget
{
    public:
        // default constructor and destructor
        Widget(const std::string& name = "default");
        ~Widget();

        // copy constructor and assignment operator
        Widget(const Widget& rhs);
        Widget& operator=(const Widget& rhs);

        // move constructor and assignment operator
        Widget(Widget&& rhs) noexcept;
        Widget& operator=(Widget&& rhs) noexcept;

        void doSomething();
    private:
        // pimpl: only forward declare the implementation
        struct Impl;
        // pimpl: use std::unique_ptr to manage the implementation
        std::unique_ptr<Impl> pImpl;
};
