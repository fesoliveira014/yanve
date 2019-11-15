#pragma once

// TODO: bool operator is prone to go bad, beware!
// TODO: this should be an enumset instead, not an enum since
// we break the way it behaves when an | operator is used, i.e.,
// results should NEVER be stored and always try to define a 0
// TODO: ! operator is a workaround not having a full bool operator
#define ENUM_OPERATORS(class, flagtype)                                                 \
    inline bool operator==(class a, class b) {                                          \
        return static_cast<flagtype>(a) == static_cast<flagtype>(b);                    \
    }                                                                                   \
    inline bool operator!=(class a, class b) {                                          \
        return static_cast<flagtype>(a) != static_cast<flagtype>(b);                    \
    }                                                                                   \
    inline bool operator>=(class a, class b) {                                          \
        return static_cast<flagtype>(a) >= static_cast<flagtype>(b);                    \
    }                                                                                   \
    inline bool operator<=(class a, class b) {                                          \
        return static_cast<flagtype>(a) <= static_cast<flagtype>(b);                    \
    }                                                                                   \
    inline class operator|(class a, class b) {                                          \
        return static_cast<class>(static_cast<flagtype>(b) | static_cast<flagtype>(a)); \
    }                                                                                   \
    inline class operator&(class a, class b) {                                          \
        return static_cast<class>(static_cast<flagtype>(b) & static_cast<flagtype>(a)); \
    }                                                                                   \
    inline class operator^(class a, class b) {                                          \
        return static_cast<class>(static_cast<flagtype>(b) ^ static_cast<flagtype>(a)); \
    }                                                                                   \
    inline class operator~(class a) {                                                   \
        return static_cast<class>(~static_cast<flagtype>(a));                           \
    }                                                                                   \
    inline bool operator!(class a) {                                                    \
        return static_cast<flagtype>(a) == (flagtype)0;                                 \
    }                                                                                   \
    inline class& operator|=(class& a, class b) { return a = a | b; }                   \
    inline class& operator&=(class& a, class b) { return a = a & b; }                   \
    inline class& operator^=(class& a, class b) { return a = a ^ b; }
