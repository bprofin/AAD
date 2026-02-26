#pragma once
#include <vector>
#include "common/real.h"
#include "common/tape.h"


namespace aad {

    template <typename Func>
    std::vector<double> grad(Func f, const std::vector<double>& inputs) {
        Tape tape;
        std::vector<Real> x;
        x.reserve(inputs.size());

        for (double v : inputs) {
            x.push_back({tape.new_var(v), &tape});
        }

        Real out = f(x);
        tape.backward(out.idx);

        std::vector<double> g;
        g.reserve(inputs.size());
        for (auto& xi : x) {
            g.push_back(xi.grad());
        }

        return g;
    }
}