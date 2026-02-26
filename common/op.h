#pragma once
#include <cstddef>
#include <vector>
#include <iostream>
#include "api/export.h"

namespace aad {

    enum OpType {
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_EXP,
        OP_LOG
    };

    inline std::ostream &operator<<(std::ostream &os, OpType e) {
        switch (e) {
            case OpType::OP_ADD: return os << "OP_ADD";
            case OpType::OP_SUB: return os << "OP_SUB";
            case OpType::OP_MUL: return os << "OP_MUL";
            case OpType::OP_DIV: return os << "OP_DIV";
            case OpType::OP_EXP: return os << "EXP";
            case OpType::OP_LOG: return os << "LOG";
            default: return os << "UNKNOWN";
        }
    }

    // =======================
    // Operation record
    // =======================
    AAD_VISIBILITY struct Op {
        OpType type;
        size_t out;
        size_t a;
        size_t b;   // unused for unary ops
    };

    inline std::ostream& operator<<(std::ostream& os, const Op& op) {
        os << "Op(type=" << op.type
           << ", out=" << op.out
           << ", a=" << op.a;

        // Only print b if it's used
        if (op.type == OP_ADD || op.type == OP_SUB ||
            op.type == OP_MUL || op.type == OP_DIV) {
            os << ", b=" << op.b;
            }

        os << ")";
        return os;
    }
}