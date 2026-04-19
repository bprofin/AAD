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

        /*
        Real& operator=(const Real& rhs) {
            idx = rhs.idx;
            tape = rhs.tape;
            return *this;
        }

        Real& operator=(Real&& rhs) noexcept {
            idx = rhs.idx;
            tape = rhs.tape;
            return *this;
        }*/

        AAD_API double val() const;
        AAD_API double grad() const;

        // Binary operators (Real op Real)
        AAD_API Real operator+(const Real& rhs) const;
        AAD_API Real operator-(const Real& rhs) const;
        AAD_API Real operator*(const Real& rhs) const;
        AAD_API Real operator/(const Real& rhs) const;

        // Binary operators (Real op double)
        AAD_API Real operator+(double rhs) const;
        AAD_API Real operator-(double rhs) const;
        AAD_API Real operator*(double rhs) const;
        AAD_API Real operator/(double rhs) const;


    };

    // Binary operators (double op Real)
    AAD_API inline Real operator+(double lhs, const Real &rhs) {
        double v = lhs + rhs.val();
        size_t out = rhs.tape->new_var(v);
        rhs.tape->record(OP_ADD_RIGHT, out, rhs.tape->new_var(lhs), rhs.idx);
        return {out, rhs.tape};
    }

    AAD_API inline Real operator-(double lhs, const Real &rhs) {
        double v = lhs - rhs.val();
        size_t out = rhs.tape->new_var(v);
        rhs.tape->record(OP_SUB_RIGHT, out, rhs.tape->new_var(lhs), rhs.idx);
        return {out, rhs.tape};
    }

    AAD_API inline Real operator*(double lhs, const Real &rhs) {
        double v = lhs * rhs.val();
        size_t out = rhs.tape->new_var(v);
        rhs.tape->record(OP_MUL_RIGHT, out, rhs.tape->new_var(lhs), rhs.idx);
        return {out, rhs.tape};
    }

    AAD_API inline Real operator/(double lhs, const Real &rhs) {
        double v = lhs / rhs.val();
        size_t out = rhs.tape->new_var(v);
        rhs.tape->record(OP_DIV_RIGHT, out, rhs.tape->new_var(lhs), rhs.idx);
        return {out, rhs.tape};
    }

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

    AAD_API inline auto operator<=>(const Real& lhs, const Real& rhs) {
        return lhs.val() <=> rhs.val();
    }

    AAD_API inline bool operator==(const Real& lhs, const Real& rhs) {
        return lhs.val() == rhs.val();
    }
}