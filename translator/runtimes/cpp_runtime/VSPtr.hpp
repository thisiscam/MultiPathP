#ifndef VS_PTR_HPP
#define VS_PTR_HPP

#include <iostream>
#include <list>
#include <cassert>

#include "ValueSummaryOperations.h"

namespace RUNTIME_NAMESPACE {

template<typename P>
class ValueSummary<P*> final {

public:

    ValueSummary():
        ValueSummary(NULL)
    { }

    ValueSummary(P* v):values(std::list<Guard<P*>>(1, {PathConstraint::pc(), v}))
    { }

    ValueSummary(const ValueSummary<P*>& other)
    {
        // TODO: better filter initialization
        for(const auto& gv : other.values) {
            Bdd pred = PathConstraint::pc() & gv.second;
            if (!pred.isZero()) {
                values.push_back({pred, gv.first});
            }
        }
    }

    ValueSummary(const ValueSummary<P*>&& other):values(other.values) { }

    ValueSummary(ValueSummary<P*>&& other) = default;

    inline ValueSummary<P*>& operator= (const ValueSummary<P*>& rhs) {
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

    inline ValueSummary<bool> operator <(const ValueSummary<P*>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a < b; });
    }

    inline ValueSummary<bool> operator <=(const ValueSummary<P*>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a <= b; });
    }

    inline ValueSummary<bool> operator >(const ValueSummary<P*>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a > b; });
    }

    inline ValueSummary<bool> operator >=(const ValueSummary<P*>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a >= b; });
    }

    inline ValueSummary<bool> operator ==(const ValueSummary<P*>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a == b; });
    }

    inline ValueSummary<bool> operator !=(const ValueSummary<P*>& b) const {
        return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a != b; });
    }

    inline ValueSummary<P> operator *() const {
        return unaryOp<P>(*this, [](P* a) { return *a; });
    }

    friend ValueSummary<bool> operator >(const ValueSummary<P*>&, P*);
    friend ValueSummary<bool> operator >(int, const ValueSummary<P*>&);

    friend ValueSummary<bool> operator >=(const ValueSummary<P*>&, P*);
    friend ValueSummary<bool> operator >=(int, const ValueSummary<P*>&);

    friend ValueSummary<bool> operator <(const ValueSummary<P*>&, P*);
    friend ValueSummary<bool> operator <(int, const ValueSummary<P*>&);

    friend ValueSummary<bool> operator <=(const ValueSummary<P*>&, P*);
    friend ValueSummary<bool> operator <=(int, const ValueSummary<P*>&);

    friend ValueSummary<bool> operator ==(const ValueSummary<P*>&, P*);
    friend ValueSummary<bool> operator ==(int, const ValueSummary<P*>&);

    friend ValueSummary<bool> operator !=(const ValueSummary<P*>&, P*);
    friend ValueSummary<bool> operator !=(int, const ValueSummary<P*>&);

    template<typename, bool, typename, typename, typename>
    friend struct BinaryOpFunctor;

    template<typename, bool, typename, typename>
    friend struct UnaryOpFunctor;

    friend std::ostream& operator<<(std::ostream&, const ValueSummary<P*>&);

private:

    ValueSummary(const std::list<Guard<P*>>& values):values(values) { }

    std::list<Guard<P*>> values;

public:
    class Builder {
    public:
        void addValue(const Bdd& pred, P* value) {
            values.push_back({pred, value});
        }

        void addValue(const Bdd& pred, const ValueSummary<P*>& values) {
            for(const auto& v : values.values) {
                addValue(v.second, v.first);
            }
        }

        ValueSummary<P*> build() {
            return ValueSummary<P*>(values);
        }

        std::list<Guard<P*>> values;
    };
};

template<typename P>
inline ValueSummary<bool> operator>(const ValueSummary<P*>& a, P* b) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a > b; });
}

template<typename P>
inline ValueSummary<bool> operator>(P* b, const ValueSummary<P*>& a) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a > b; });
}

template<typename P>
inline ValueSummary<bool> operator>=(const ValueSummary<P*>& a, P* b) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a >= b; });
}

template<typename P>
inline ValueSummary<bool> operator>=(P* b, const ValueSummary<P*>& a) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a >= b; });
}

template<typename P>
inline ValueSummary<bool> operator<(const ValueSummary<P*>& a, P* b) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a < b; });
}

template<typename P>
inline ValueSummary<bool> operator<(P* b, const ValueSummary<P*>& a) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a < b; });
}

template<typename P>
inline ValueSummary<bool> operator<=(const ValueSummary<P*>& a, P* b) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a <= b; });
}

template<typename P>
inline ValueSummary<bool> operator<=(P* b, const ValueSummary<P*>& a) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a <= b; });
}

template<typename P>
inline ValueSummary<bool> operator==(const ValueSummary<P*>& a, P* b) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a == b; });
}

template<typename P>
inline ValueSummary<bool> operator==(P* b, const ValueSummary<P*>& a) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a == b; });
}

template<typename P>
inline ValueSummary<bool> operator!=(const ValueSummary<P*>& a, P* b) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a != b; });
}

template<typename P>
inline ValueSummary<bool> operator!=(P* b, const ValueSummary<P*>& a) {
    return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a != b; });
}

template<typename P>
std::ostream& operator<<(std::ostream& os, const ValueSummary<P*>& v)  
{  
    os << "VS[";
    int i = 0;
    for(const auto& gv : v.values) {
        if(i != 0) {
            os << ",";
        }
        os << gv.value;
        ++i;
    }
    os << "]";
    return os;  
}

};

#endif