#include <iostream>
#include <memory>
#include <vector>

// these are our widgets, concrete types, no inheritance necessary
struct Clock 
{
    Clock() : id{++count}
    {}
    static int count;
    int id;
};
int Clock::count = 0;

struct Weather {};

// base class for external polymorphism, see https://www.dre.vanderbilt.edu/~schmidt/PDF/C++-EP.pdf
// this class provides the interface for the common polymorphic behaviour of "widgets"
struct WidgetConcept 
{
    virtual ~WidgetConcept() = default;
    virtual void do_print() const = 0;
};

// this class provides access to the implementations of the common polymorphic 
// behaviour of "widgets" by delegating to the implementation of the print function
template <typename WidgetT>
struct WidgetModel : WidgetConcept 
{
    explicit WidgetModel(const WidgetT widget)
        : widget_{widget}
    {}
    // make any class with an implementation of print(WidgetT) available to the
    // polymorphic behaviour introduced by by WidgetConcept
    void do_print() const override 
    { 
        print(widget_); 
    }
    // each WidgetModel has a widget
    WidgetT widget_;
};

// implementation of print for our widgets
void print(Clock const& clock)
{
    std::cout << "Clock " << clock.id << std::endl;
}
void print(Weather const &)
{
    std::cout << "Weather" << std::endl;
}

// iterate over the vector of pointers to the WidgetConcept class and invoke
// the polymorphic for print
void printAllWidgets(std::vector<std::unique_ptr<WidgetConcept>> const & widgets)
{
    for (auto const& widget : widgets)
    {
        widget->do_print();
    }
}

int main()
{
    // create a vector of pointers to the base class
    std::vector<std::unique_ptr<WidgetConcept>> widgets;
    // add some widgets
    widgets.emplace_back(std::make_unique<WidgetModel<Clock>>(Clock{}));
    widgets.emplace_back(std::make_unique<WidgetModel<Clock>>(Clock{}));
    widgets.emplace_back(std::make_unique<WidgetModel<Weather>>(Weather{}));
    // print the widgets
    printAllWidgets(widgets);
}