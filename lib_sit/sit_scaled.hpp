#include <cstdint>
#include <type_traits>
#include <ratio>

// using atto  = std::ratio<1, 1000000000000000000LL>;
// using femto = std::ratio<1, 1000000000000000LL>;
using pico  = std::ratio<1, 1000000000000LL>;
using nano  = std::ratio<1, 1000000000>;
using micro = std::ratio<1, 1000000>;
using milli = std::ratio<1, 1000>;
using centi = std::ratio<1, 100>;
using deci  = std::ratio<1, 10>;
using deca  = std::ratio<10, 1>;
using hecto = std::ratio<100, 1>;
using kilo  = std::ratio<1000, 1>;
using mega  = std::ratio<1000000, 1>;
using giga  = std::ratio<1000000000, 1>;
using tera  = std::ratio<1000000000000LL, 1>;
// using peta  = std::ratio<1000000000000000LL, 1>;
// using exa   = std::ratio<1000000000000000000LL, 1>;

template<std::ratio<1000, 1> scale, int m_exp, int kg_exp, int s_exp>
// template<typename T, T scale, int m_exp, int kg_exp, int s_exp>
struct myUnit2 {
    double repr = 1;
    // default=1 so that if somebody default constructs
    // don't stays surprised because of zero-ing out everything else.

    // +
    myUnit2<scale, m_exp, kg_exp, s_exp> operator+(myUnit2<scale, m_exp, kg_exp, s_exp> rhs) {
        return myUnit2<m_exp, kg_exp, s_exp>{ repr + rhs.repr };
    }

    // -
    myUnit2<scale, m_exp, kg_exp, s_exp> operator-(myUnit2<scale, m_exp, kg_exp, s_exp> rhs) {
        return myUnit2<m_exp, kg_exp, s_exp>{ repr - rhs.repr };
    }

    // *
    template<std::ratio scale2, int m2_exp, int s2_exp, int kg2_exp>
    myUnit2<scale * scale2, m_exp + m2_exp, kg_exp + s2_exp, s_exp + kg2_exp> 
    operator*(
        myUnit2<scale2, m2_exp, s2_exp, kg2_exp> rhs
    ) {
        return myUnit2<
            scale * scale2, m_exp + m2_exp, kg_exp + s2_exp, s_exp + kg2_exp
        >{ repr * rhs.repr };
    }

    // /
    template<std::ratio scale2, int m2_exp, int s2_exp, int kg2_exp>
    myUnit2<scale / scale2, m_exp - m2_exp, kg_exp - s2_exp, s_exp - kg2_exp> 
    operator/(
        myUnit2<scale2, m2_exp, s2_exp, kg2_exp> rhs
    ) {
        return myUnit2<
            scale / scale2, m_exp - m2_exp, kg_exp - s2_exp, s_exp - kg2_exp
        >{ repr / rhs.repr };
    }

    inline constexpr bool operator==(const myUnit2<scale, m_exp, kg_exp, s_exp> rhs) const {
        return repr == rhs.repr;
    }

    inline constexpr bool operator<(const myUnit2<scale, m_exp, kg_exp, s_exp> rhs) const {
        return repr < rhs.repr;
    }

    inline constexpr bool operator>(const myUnit2<scale, m_exp, kg_exp, s_exp> rhs) const {
        return repr > rhs.repr;
    }

    inline constexpr bool operator<=(const myUnit2<scale, m_exp, kg_exp, s_exp> rhs) const {
        return repr <= rhs.repr;
    }

    inline constexpr bool operator>=(const myUnit2<scale, m_exp, kg_exp, s_exp> rhs) const {
        return repr >= rhs.repr;
    }

    inline constexpr bool operator!=(const myUnit2<scale, m_exp, kg_exp, s_exp> rhs) const {
        return repr != rhs.repr;
    }
};


/*
template<int m_exp, int kg_exp, int s_exp> 
struct picoMyUnit2  = myUnit2<pico, m_exp, kg_exp, s_exp>;

struct nanoMyUnit2  = 

struct microMyUnit2 = 

struct milliMyUnit2 = 

struct centiMyUnit2 = 

struct deciMyUnit2  = 

struct decaMyUnit2  = 

struct hectoMyUnit2 = 

struct kiloMyUnit2  = 

struct megaMyUnit2  = 

struct gigaMyUnit2  = 

struct teraMyUnit2  = 
     
myUnit2
*/
