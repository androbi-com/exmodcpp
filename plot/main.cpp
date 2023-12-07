#include <iostream>
#include <array>
#include <functional>
#include <sciplot/sciplot.hpp>

// integrate function given by y using trapezoidal rule with fixed step size delta
template <typename T, std::size_t N>
auto integrate(const std::array<T,N>& y, T delta) noexcept {
    T sum = y[0] + y[y.size() - 1];
    for (std::size_t i = 1; i < y.size() - 1; ++i) {
        sum += 2 * y[i];
    }
    return sum * delta / 2;
}

// create linearly spaced points between a and b in points
template <typename T, std::size_t N>
auto linspace(std::array<T, N>& points, T a, T b) {
    if (points.size() < 2) {
        throw std::invalid_argument("number of points must be at least 2");
    }
    const T delta = (b - a) / (points.size() - 1.0);
    for (std::size_t i = 0; i < points.size(); ++i) {
        points[i] = a + i * delta;
    }
    return delta;
}

template <typename T, std::size_t N>
void apply_function(const std::array<T, N>& x, std::array<T, N>& y, std::function< T(T) > f) {
    for (std::size_t i = 0; i < x.size(); ++i) {
        y[i] = f(x[i]);
    }
}

int main(void)
{
    constexpr auto N = 100;
    // alias for the type of the data, use float, double or long double 
    using numtype = double;
    constexpr numtype factor = 2.0;
    constexpr numtype xmin = 0;
    constexpr numtype xmax = 2*M_PI;

    // use std::array for fixed size arrays
    auto x = std::array<numtype, N>();
    auto y = std::array<numtype, N>();
    
    // calculate x values between xmin and xmax with step size delta
    auto delta = linspace(x, xmin, xmax);

    // use a lambda to define the function and apply it as y=f(x)
    // need to specify the type of the lambda because std::function<T(T)> is no valid match
    apply_function<numtype>(x, y, [](numtype xloc)->numtype { return factor*std::sin(xloc); });
    auto result = integrate(y, delta);
    std::cout << "result:" << result << std::endl;

    // Create a Plot object
    sciplot::Plot2D plot;

    // Set the x and y labels
    plot.xlabel("x");
    plot.ylabel("y");

    // calculate minimum and maximum of y
    const auto [ymin, ymax] = std::minmax_element(y.begin(), y.end());

    // set plot ranges
    plot.xrange(xmin, xmax);
    plot.yrange(*ymin, *ymax);

    // Set the legend to be on the bottom along the horizontal
    plot.legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);

    // Draw the curve
    plot.drawCurve(x, y).label("2*sin(x)");

    // Create figure to hold plot
    sciplot::Figure fig = {{plot}};

    // Create canvas to hold figure and show it
    sciplot::Canvas canvas = {{fig}};
    canvas.size(1200, 1200);
    canvas.show();
}
