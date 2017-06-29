// #ifndef VS_PTR_HPP
// #define VS_PTR_HPP

// #include <iostream>
// #include <unordered_map>
// #include <cassert>
// #include <climits>

// #include "ValueSummaryOperations.h"

// namespace RUNTIME_NAMESPACE {

// template<typename P>
// class ValueSummary<P*> final {

// public:

//     ValueSummary():
//         ValueSummary(0)
//     { }

//     ValueSummary(P* i):values(std::unordered_map<P*, Bdd>({{i, PathConstraint::pc()}}))
//     { }

//     ValueSummary(const ValueSummary<P*>& other)
//     {
//         // TODO: better filter initialization
//         for(const auto& gv : other.values) {
//             Bdd pred = PathConstraint::pc() & gv.second;
//             if (!pred.isZero()) {
//                 values.insert({gv.first, pred});
//             }
//         }
//     }

//     ValueSummary(const ValueSummary<P*>&& other):values(other.values) { }

//     ValueSummary(ValueSummary<P*>&& other) = default;

//     inline ValueSummary<P*>& operator= (const ValueSummary<P*>& rhs) {
//         for(const auto& gvRhs : rhs.values) {
//             Bdd pred = PathConstraint::pc() & gvRhs.second;
//             if(!pred.isZero()) {
//                 bool found = false;
//                 for(auto gvLhs = begin(values); gvLhs != end(values); ) {
//                     if(gvLhs->first == gvRhs.first) {
//                         gvLhs->second |= pred;
//                         found = true;
//                         ++gvLhs;
//                     } else {
//                         gvLhs->second &= !pred;
//                         if (gvLhs->second.isZero()) {
//                             gvLhs = values.erase(gvLhs);
//                         } else {
//                             ++gvLhs;
//                         }
//                     }
//                 }
//                 if(!found) {
//                     values.insert({gvRhs.first, pred});
//                 }
//             }
//         }
//         return *this;
//     }

//     inline ValueSummary<bool> operator <(const ValueSummary<P*>& b) const {
//         return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a < b; });
//     }

//     inline ValueSummary<bool> operator <=(const ValueSummary<P*>& b) const {
//         return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a <= b; });
//     }

//     inline ValueSummary<bool> operator >(const ValueSummary<P*>& b) const {
//         return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a > b; });
//     }

//     inline ValueSummary<bool> operator >=(const ValueSummary<P*>& b) const {
//         return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a >= b; });
//     }

//     inline ValueSummary<bool> operator ==(const ValueSummary<P*>& b) const {
//         return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a == b; });
//     }

//     inline ValueSummary<bool> operator !=(const ValueSummary<P*>& b) const {
//         return binaryOp<ValueSummary<bool>>(*this, b, [](P* a, P* b) { return a != b; });
//     }

//     inline ValueSummary<P*> operator -() {
//         return unaryOp<ValueSummary<P*>>(*this, [](P* a) { return -a; });
//     }

//     inline ValueSummary<P*>& operator ++() {
//         return *this = *this + 1;
//     }

//     inline ValueSummary<P*>& operator --() {
//         return *this = *this - 1;
//     }

//     template<typename T>
//     friend ValueSummary<bool> operator >(const ValueSummary<T*>&, T*);
//     template<typename T>
//     friend ValueSummary<bool> operator >(T*, const ValueSummary<T*>&);

//     template<typename T>
//     friend ValueSummary<bool> operator >=(const ValueSummary<T*>&, T*);
//     template<typename T>
//     friend ValueSummary<bool> operator >=(T*, const ValueSummary<T*>&);

//     template<typename T>
//     friend ValueSummary<bool> operator <(const ValueSummary<T*>&, T*);
//     template<typename T>
//     friend ValueSummary<bool> operator <(T*, const ValueSummary<T*>&);

//     template<typename T>
//     friend ValueSummary<bool> operator <=(const ValueSummary<T*>&, T*);
//     template<typename T>
//     friend ValueSummary<bool> operator <=(T*, const ValueSummary<T*>&);

//     template<typename T>
//     friend ValueSummary<bool> operator ==(const ValueSummary<T*>&, T*);
//     template<typename T>
//     friend ValueSummary<bool> operator ==(T*, const ValueSummary<T*>&);

//     template<typename T>
//     friend ValueSummary<bool> operator !=(const ValueSummary<T*>&, T*);
//     template<typename T>
//     friend ValueSummary<bool> operator !=(T*, const ValueSummary<T*>&);

//     template<typename T>
//     friend std::ostream& operator<<(std::ostream&, const ValueSummary<T*>&);

//     template<typename, bool, typename, typename, typename>
//     friend struct BinaryOpFunctor;

//     template<typename, bool, typename, typename>
//     friend struct UnaryOpFunctor;

//     void printDot(const std::string& fname) const {
//         for(const auto& gv : values) {
//             RUNTIME_NAMESPACE::printDot(gv.second, fname + std::to_string((long)gv.first));
//         }
//     }

// private:

//     ValueSummary(const std::unordered_map<P*, Bdd>& values):values(values) { }

//     std::unordered_map<P*, Bdd> values;

// public:

//     class Builder {
//     public:
//         void addValue(const Bdd& pred, P* value) {
//             values[value] |= pred;
//         }

//         void addValue(const Bdd& pred, const ValueSummary<P*>& values) {
//             for(const auto& v : values.values) {
//                 addValue(v.second, v.first);
//             }
//         }

//         ValueSummary<P*> build() {
//             return ValueSummary<P*>(values);
//         }

//         std::unordered_map<P*, Bdd> values;
//     };
// };

// template<typename P>
// inline ValueSummary<bool> operator>(const ValueSummary<P*>& a, P* b) {
//     return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a > b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator>(P* a, const ValueSummary<P*>& b) {
//     return unaryOp<ValueSummary<bool>>(b, [=](P* b) { return a > b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator>=(const ValueSummary<P*>& a, P* b) {
//     return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a >= b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator>=(P* a, const ValueSummary<P*>& b) {
//     return unaryOp<ValueSummary<bool>>(b, [=](P* b) { return a >= b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator<(const ValueSummary<P*>& a, P* b) {
//     return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a < b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator<(P* a, const ValueSummary<P*>& b) {
//     return unaryOp<ValueSummary<bool>>(b, [=](P* b) { return a < b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator<=(const ValueSummary<P*>& a, P* b) {
//     return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a <= b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator<=(P* a, const ValueSummary<P*>& b) {
//     return unaryOp<ValueSummary<bool>>(b, [=](P* b) { return a <= b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator==(const ValueSummary<P*>& a, P* b) {
//     return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a == b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator==(P* a, const ValueSummary<P*>& b) {
//     return unaryOp<ValueSummary<bool>>(b, [=](P* b) { return a == b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator!=(const ValueSummary<P*>& a, P* b) {
//     return unaryOp<ValueSummary<bool>>(a, [=](P* a) { return a != b; });
// }

// template<typename P>
// inline ValueSummary<bool> operator!=(P* a, const ValueSummary<P*>& b) {
//     return unaryOp<ValueSummary<bool>>(b, [=](P* b) { return a != b; });
// }

// template<typename P>
// std::ostream& operator<<(std::ostream& os, const ValueSummary<P*>& v)  
// {  
//     os << "VS[";
//     int i = 0;
//     for(const auto& gv : v.values) {
//         if(i != 0) {
//             os << ",";
//         }
//         os << gv.first;
//         ++i;
//     }
//     os << "]";
//     return os;  
// }

// };

// #endif