#pragma once
#include "api/export.h"
#include "common/tape.h"
#include <cstddef>
#include <cmath>

namespace aad {
    struct Tape;

    AAD_VISIBILITY struct Real {
        size_t idx;
        Tape* tape;

        AAD_API double val() const;
        AAD_API double grad() const;

        // Binary operators
        AAD_API Real operator+(const Real& rhs) const;
        AAD_API Real operator-(const Real& rhs) const;
        AAD_API Real operator*(const Real& rhs) const;
        AAD_API Real operator/(const Real& rhs) const;
    };

    // =======================
    // Unary functions
    // =======================
    AAD_API inline Real exp(const Real &x) {
        double v = std::exp(x.val());
        size_t out = x.tape->new_var(v);
        x.tape->record(OP_EXP, out, x.idx);
        return {out, x.tape};
    }

    AAD_API inline Real log(const Real &x) {
        double v = std::log(x.val());
        size_t out = x.tape->new_var(v);
        x.tape->record(OP_LOG, out, x.idx);
        return {out, x.tape};
    }
}