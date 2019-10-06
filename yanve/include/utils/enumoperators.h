#pragma once

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
    inline class& operator|=(class& a, class b) { return a = a | b; }                   \
    inline class& operator&=(class& a, class b) { return a = a & b; }                   \
    inline class& operator^=(class& a, class b) { return a = a ^ b; }
