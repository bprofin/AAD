#include <iostream>
#include <iomanip>
#include <limits>
#include "aad.h"

// --------------------------------------------------
// Function written ONCE as a template
// --------------------------------------------------
template <typename T>
T f(const std::vector<T>& x) {
    // f(x,y) = log(x*y + x - y) + exp(x/y)
    return log(x[0] * x[1] + x[0] - 2 * x[1]) + exp(x[0] / x[1]);
}

int main(int argc, char** argv) {
    std::cout<< "Testing Function f(x,y) = log(x*y + x - y) + exp(x/y)" <<std::endl;
    std::vector<double> inputs = {2.0, 3.0};
    std::cout << "Inputs: (x=" << inputs[0] << ", y=" << inputs[1] << ")" << std::endl;

    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);

    // --------------------------------------------------
    // 1. AAD Gradient
    // --------------------------------------------------

    auto g_aad = aad::grad(
    static_cast<aad::Real(*)(const std::vector<aad::Real>&)>(f<aad::Real>),
    inputs);

    std::cout << "\nAAD Gradient:\n";
    std::cout << "df/dx = " << g_aad[0] << "\n";
    std::cout << "df/dy = " << g_aad[1] << "\n";

    // --------------------------------------------------
    // 2. Analytic Gradient
    // --------------------------------------------------
    double x = inputs[0];
    double y = inputs[1];

    double denom = x*y + x - 2 * y;
    double exp_term = std::exp(x/y);

    double dfdx_analytic =
        (y + 1.0) / denom
        + exp_term * (1.0 / y);

    double dfdy_analytic =
        (x - 2.0) / denom
        - exp_term * (x / (y*y));

    std::cout << "\nAnalytic Gradient:\n";
    std::cout << "df/dx = " << dfdx_analytic << "\n";
    std::cout << "df/dy = " << dfdy_analytic << "\n";

    // --------------------------------------------------
    // 3. Finite Difference Gradient (central)
    // --------------------------------------------------
    double h = 1e-6;
    std::vector<double> g_fd(2);

    for (size_t i = 0; i < inputs.size(); ++i) {
        std::vector<double> x_plus = inputs;
        std::vector<double> x_minus = inputs;

        x_plus[i] += h;
        x_minus[i] -= h;

        g_fd[i] =
            (f<double>(x_plus) - f<double>(x_minus)) / (2.0 * h);
    }

    std::cout << "\nFinite Difference Gradient:\n";
    std::cout << "(Central difference 1e-06)\n";
    std::cout << "df/dx = " << g_fd[0] << "\n";
    std::cout << "df/dy = " << g_fd[1] << "\n";

    return 0;

}