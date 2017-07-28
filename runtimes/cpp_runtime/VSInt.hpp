#ifndef VS_INT_HPP
#define VS_INT_HPP

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <climits>
#include <stdexcept>

#include "Operations.h"

namespace RUNTIME_NAMESPACE {

template<>
class ValueSummary<int> final {
    
    friend class ExecutionEngine;
    
    template<typename>
    friend class DefaultArray;

private:
    struct unused { };
    ValueSummary(unused _) noexcept { }

public:
    static const ValueSummary<int>& undefined() {
        static ValueSummary<int> _undefined = ValueSummary<int>(unused());
        return _undefined;
    }

    ValueSummary() noexcept:
        ValueSummary(0) 
    { }

    ValueSummary(int i) noexcept:
        values(std::unordered_map<int, Bdd>({{i, PathConstraint::pc()}}))
    { }

    ValueSummary(const ValueSummary<int>& other) noexcept
    {
        // TODO: better filter initialization
        for(const auto& gv : other.values) {
            Bdd&& pred = PathConstraint::pc() & gv.second;
            if (!pred.isZero()) {
                values.insert({gv.first, pred});
            }
        }
    }

    ValueSummary(const ValueSummary<int>&& other) noexcept :values(other.values) { }

    ValueSummary(ValueSummary<int>&& other) = default;

    inline ValueSummary<int>& operator= (const ValueSummary<int>& rhs) {
        if(values.size() > 5) {
            std::unordered_map<int, std::pair<Bdd, Bdd>> tmpMap;
            std::vector<int> tmpOrder;
            Bdd before = Bdd::bddOne();
            for(const auto& gvRhs : rhs.values) {
                Bdd pred = gvRhs.second & PathConstraint::pc();
                if(!pred.isZero()) {
                    tmpMap.insert({gvRhs.first, {pred, before}});
                    tmpOrder.push_back(gvRhs.first);
                    before &= !pred;
                }
            }
            Bdd after = Bdd::bddOne();
            for(auto i = tmpOrder.rbegin(); i != tmpOrder.rend(); ++i) {
                auto& entry = tmpMap.at(*i);
                entry.second &= after;
                after &= !entry.first;
            }
            for(auto gvLhs = begin(values); gvLhs != end(values); /**/) {
                const auto& entry = tmpMap.find(gvLhs->first);
                if(entry != tmpMap.end()) {
                    tmpMap.erase(entry);
                    gvLhs->second = (gvLhs->second & entry->second.second) | entry->second.first;
                    ++gvLhs;
                } else {
                    gvLhs->second &= before;
                    if(gvLhs->second.isZero()) {
                        gvLhs = values.erase(gvLhs);
                    } else {
                        ++gvLhs;
                    }
                }
            }
            for(const auto& gvTmp : tmpMap) {
                values.insert({gvTmp.first, gvTmp.second.first});
            }
        } else {
            std::unordered_map<int, Bdd> tmpRhsMap;
            for(const auto& gvRhs : rhs.values) {
                Bdd&& pred = PathConstraint::pc() & gvRhs.second;
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
                        tmpRhsMap.insert({gvRhs.first, pred});
                    }
                }
            }
            values.insert(tmpRhsMap.begin(), tmpRhsMap.end());
        }
        return *this;
    }

    inline ValueSummary<int> operator +(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<int>>(*this, b, [](int a, int b) { return a + b; });
    }

    inline ValueSummary<int> operator -(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<int>>(*this, b, [](int a, int b) { return a - b; });
    }

    inline ValueSummary<int> operator *(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<int>>(*this, b, [](int a, int b) { return a * b; });
    }

    inline ValueSummary<int> operator /(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<int>>(*this, b, [](int a, int b) { return a / b; });
    }

    inline ValueSummary<int> operator %(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<int>>(*this, b, [](int a, int b) { return a % b; });
    }

    inline ValueSummary<bool> operator <(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](int a, int b) { return a < b; });
    }

    inline ValueSummary<bool> operator <=(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](int a, int b) { return a <= b; });
    }

    inline ValueSummary<bool> operator >(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](int a, int b) { return a > b; });
    }

    inline ValueSummary<bool> operator >=(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](int a, int b) { return a >= b; });
    }

    inline ValueSummary<bool> operator ==(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](int a, int b) { return a == b; });
    }

    inline ValueSummary<bool> operator !=(const ValueSummary<int>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](int a, int b) { return a != b; });
    }

    inline ValueSummary<int> operator -() {
        return unaryOp<ValueSummary<int>>(*this, [](int a) { return -a; });
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

    template<typename, typename, typename, typename>
    friend struct BinaryOpFunctor;

    template<typename, typename, typename>
    friend struct UnaryOpFunctor;

    void printDot(const std::string& fname) const {
        for(const auto& gv : values) {
            RUNTIME_NAMESPACE::printDot(gv.second, fname + std::to_string(gv.first));
        }
    }

    void checkDisjoint() const {
        for (auto i = values.begin(); i != values.end(); ++i) {
            for (auto j = i; ++j != values.end(); /**/) {
                if(!(i->second & j->second).isZero()) {
                    throw std::runtime_error("failed");
                }
            }
        }
    }

private:

    ValueSummary(std::unordered_map<int, Bdd>&& values):values(std::move(values)) { }

    std::unordered_map<int, Bdd> values;

public:

    inline int maxValue() const {
        int m = INT_MIN;
        for(auto && gv : values) {
            m = std::max(gv.first, m);
        }
        return m;
    }

    class Builder {
    public:
        inline void addValue(const Bdd& pred, int value) {
            values[value] |= pred;
        }

        inline void addValue(const Bdd& pred, const ValueSummary<int>& rhs) {
          for(const auto& v : rhs.values) {
                Bdd&& constraintPred = pred & v.second;
                if(!constraintPred.isZero()) {
                    addValue(constraintPred, v.first);
                }
            }   
        }

        ValueSummary<int> build() {
            return ValueSummary<int>(std::move(values));
        }

        std::unordered_map<int, Bdd> values;
    };
};

