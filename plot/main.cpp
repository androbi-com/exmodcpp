#include <iostream>
#include <array>
#include <functional>
#include <sciplot/sciplot.hpp>

// integrate function given by y using trapezoidal rule with fixed step size delta
template <typename T, std::size_t N>
auto integrate(const std::array<T,N>& y, T delta) {
    // check if N is at least 2

    // check if y has at least 2 elements
    if (y.size() < 2) {
        throw std::invalid_argument("number of points must be at least 2");
    }
    const T half = 0.5;
    // trapezoidal rule: sum_{i=1}^{N-1} (y[i-1] + y[i]) * delta / 2
    // endpoints contribute only once, midpoints twice
    T sum = half * (y[0] + y[N - 1]);
    for (std::size_t i = 1; i < N - 1; ++i) {
        sum += y[i]; // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    return sum * delta;
}

// create linearly spaced points between a and b in points
template <typename T, std::size_t N>
auto linspace(std::array<T, N>& points, T a, T b) {
    if (points.size() < 2) {
        throw std::invalid_argument("number of points must be at least 2");
    }
    const T delta = (b - a) / (points.size() - 1.0);
    for (std::size_t i = 0; i < points.size(); ++i) {
        points[i] = a + i * delta; // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    return delta;
}

template <typename T, std::size_t N>
void apply_function(const std::array<T, N>& x, std::array<T, N>& y, std::function< T(T) > f) {
    // check if x and y have the same size
    if (x.size() != y.size()) {
        throw std::invalid_argument("x and y must have the same size");
    }
    for (std::size_t i = 0; i < x.size(); ++i) {
        y[i] = f(x[i]); // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
}

int main(void)
{
    constexpr auto CANVAS_SIZE = 800;
    constexpr auto N = 100;
    // alias for the type of the data, use float, double or long double 
    using numtype = double;
    constexpr numtype factor = 2.0;
    constexpr numtype xmin = 0;
    constexpr numtype xmax = 2*M_PI;

    // use std::array for fixed size arrays, these will be assigned so no constexpr
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

    // Plot the sine function
    plot.drawCurve(x, y).label("y=sin(x)");    
    // Create figure to hold plot
    sciplot::Figure fig = {{plot}};
    // Create canvas to hold figure
    sciplot::Canvas canvas = {{fig}};

    // Set the size of the canvas
    canvas.size(CANVAS_SIZE, CANVAS_SIZE);
    //canvas.show();
    canvas.save("sine-function.pdf");
}
