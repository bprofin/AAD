#include "api/export.h"
#include "common/grad.h"
#include <functional>

namespace aad {

    template std::vector<double> AAD_API grad<std::function<Real(const std::vector<Real>&)>>(
        std::function<Real(const std::vector<Real>&)> f,
        const std::vector<double>& inputs
    );

    using VectorOfRealToRealFuncPtr = aad::Real(*)(const std::vector<aad::Real>&);
    template std::vector<double> AAD_API grad<VectorOfRealToRealFuncPtr>(
        VectorOfRealToRealFuncPtr f,
        const std::vector<double>& inputs
    );

}