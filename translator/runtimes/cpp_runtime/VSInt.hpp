#ifndef VS_INT_HPP
#define VS_INT_HPP

#include <iostream>
#include <unordered_map>
#include <cassert>

namespace RUNTIME_NAMESPACE {

template<>
class ValueSummary<int> final {

public:

    ValueSummary():
        ValueSummary(0)
    { }

    ValueSummary(int i):values(std::unordered_map<int, Bdd>({{i, PathConstraint::pc()}}))
    { }

    ValueSummary(const ValueSummary<int>& other)
    {
        // TODO: better filter initialization
        for(const auto& gv : other.values) {
            Bdd pred = PathConstraint::pc() & gv.second;
            if (!pred.isZero()) {
                values.insert({gv.first, pred});
            }
        }
    }

    ValueSummary(const ValueSummary<int>&& other):values(other.values) { }

    ValueSummary(ValueSummary<int>&& other) = default;

    inline ValueSummary<int>& operator= (const ValueSummary<int>& rhs) {
        for(const auto& gvRhs : rhs.values) {
            Bdd pred = PathConstraint::pc() & gvRhs.second;
            if(!pred.isZero()) {
                bool found = false;
                for(auto gvLhs = begin(values); gvLhs != end(values); ) {
                    if(gvLhs->first == gvRhs.first) {
                        gvLhs->second |= pred;
                        found = true;
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

    inline ValueSummary<int> operator +(const ValueSummary<int>& b) const {
        return binaryOp<int>(*this, b, [](int a, int b) { return a + b; });
    }

    inline ValueSummary<int> operator -(const ValueSummary<int>& b) const {
        return binaryOp<int>(*this, b, [](int a, int b) { return a - b; });
    }

    inline ValueSummary<int> operator *(const ValueSummary<int>& b) const {
        return binaryOp<int>(*this, b, [](int a, int b) { return a * b; });
    }

    inline ValueSummary<int> operator /(const ValueSummary<int>& b) const {
        return binaryOp<int>(*this, b, [](int a, int b) { return a / b; });
    }

    inline ValueSummary<bool> operator <(const ValueSummary<int>& b) const {
        return binaryOp<bool>(*this, b, [](int a, int b) { return a < b; });
    }

    inline ValueSummary<bool> operator <=(const ValueSummary<int>& b) const {
        return binaryOp<bool>(*this, b, [](int a, int b) { return a <= b; });
    }

    inline ValueSummary<bool> operator >(const ValueSummary<int>& b) const {
        return binaryOp<bool>(*this, b, [](int a, int b) { return a > b; });
    }

    inline ValueSummary<bool> operator >=(const ValueSummary<int>& b) const {
        return binaryOp<bool>(*this, b, [](int a, int b) { return a >= b; });
    }

    inline ValueSummary<bool> operator ==(const ValueSummary<int>& b) const {
        return binaryOp<bool>(*this, b, [](int a, int b) { return a == b; });
    }

    inline ValueSummary<bool> operator !=(const ValueSummary<int>& b) const {
        return binaryOp<bool>(*this, b, [](int a, int b) { return a != b; });
    }

    inline ValueSummary<int> operator -() {
        return unaryOp<int>(*this, [](int a) { return -a; });
    }

    inline ValueSummary<int>& operator ++() {
        return *this = *this + 1;
    }

    inline ValueSummary<int>& operator --() {
        return *this = *this - 1;
    }

    friend ValueSummary<int> operator +(const ValueSummary<int>&, int);
    friend ValueSummary<int> operator +(int, const ValueSummary<int>&);

    friend ValueSummary<int> operator -(const ValueSummary<int>&, int);
    friend ValueSummary<int> operator -(int, const ValueSummary<int>&);

    friend ValueSummary<int> operator *(const ValueSummary<int>&, int);
    friend ValueSummary<int> operator *(int, const ValueSummary<int>&);

    friend ValueSummary<int> operator /(const ValueSummary<int>&, int);
    friend ValueSummary<int> operator /(int, const ValueSummary<int>&);

    friend ValueSummary<int> operator %(const ValueSummary<int>&, int);
    friend ValueSummary<int> operator %(int, const ValueSummary<int>&);

    friend ValueSummary<bool> operator >(const ValueSummary<int>&, int);
    friend ValueSummary<bool> operator >(int, const ValueSummary<int>&);

    friend ValueSummary<bool> operator >=(const ValueSummary<int>&, int);
    friend ValueSummary<bool> operator >=(int, const ValueSummary<int>&);

    friend ValueSummary<bool> operator <(const ValueSummary<int>&, int);
    friend ValueSummary<bool> operator <(int, const ValueSummary<int>&);

    friend ValueSummary<bool> operator <=(const ValueSummary<int>&, int);
    friend ValueSummary<bool> operator <=(int, const ValueSummary<int>&);

    friend ValueSummary<bool> operator ==(const ValueSummary<int>&, int);
    friend ValueSummary<bool> operator ==(int, const ValueSummary<int>&);

    friend ValueSummary<bool> operator !=(const ValueSummary<int>&, int);
    friend ValueSummary<bool> operator !=(int, const ValueSummary<int>&);

    friend std::ostream& operator<<(std::ostream&, const ValueSummary<int>&);

    template<typename ReturnType, typename A, typename B, typename BinOp>
    friend ValueSummary<ReturnType>
    binaryOp(const ValueSummary<A>& a, const ValueSummary<B>& b, BinOp&& binOp);

    template<typename ReturnType, typename A, typename UnaryOp>
    friend ValueSummary<ReturnType> 
    unaryOp(const ValueSummary<A>& a, UnaryOp&& uOp);

private:

    ValueSummary(const std::unordered_map<int, Bdd>& values):values(values) { }

    std::unordered_map<int, Bdd> values;

public:
    class Builder {
    public:
        void addValue(const Bdd& pred, int value) {
            values[value] |= pred;
        }

        ValueSummary<int> build() {
            return ValueSummary<int>(values);
        }

        std::unordered_map<int, Bdd> values;
    };
};

inline ValueSummary<int> operator+(const ValueSummary<int>& a, int b) {
    return unaryOp<int>(a, [=](int a) { return a + b; });
}

inline ValueSummary<int> operator+(int b, const ValueSummary<int>& a) {
    return unaryOp<int>(a, [=](int a) { return a + b; });
}

inline ValueSummary<int> operator-(const ValueSummary<int>& a, int b) {
    return unaryOp<int>(a, [=](int a) { return a - b; });
}

inline ValueSummary<int> operator-(int b, const ValueSummary<int>& a) {
    return unaryOp<int>(a, [=](int a) { return a - b; });
}

inline ValueSummary<int> operator*(const ValueSummary<int>& a, int b) {
    return unaryOp<int>(a, [=](int a) { return a * b; });
}

inline ValueSummary<int> operator*(int b, const ValueSummary<int>& a) {
    return unaryOp<int>(a, [=](int a) { return a * b; });
}

inline ValueSummary<int> operator/(const ValueSummary<int>& a, int b) {
    return unaryOp<int>(a, [=](int a) { return a / b; });
}

inline ValueSummary<int> operator/(int b, const ValueSummary<int>& a) {
    return unaryOp<int>(a, [=](int a) { return a / b; });
}

inline ValueSummary<int> operator%(const ValueSummary<int>& a, int b) {
    return unaryOp<int>(a, [=](int a) { return a % b; });
}

inline ValueSummary<int> operator%(int b, const ValueSummary<int>& a) {
    return unaryOp<int>(a, [=](int a) { return a % b; });
}

inline ValueSummary<bool> operator>(const ValueSummary<int>& a, int b) {
    return unaryOp<bool>(a, [=](int a) { return a > b; });
}

inline ValueSummary<bool> operator>(int b, const ValueSummary<int>& a) {
    return unaryOp<bool>(a, [=](int a) { return a > b; });
}

inline ValueSummary<bool> operator>=(const ValueSummary<int>& a, int b) {
    return unaryOp<bool>(a, [=](int a) { return a >= b; });
}

inline ValueSummary<bool> operator>=(int b, const ValueSummary<int>& a) {
    return unaryOp<bool>(a, [=](int a) { return a >= b; });
}

inline ValueSummary<bool> operator<(const ValueSummary<int>& a, int b) {
    return unaryOp<bool>(a, [=](int a) { return a < b; });
}

inline ValueSummary<bool> operator<(int b, const ValueSummary<int>& a) {
    return unaryOp<bool>(a, [=](int a) { return a < b; });
}

inline ValueSummary<bool> operator<=(const ValueSummary<int>& a, int b) {
    return unaryOp<bool>(a, [=](int a) { return a <= b; });
}

inline ValueSummary<bool> operator<=(int b, const ValueSummary<int>& a) {
    return unaryOp<bool>(a, [=](int a) { return a <= b; });
}

inline ValueSummary<bool> operator==(const ValueSummary<int>& a, int b) {
    return unaryOp<bool>(a, [=](int a) { return a == b; });
}

inline ValueSummary<bool> operator==(int b, const ValueSummary<int>& a) {
    return unaryOp<bool>(a, [=](int a) { return a == b; });
}

inline ValueSummary<bool> operator!=(const ValueSummary<int>& a, int b) {
    return unaryOp<bool>(a, [=](int a) { return a != b; });
}

inline ValueSummary<bool> operator!=(int b, const ValueSummary<int>& a) {
    return unaryOp<bool>(a, [=](int a) { return a != b; });
}

std::ostream& operator<<(std::ostream& os, const ValueSummary<int>& v)  
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