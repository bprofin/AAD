#pragma once
#include <cstddef>
#include <vector>
#include <iostream>
#include "api/export.h"

namespace aad {
    enum OpType {
        OP_ADD,
        OP_ADD_LEFT,
        OP_ADD_RIGHT,
        OP_SUB,
        OP_SUB_LEFT,
        OP_SUB_RIGHT,
        OP_MUL,
        OP_MUL_LEFT,
        OP_MUL_RIGHT,
        OP_DIV,
        OP_DIV_LEFT,
        OP_DIV_RIGHT,
        OP_EXP,
        OP_LOG
    };

    inline std::ostream &operator<<(std::ostream &os, OpType e) {
        switch (e) {
            case OpType::OP_ADD: return os << "OP_ADD";
            case OpType::OP_ADD_LEFT: return os << "OP_ADD_LEFT";
            case OpType::OP_ADD_RIGHT: return os << "OP_ADD_RIGHT";

            case OpType::OP_SUB: return os << "OP_SUB";
            case OpType::OP_SUB_LEFT: return os << "OP_SUB_LEFT";
            case OpType::OP_SUB_RIGHT: return os << "OP_SUB_RIGHT";

            case OpType::OP_MUL: return os << "OP_MUL";
            case OpType::OP_MUL_LEFT: return os << "OP_MUL_LEFT";
            case OpType::OP_MUL_RIGHT: return os << "OP_MUL_RIGHT";

            case OpType::OP_DIV: return os << "OP_DIV";
            case OpType::OP_DIV_LEFT: return os << "OP_DIV_LEFT";
            case OpType::OP_DIV_RIGHT: return os << "OP_DIV_RIGHT";

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
        size_t b; // unused for unary ops
    };

    inline std::ostream &operator<<(std::ostream &os, const Op &op) {
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