inline ValueSummary<int> operator+(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<int>>(a, [=](int a) { return a + b; });
}

inline ValueSummary<int> operator+(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<int>>(b, [=](int b) { return a + b; });
}

inline ValueSummary<int> operator-(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<int>>(a, [=](int a) { return a - b; });
}

inline ValueSummary<int> operator-(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<int>>(b, [=](int b) { return a - b; });
}

inline ValueSummary<int> operator*(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<int>>(a, [=](int a) { return a * b; });
}

inline ValueSummary<int> operator*(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<int>>(b, [=](int b) { return a * b; });
}

inline ValueSummary<int> operator/(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<int>>(a, [=](int a) { return a / b; });
}

inline ValueSummary<int> operator/(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<int>>(b, [=](int b) { return a / b; });
}

inline ValueSummary<int> operator%(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<int>>(a, [=](int a) { return a % b; });
}

inline ValueSummary<int> operator%(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<int>>(b, [=](int b) { return a % b; });
}

inline ValueSummary<bool> operator>(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<bool>>(a, [=](int a) { return a > b; });
}

inline ValueSummary<bool> operator>(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](int b) { return a > b; });
}

inline ValueSummary<bool> operator>=(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<bool>>(a, [=](int a) { return a >= b; });
}

inline ValueSummary<bool> operator>=(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](int b) { return a >= b; });
}

inline ValueSummary<bool> operator<(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<bool>>(a, [=](int a) { return a < b; });
}

inline ValueSummary<bool> operator<(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](int b) { return a < b; });
}

inline ValueSummary<bool> operator<=(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<bool>>(a, [=](int a) { return a <= b; });
}

inline ValueSummary<bool> operator<=(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](int b) { return a <= b; });
}

inline ValueSummary<bool> operator==(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<bool>>(a, [=](int a) { return a == b; });
}

inline ValueSummary<bool> operator==(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](int b) { return a == b; });
}

inline ValueSummary<bool> operator!=(const ValueSummary<int>& a, int b) {
    return unaryOp<ValueSummary<bool>>(a, [=](int a) { return a != b; });
}

inline ValueSummary<bool> operator!=(int a, const ValueSummary<int>& b) {
    return unaryOp<ValueSummary<bool>>(b, [=](int b) { return a != b; });
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