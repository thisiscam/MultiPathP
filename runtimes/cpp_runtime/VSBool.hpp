#ifndef VS_BOOL_HPP
#define VS_BOOL_HPP

#include <iostream>

#include "Operations.h"

namespace RUNTIME_NAMESPACE {

template<>
class ValueSummary<bool> final {
    
private:
    struct unused { };
    ValueSummary(unused _) noexcept { }

public:
    Bdd T, F;

    static const ValueSummary<bool>& undefined() {
        static ValueSummary<bool> _undefined = ValueSummary<bool>(unused());
        return _undefined;
    }

    ValueSummary() noexcept:
        ValueSummary(false)
    { }

    ValueSummary(const ValueSummary<bool>& other) noexcept:
        T(other.T & PathConstraint::pc()),
        F(other.F & PathConstraint::pc()) 
    { }

    ValueSummary(ValueSummary<bool>&& other) = default;

    ValueSummary(const bool b) noexcept:
        T( b ? PathConstraint::pc() : Bdd::bddZero()),
        F(!b ? PathConstraint::pc() : Bdd::bddZero())
    { }

    inline ValueSummary<bool>& operator=(ValueSummary<bool> other) {
        T = (T | other.T) & !other.F;
        F = (F | other.F) & !other.T;
        return *this;
    }

    inline ValueSummary<bool> operator&(const ValueSummary<bool>& b) const {
        Bdd&& t = PathConstraint::pc() & T & b.T;
        Bdd&& f = PathConstraint::pc() - t;
        return ValueSummary<bool>(std::forward<Bdd>(t), std::forward<Bdd>(f));
    }

    inline ValueSummary<bool> operator|(const ValueSummary<bool>& b) const {
        Bdd&& f = PathConstraint::pc() & T & b.T;
        Bdd&& t = PathConstraint::pc() - f;
        return ValueSummary<bool>(std::forward<Bdd>(t), std::forward<Bdd>(f));
    }

    inline ValueSummary<bool> operator==(const ValueSummary<bool>& b) const {
        Bdd&& t = (PathConstraint::pc() & T & b.T) | (PathConstraint::pc() & F & b.F);
        Bdd&& f = PathConstraint::pc() - t;
        return ValueSummary<bool>(std::forward<Bdd>(t), std::forward<Bdd>(f));
    }

    inline ValueSummary<bool> operator!=(const ValueSummary<bool>& b) const {
        Bdd&& t = (PathConstraint::pc() & T & b.F) | (PathConstraint::pc() & T & b.F);
        Bdd&& f = PathConstraint::pc() - t;
        return ValueSummary<bool>(std::forward<Bdd>(t), std::forward<Bdd>(f));
    }

    ValueSummary<bool> operator!() const {
        return ValueSummary<bool>(PathConstraint::pc() & F, PathConstraint::pc() & T);
    }

    friend std::ostream& operator<<(std::ostream&, const ValueSummary<bool>&);

    void printDot(const std::string& fname) const {
        RUNTIME_NAMESPACE::printDot(T, fname + "T");
        RUNTIME_NAMESPACE::printDot(F, fname + "F");
    }

    class Builder {

    public: 
        inline Builder& addValue(const Bdd& pred, bool value) {
            if(value) {
                T |= pred;
            } else {
                F |= pred;
            }
            return *this;
        }

        inline Builder& addValue(const Bdd& pred, const ValueSummary<bool>& rhs) {
            return addValue(pred & rhs.T, true).addValue(pred & rhs.F, false);
        }

        inline ValueSummary<bool> build() {
            return ValueSummary<bool>(T, F);
        }

    private:
        Bdd T, F;
    };

private:
    ValueSummary(const Bdd& t, const Bdd& f):T(t),F(f) { }
};

inline ValueSummary<bool> operator&(const ValueSummary<bool>& a, const bool b) {
    if(b) {
        return a;
    } else {
        return false;
    }
}

inline ValueSummary<bool> operator&(const bool a, const ValueSummary<bool>& b) {
    if(a) {
        return b;
    } else {
        return false;
    }
}

inline ValueSummary<bool> operator|(const ValueSummary<bool>& a, const bool b) {
    if(b) {
        return true;
    } else {
        return a;
    }
}

inline ValueSummary<bool> operator|(const bool a, const ValueSummary<bool>& b) {
    if(a) {
        return true;
    } else {
        return b;
    }
}

inline ValueSummary<bool> operator==(const ValueSummary<bool>& a, const bool b) {
    if(b) {
        return (PathConstraint::pc() & a.F).isZero();
    } else {
        return (PathConstraint::pc() & a.T).isZero();
    }
}

inline ValueSummary<bool> operator==(const bool a, const ValueSummary<bool>& b) {
    if(a) {
        return (PathConstraint::pc() & b.F).isZero();
    } else {
        return (PathConstraint::pc() & b.T).isZero();
    }
}

inline ValueSummary<bool> operator!=(const ValueSummary<bool>& a, const bool b) {
    if(!b) {
        return (PathConstraint::pc() & a.F).isZero();
    } else {
        return (PathConstraint::pc() & a.T).isZero();
    }
}

inline ValueSummary<bool> operator!=(const bool a, const ValueSummary<bool>& b) {
    if(!a) {
        return (PathConstraint::pc() & b.F).isZero();
    } else {
        return (PathConstraint::pc() & b.T).isZero();
    }
}

std::ostream& operator<<(std::ostream& os, const ValueSummary<bool>& v)  
{  
    os << "VS[";
    if(!v.T.isZero()) {
        os << "T";
    } 
    if(!v.F.isZero()) {
        os << "F";
    }
    os << "]";
    return os;  
}


};

#endif