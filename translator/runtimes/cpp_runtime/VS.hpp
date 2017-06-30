#ifndef VS_HPP
#define VS_HPP

#include <iostream>
#include <functional>
#include <unordered_map>
#include <cassert>
#include <climits>

#include "ValueSummaryOperations.h"

namespace RUNTIME_NAMESPACE {

template<typename T>
class ValueSummary final {

public:

    ValueSummary():
        ValueSummary(T())
    { }

    ValueSummary(const T& i):values(std::unordered_map<T, Bdd>({{i, PathConstraint::pc()}}))
    { }

    ValueSummary(const ValueSummary<T>& other)
    {
        // TODO: better filter initialization
        for(const auto& gv : other.values) {
            Bdd pred = PathConstraint::pc() & gv.second;
            if (!pred.isZero()) {
                values.insert({gv.first, pred});
            }
        }
    }

    ValueSummary(const ValueSummary<T>&& other):values(other.values) { }

    ValueSummary(ValueSummary<T>&& other) = default;

    inline ValueSummary<T>& operator= (const ValueSummary<T>& rhs) {
        for(const auto& gvRhs : rhs.values) {
            Bdd pred = PathConstraint::pc() & gvRhs.second;
            if(!pred.isZero()) {
                bool found = false;
                for(auto gvLhs = begin(values); gvLhs != end(values); ) {
                    if(gvLhs->first == gvRhs.first) {
                        gvLhs->second |= pred;
                        found = true;
                        ++gvLhs;
                    } else {
                        gvLhs->second &= !pred;
                        if (gvLhs->second.isZero()) {
                            gvLhs = values.erase(gvLhs);
                        } else {
                            ++gvLhs;
                        }
                    }
                }
                if(!found) {
                    values.insert({gvRhs.first, pred});
                }
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

    template<typename P>
    friend ValueSummary<bool> operator ==(const ValueSummary<P>&, const P&);
    template<typename P>
    friend ValueSummary<bool> operator ==(const P&, const ValueSummary<P>&);

    template<typename P>
    friend ValueSummary<bool> operator !=(const ValueSummary<P>&, const P&);
    template<typename P>
    friend ValueSummary<bool> operator !=(const P&, const ValueSummary<P>&);

    template<typename P>
    friend std::ostream& operator<<(std::ostream&, const ValueSummary<P>&);

    template<typename, typename, typename, typename>
    friend struct BinaryOpFunctor;

    template<typename, typename, typename>
    friend struct UnaryOpFunctor;

    void printDot(const std::string& fname) const {
        for(const auto& gv : values) {
            RUNTIME_NAMESPACE::printDot(gv.second, fname + std::to_string(gv.first));
        }
    }

private:

    ValueSummary(const std::unordered_map<T, Bdd>& values):values(values) { }

    std::unordered_map<T, Bdd> values;

public:

    class Builder {
    public:
        void addValue(const Bdd& pred, const T& value) {
            values[value] |= pred;
        }

        void addValue(const Bdd& pred, const ValueSummary<T>& rhs) {
            for(const auto& v : rhs.values) {
                addValue(v.second, v.first);
            }
        }

        ValueSummary<T> build() {
            return ValueSummary<T>(values);
        }

        std::unordered_map<T, Bdd> values;
    };
};

template<typename P>
inline ValueSummary<bool> operator==(const ValueSummary<P>& a, const P& b) {
    return unaryOp<ValueSummary<bool>>(a, [=](const P& a) { return a == b; });
}

template<typename P>
inline ValueSummary<bool> operator==(const P& a, const ValueSummary<P>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](const P& b) { return a == b; });
}

template<typename P>
inline ValueSummary<bool> operator!=(const ValueSummary<P>& a, const P& b) {
    return unaryOp<ValueSummary<bool>>(a, [=](const P& a) { return a != b; });
}

template<typename P>
inline ValueSummary<bool> operator!=(const P& a, const ValueSummary<P>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](const P& b) { return a != b; });
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ValueSummary<T>& v)  
{  
    os << "VS[";
    int i = 0;
    for(const auto& gv : v.values) {
        if(i != 0) {
            os << ",";
        }
        os << gv.first;
        ++i;
    }
    os << "]";
    return os;  
}

};

#endif