#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

template<typename T>
class interpolator_1d {
public:
    interpolator_1d(const std::vector<T> &X, const std::vector<T> &Y){
        initialize(X, Y);
    }

    virtual ~interpolator_1d() = default;

    // pure virtual evaluation function
    virtual T operator()(T x) const = 0;

    const std::vector<T> &X() const { return X_; }
    const std::vector<T> &Y() const { return Y_; }

protected:
    void initialize(const std::vector<T>& X, const std::vector<T>& Y) {

        if (X.size()==0 || Y.size()==0) {
            throw std::invalid_argument("X and Y must have at least one element");
        }

        if (X.size() != Y.size()) {
            throw std::invalid_argument("X and Y must have the same size");
        }

        if (X.empty()) {
            X_.clear();
            Y_.clear();
            return;
        }

        std::vector<std::pair<T, T>> xy;
        xy.reserve(X.size());

        for (std::size_t i = 0; i < X.size(); ++i) {
            xy.emplace_back(X[i], Y[i]);
        }

        std::sort(xy.begin(), xy.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });

        // Check for duplicates in the base
        for (std::size_t i = 1; i < xy.size(); ++i) {
            if (xy[i].first == xy[i - 1].first) {
                throw std::runtime_error("Duplicate X values are not allowed");
            }
        }

        // clear and fill X_ and Y_ without default constructor
        X_.clear(); X_.reserve(xy.size());
        Y_.clear(); Y_.reserve(xy.size());
        for (auto& p : xy) {
            X_.push_back(p.first);
            Y_.push_back(p.second);  // works for types like aad::Real
        }
    }

private:
    std::vector<T> X_;
    std::vector<T> Y_;
};

template <typename T>
class linear_interpolator_1d : public interpolator_1d<T> {
public:
    linear_interpolator_1d(const std::vector<T>& X,
                           const std::vector<T>& Y)
        : interpolator_1d<T>(X, Y)
    {
        precompute();
    }

    T operator()(T x) const override {
        const auto& X = this->X();
        const auto& Y = this->Y();
        const std::size_t n = X.size();

        if (n == 1) return Y[0];

        auto it = std::upper_bound(X.begin(), X.end(), x);
        std::size_t i;

        if (it == X.begin()) i = 0;
        else if (it == X.end()) i = n - 2;
        else i = static_cast<std::size_t>(it - X.begin() - 1);

        return Y[i] + slopes_[i] * (x - X[i]);
    }

private:
    void precompute() {
        const auto& X = this->X();
        const auto& Y = this->Y();
        const std::size_t n = X.size();

        if (n < 2) {
            slopes_.clear();
            return;
        }

        slopes_.resize(n - 1);
        for (std::size_t i = 0; i < n - 1; ++i) {
            slopes_[i] = (Y[i + 1] - Y[i]) / (X[i + 1] - X[i]);
        }
    }

private:
    std::vector<T> slopes_;
};

template <typename T>
class spline_interpolator_1d : public interpolator_1d<T> {
public:
    spline_interpolator_1d(const std::vector<T>& X,
                           const std::vector<T>& Y)
        : interpolator_1d<T>(X, Y)
    {
        precompute();
    }

    T operator()(T x) const override {
        const auto& X = this->X();
        const auto& Y = this->Y();
        const std::size_t n = X.size();

        if (n == 1) return Y[0];

        auto it = std::upper_bound(X.begin(), X.end(), x);
        std::size_t i;

        if (it == X.begin()) i = 0;
        else if (it == X.end()) i = n - 2;
        else i = static_cast<std::size_t>(it - X.begin() - 1);

        const T h = X[i + 1] - X[i];
        const T a = (X[i + 1] - x) / h;
        const T b = (x - X[i]) / h;

        return a * Y[i] + b * Y[i + 1]
             + ((a * a * a - a) * M_[i]
             +  (b * b * b - b) * M_[i + 1]) * (h * h) / 6.0;
    }

private:
    void precompute() {
        const auto& X = this->X();
        const auto& Y = this->Y();
        const std::size_t n = X.size();

        if (n < 2) {
            M_.clear();
            return;
        }

        if (n == 2) return; // natural spline: linear segment, M_ unused

        M_.reserve(n);
        std::vector<T> a, b, c, d, c_star, d_star;
        a.reserve(n);
        b.reserve(n);
        c.reserve(n);
        d.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            M_.push_back(Y[i] * 0.0);
            a.push_back(Y[i] * 0.0);
            b.push_back(Y[i] * 0.0);
            c.push_back(Y[i] * 0.0);
            d.push_back(Y[i] * 0.0);
            c_star.push_back(Y[i] * 0.0);
            d_star.push_back(Y[i] * 0.0);
        }

        for (std::size_t i = 1; i < n - 1; ++i) {
            const T h_i   = X[i]     - X[i - 1];
            const T h_ip1 = X[i + 1] - X[i];

            a[i] = h_i;
            b[i] = 2.0 * (h_i + h_ip1);
            c[i] = h_ip1;

            d[i] = 6.0 * ((Y[i + 1] - Y[i]) / h_ip1 - (Y[i] - Y[i - 1]) / h_i);
        }

        c_star[1] = c[1] / b[1];
        d_star[1] = d[1] / b[1];

        for (std::size_t i = 2; i < n - 1; ++i) {
            const T m = b[i] - a[i] * c_star[i - 1];
            c_star[i] = c[i] / m;
            d_star[i] = (d[i] - a[i] * d_star[i - 1]) / m;
        }

        for (std::size_t i = n - 2; i-- > 0;) {
            M_[i] = d_star[i] - c_star[i] * M_[i + 1];
        }
    }

private:
    std::vector<T> M_; // second derivatives
};