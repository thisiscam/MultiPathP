#ifndef VS_BOOL_HPP
#define VS_BOOL_HPP

namespace RUNTIME_NAMESPACE {

template<>
class ValueSummary<bool> final {
    
public:
    Bdd T, F;
    ValueSummary():
        T(Bdd::bddZero()), 
        F(PathConstraint::pc()) 
    { }

    ValueSummary(const ValueSummary<bool>& other):
        T(other.T & PathConstraint::pc()),
        F(other.F & PathConstraint::pc()) 
    { }

    ValueSummary(ValueSummary<bool>&& other) = default;

    ValueSummary(bool b):
        T( b ? PathConstraint::pc() : Bdd::bddZero()),
        F(!b ? PathConstraint::pc() : Bdd::bddZero())
    { }

    inline ValueSummary<bool>& operator=(const ValueSummary<bool>& other) { 
        T |= other.T;
        F |= other.F;
        return *this;
    }

    inline ValueSummary<bool> operator&(const ValueSummary<bool>& b) const {
        auto&& t = PathConstraint::pc() & T & b.T;
        auto&& f = PathConstraint::pc() - t;
        return ValueSummary<bool>(t, f);
    }

    inline ValueSummary<bool> operator|(const ValueSummary<bool>& b) const {
        auto&& f = PathConstraint::pc() & T & b.T;
        auto&& t = PathConstraint::pc() - f;
        return ValueSummary<bool>(t, f);
    }

    inline ValueSummary<bool> operator==(const ValueSummary<bool>& b) const {
        auto&& t = (PathConstraint::pc() & T & b.T) | (PathConstraint::pc() & F & b.F);
        auto&& f = PathConstraint::pc() - t;
        return ValueSummary<bool>(t, f);
    }

    inline ValueSummary<bool> operator!=(const ValueSummary<bool>& b) const {
        auto&& t = (PathConstraint::pc() & T & b.F) | (PathConstraint::pc() & T & b.F);
        auto&& f = PathConstraint::pc() - t;
        return ValueSummary<bool>(t, f);
    }

    ValueSummary<bool> operator!() const {
        return ValueSummary<bool>(PathConstraint::pc() & F, PathConstraint::pc() & T);
    }

    friend std::ostream& operator<<(std::ostream&, const ValueSummary<bool>&);

    class Builder {

    public: 
        void addValue(const Bdd& pred, bool value) {
            if(value) {
                T |= pred;
            } else {
                F |= pred;
            }
        }

        ValueSummary<bool> build() {
            return ValueSummary<bool>(T, F);
        }

    private:
        Bdd T, F;
    };

private:
    ValueSummary(const Bdd& t, const Bdd& f):T(t),F(f) { }
};


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