#include "common/real.h"
#include "common/tape.h"
#include <cmath>

namespace aad {
    double Real::val() const {
        return tape->vals[idx];
    }

    double Real::grad() const {
        return tape->grads[idx];
    }

    Real Real::operator+(const Real &rhs) const {
        double v = val() + rhs.val();
        size_t out = tape->new_var(v);
        tape->record(OP_ADD, out, idx, rhs.idx);
        return {out, tape};
    }

    Real Real::operator-(const Real &rhs) const {
        double v = val() - rhs.val();
        size_t out = tape->new_var(v);
        tape->record(OP_SUB, out, idx, rhs.idx);
        return {out, tape};
    }

    Real Real::operator*(const Real &rhs) const {
        double v = val() * rhs.val();
        size_t out = tape->new_var(v);
        tape->record(OP_MUL, out, idx, rhs.idx);
        return {out, tape};
    }

    Real Real::operator/(const Real &rhs) const {
        double v = val() / rhs.val();
        size_t out = tape->new_var(v);
        tape->record(OP_DIV, out, idx, rhs.idx);
        return {out, tape};
    }

}
