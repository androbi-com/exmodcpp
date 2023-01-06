#include <iostream>
#include <memory>
#include <vector>

// these are our widgets, concrete types, no inheritance necessary
struct Clock 
{
    Clock() : id{++count}
    { 
        std::cout << "Create Clock " << id << std::endl;
    }
    static int count;
    int id;
};
int Clock::count = 0;

struct Weather {
    Weather()
    {
        std::cout << "Create Weather" << std::endl;
    }
};

// implementations of print() for our widgets
void print(Clock const& clock)
{
    std::cout << "Clock " << clock.id << std::endl;
}

void print(Weather const&)
{
    std::cout << "Weather" << std::endl;
}

// Widget encapsulates external polymorphism mechanism
class Widget
{
  private:

    // base class for external polymorphism
    struct WidgetConcept 
    {
        virtual ~WidgetConcept() = default;
        virtual void do_print() const = 0;
    };

    // this templated class provides access to the implementations of the common polymorphic 
    // behaviour of "widgets" by delegating to the overloaded print function
    template <typename WidgetT>
    struct WidgetModel : WidgetConcept 
    {
        explicit WidgetModel(const WidgetT widget)
            : widget_{widget}
        {}
        // make any class with a overloaded implementation of print(WidgetT) available to the
        // polymorphic behaviour introduced by by WidgetConcept
        void do_print() const override 
        { 
            print(widget_);
        }
        // each WidgetModel has a widget of type WidgetT
        WidgetT widget_;
    };

    // define a friend function to allow it to access the private
    // pimpl_ member. This is a free function, not a member function
    // Here we invoke the polymorphic behaviour of the WidgetConcept class
    friend void friend_print(Widget const& widget)
    {
        widget.pimpl_->do_print();
    }

    std::unique_ptr<WidgetConcept> pimpl_;
  public:
    // here is where type erasure happens
    template <typename WidgetT>
    explicit Widget(WidgetT widget)
        : pimpl_{std::make_unique<WidgetModel<WidgetT>>( std::move(widget) )}
    {}
};

// iterate over the vector of pointers to the WidgetConcept class and invoke
// the polymorphic for print
void printAllWidgets(std::vector<Widget> const & widgets)
{
    for (auto const& widget : widgets)
    {
        // call the friend function to invoke the polymorphic behaviour
        friend_print(widget);
    }
}

int main()
{
    // create a vector of pointers to the base class
    std::vector<Widget> widgets;
    // add some widgets
    widgets.emplace_back(Clock{});
    widgets.emplace_back(Clock{});
    widgets.emplace_back(Weather{});
    // print the widgets
    printAllWidgets(widgets);
}