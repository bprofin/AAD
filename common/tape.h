#pragma once
#include <cstddef>
#include <vector>

#include "common/op.h"

namespace aad {
    struct Tape {
        std::vector<double> vals;
        std::vector<double> grads;
        std::vector<Op> ops;

        size_t new_var(double v) {
            size_t idx = (size_t) vals.size();
            vals.push_back(v);
            grads.push_back(0.0);
            return idx;
        }

        void record(OpType type, size_t out, size_t a, size_t b = 0) {
            ops.push_back({type, out, a, b});
        }

        void reset_grads() {
            for (auto &g: grads) g = 0.0;
        }

        void backward(size_t out_idx) {
            reset_grads();
            grads[out_idx] = 1.0;

            for (int i = ops.size() - 1; i >= 0; --i) {
                const Op &op = ops[i];

                switch (op.type) {
                    case OP_ADD:
                        grads[op.a] += grads[op.out];
                        grads[op.b] += grads[op.out];
                        break;

                    case OP_SUB:
                        grads[op.a] += grads[op.out];
                        grads[op.b] -= grads[op.out];
                        break;

                    case OP_MUL:
                        grads[op.a] += vals[op.b] * grads[op.out];
                        grads[op.b] += vals[op.a] * grads[op.out];
                        break;

                    case OP_DIV:
                        grads[op.a] += grads[op.out] / vals[op.b];
                        grads[op.b] -= grads[op.out] * vals[op.a] / (vals[op.b] * vals[op.b]);
                        break;

                    case OP_EXP:
                        grads[op.a] += grads[op.out] * vals[op.out];
                        break;

                    case OP_LOG:
                        grads[op.a] += grads[op.out] / vals[op.a];
                        break;
                }
            }
        }
    };
}
