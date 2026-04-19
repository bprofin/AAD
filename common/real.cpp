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

    //////////////////////////////////
    ///OP Real Real //////////////////
    //////////////////////////////////
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

    //////////////////////////////////
    /// OP Real double //////////////
    //////////////////////////////////
    Real Real::operator+(double rhs) const {
        double v = val() + rhs;
        size_t out = tape->new_var(v);
        // store constant in 'b' so backward pass knows it
        tape->record(OP_ADD_LEFT, out, idx, tape->new_var(rhs));
        return {out, tape};
    }

    Real Real::operator-(double rhs) const {
        double v = val() - rhs;
        size_t out = tape->new_var(v);
        tape->record(OP_SUB_LEFT, out, idx, tape->new_var(rhs));
        return {out, tape};
    }

    Real Real::operator*(double rhs) const {
        double v = val() * rhs;
        size_t out = tape->new_var(v);
        tape->record(OP_MUL_LEFT, out, idx, tape->new_var(rhs)); // store rhs
        return {out, tape};
    }

    Real Real::operator/(double rhs) const {
        double v = val() / rhs;
        size_t out = tape->new_var(v);
        tape->record(OP_DIV_LEFT, out, idx, tape->new_var(rhs)); // store rhs
        return {out, tape};
    }

}
