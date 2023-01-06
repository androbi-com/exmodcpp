#include <iostream>
#include "widget.hpp"

struct Widget::Impl
{
    std::string name;
    void doSomething()
    {
        std::cout << this->name << std::endl;
    }
};

// default constructor, initialize pImpl
Widget::Widget(const std::string& name) : 
    pImpl(std::make_unique<Impl>())
{
    pImpl->name = name;
}

// default destructor, defined after definition of Widget::Impl
Widget::~Widget() = default;

// copy constructor
Widget::Widget(const Widget& rhs) : 
    pImpl(nullptr) 
{
    std::cout << "copy constructor" << std::endl;
    if (rhs.pImpl)
    {
        pImpl = std::make_unique<Impl>(*rhs.pImpl);
        pImpl->name = rhs.pImpl->name + " copy";
    }
}

// assignment operator
Widget& Widget::operator=(const Widget& rhs)
{
    std::cout << "assignment operator" << std::endl;
    if (!rhs.pImpl) { // rhs is empty -> clear pImpl    
        pImpl.reset();
    } else if (!pImpl) { // rhs is not empty, but pImpl is -> create pImpl
        pImpl = std::make_unique<Impl>(*rhs.pImpl);
    } else { // rhs and pImpl are not empty -> copy rhs.pImpl to pImpl elementwise
        *pImpl = *rhs.pImpl;
    }
    return *this;
}

// move constructor and assignment operator are default 
Widget::Widget(Widget&& rhs) noexcept = default;
Widget& Widget::operator=(Widget&& rhs) noexcept = default;

// Widget::Impl is now defined, use it
void Widget::doSomething()
{
    pImpl->doSomething();
}

