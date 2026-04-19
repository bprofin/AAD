#include <iostream>
#include <stddef.h>
#include <vector>
#include <iomanip>
#include "aad.h"

#include "interpolator_1d.h"



int main() {
    std::cout << "Testing spline interpolator with AAD gradients\n";

    // --------------------------------------------------
    // 1. Define X and Y points
    // --------------------------------------------------
    std::vector<double> X = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> Y = {0.0, 0.5, 0.8, 0.9, 1.0};

    // --------------------------------------------------
    // 2. Precompute a spline interpolator
    // --------------------------------------------------
    aad::Tape tape;
    std::vector<aad::Real> real_Y, real_X ;
    real_Y.reserve(Y.size());
    for (double v : Y) {
        real_Y.push_back({tape.new_var(v), &tape});
    }
    real_X.reserve(X.size());
    for (double v : X) {
        real_X.push_back({tape.new_var(v), &tape});
    }
    spline_interpolator_1d<aad::Real> spline(real_X, real_Y);

    std::cout << "Calculation of gradient against Y_i, at the support points, which should be 1.0 or 0.0" <<std::endl<<std::endl;

    for (size_t i = 0; i < X.size(); i++) {
        auto out  = spline(real_X[i]);
        std::cout << X[i] << std::endl;
        std::cout << "Val = " << out.val() << " | " << Y[i] << std::endl;

        tape.reset_grads();
        tape.backward(out.idx);
        for (size_t j = 0; j < real_Y.size(); j++) {
            std::cout << "Grad for " << Y [j] << " = " << real_Y[j].grad() << std::endl;
        }
        std::cout<<std::endl;
    }

    return 0;
}