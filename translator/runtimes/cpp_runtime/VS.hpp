#ifndef VS_HPP
#define VS_HPP

#include <list>

namespace RUNTIME_NAMESPACE {

template<typename T>
class ValueSummary final {

public:
    ValueSummary():
        ValueSummary(T())
    { }

    ValueSummary(const T& v):values(std::list<Guard<T>>(1, {PathConstraint::pc(), v}))
    { }

    ValueSummary(const ValueSummary<T>& other)
    {
        // TODO: better filter initialization
        for(const auto& gv : other.values) {
            Bdd pred = PathConstraint::pc() & gv.second;
            if (!pred.isZero()) {
                values.push_back({pred, gv.first});
            }
        }
    }

    ValueSummary(const ValueSummary<T>&& other):values(other.values) { }

    ValueSummary(ValueSummary<T>&& other) = default;

    inline ValueSummary<T>& operator= (const ValueSummary<T>& rhs) {
        for(const auto& gvRhs : rhs.values) {
            Bdd pred = PathConstraint::pc() & gvRhs.second;
            if(!pred.isZero()) {
                for(auto& gvLhs : values) {
                    gvLhs.second &= !pred;
                }
                values.push_back({pred, gvRhs.first});
            }
        }
        return *this;
    }

    inline ValueSummary<bool> operator ==(const ValueSummary<T>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](T a, T b) { return a == b; });
    }

    inline ValueSummary<bool> operator !=(const ValueSummary<T>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](T a, T b) { return a != b; });
    }

    template<typename, bool, typename, typename, typename>
    friend struct BinaryOpFunctor;

    template<typename, bool, typename, typename>
    friend struct UnaryOpFunctor;
    
private:

    std::list<Guard<T>> values;
};

};
#endif