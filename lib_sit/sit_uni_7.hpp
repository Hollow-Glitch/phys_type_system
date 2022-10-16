
#include <cstdint>

// Base units:
//          m - meter    - Length
//          s - second   - Time
//         kg - kilogram - mass
//          A - Ampere   - Current
//          K - Kelvin   - Temperature
//       mole - Mole     - ammount of substance
//         cd - candela  - Luminous intensity
//
// Derived units :: Those with only - m, kg, s:
//         Hz - hertz           - 1/s                      - s^-1
//        rad - radian          - m/m                      - 1
//         sr - steradian       - m^2/m^2                  - 1
//          N - newton          - kg.m/s^2                 - kg.m.s^-2
//         Pa - pascal          - n/m^2                    - kg.m^-1.s^-2
//          J - joule           - m.N, C.V, W.s            - kg.m^2.s^-2
//          W - watt            - J/s, V.A                 - kg.m^2.s^-3
//
// Derived units :: The rest:
//     \degC  - degree celsius  - K                        -
//         C  - coulomb         - s.A, F.V                 - 
//         V  - volt            - W/A, J/C                 - 
//         F  - farad           - C/V, s/\omega            - 
//     \omega - ohm             - 1/S, V/A                 - 
//          S - siemens         - 1/\omega, A/V            - 
//         Wb - weber           - J/A, T.m^2, V.s          - 
//          T - tesla           - V.s/m^2, Wb/m^2, N/(A.m) - 
//          H - henry           - V.s/A, \omega.s, Wb/A    - 
//         lm - lumen           - cd.sr                    - 
//         lx - lux             - lm/m^2                   - 
//         Bq - becquerel       - 1/s                      - 
//         Gy - gray            - J/kg                     - 
//         Sv - siever          - J/kg                     - 
//        kat - katal           - mol/s                    - 
//
// https://en.wikipedia.org/wiki/SI_derived_unit



// The reason fot the order of template arguments is that most of the time we are considering
// - metre with positive exp
// - kilos with positive exp
// - seconds with both positive and negative exp
//
// So for this struct that is taylored for dynamics mainly, this is the most logical
// order because this way the most often occuring negative exponent will be at the end.
template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
> struct myUnit {
    double repr = 1;
    // default=1 so that if somebody default constructs
    // don't stays surprised because of zero-ing out everything else.

    // ----------------------------------------
    // Constructor / copy-constructor
    // ------------------------------
    // - implemented by default
    
    // ----------------------------------------
    // unary +,-
    // ---------

    // unary +
    // - is this really needed???
    // - this makes a copy
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator +() {
        return myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >{ repr };
    }

    // unary -
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator -() {
        return myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >{ - repr};
    }

    // ----------------------------------------
    // prefix ++,--
    // ------------

    // prefix ++
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& operator ++() {
        repr++;
        return *this; //return new value by reference
    }

    // prefix --
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& operator --() {
        repr--;
        return *this; //return new value by reference
    }

    // ----------------------------------------
    // copy"="
    // -------
    // - copy assignment is implemented by default

    // operator=    ...   implicit

    // ----------------------------------------
    // postfix ++,--
    // -------------

    // postfix ++
    // - requires copy assignment???
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator ++(int) {
        myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        > old = *this; //<< copy old value - I assume it requires copy assignment
        operator++();
        return old;
    }

    // postfix --
    // - requires copy assignment???
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator --(int) {
        myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        > old = *this; //<< copy old value - I assume it requires copy assignment
        operator--();
        return old;
    }
    
    // ----------------------------------------
    // binary +,-,*,/
    // --------------

    // binary +
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator+(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) {
        return  myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >{ repr + rhs.repr };
    }

    // binary -
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator-(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) {
        return  myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >{ repr - rhs.repr };
    }

    // binary *
    template<
        uint32_t  m_num2, uint32_t  m_denom2, int  m_exp2, 
        uint32_t kg_num2, uint32_t kg_denom2, int kg_exp2, 
        uint32_t  s_num2, uint32_t  s_denom2, int  s_exp2
    >
    myUnit<
         m_num *  m_num2,  m_denom *  m_denom2,  m_exp +  m_exp2, 
        kg_num * kg_num2, kg_denom * kg_denom2, kg_exp + kg_exp2, 
         s_num *  s_num2,  s_denom *  s_denom2,  s_exp +  s_exp2
    > operator*( 
        const myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
        >& rhs
    ) { return  myUnit<
         m_num *  m_num2,  m_denom *  m_denom2,  m_exp +  m_exp2, 
        kg_num * kg_num2, kg_denom * kg_denom2, kg_exp + kg_exp2, 
         s_num *  s_num2,  s_denom *  s_denom2,  s_exp +  s_exp2
        >{ repr * rhs.repr };
    }

    // binary /
    template<
        uint32_t  m_num2, uint32_t  m_denom2, int  m_exp2, 
        uint32_t kg_num2, uint32_t kg_denom2, int kg_exp2, 
        uint32_t  s_num2, uint32_t  s_denom2, int  s_exp2
    >
    myUnit<
         m_num *  m_denom2,  m_denom *  m_num2,  m_exp -  m_exp2, 
        kg_num * kg_denom2, kg_denom * kg_num2, kg_exp - kg_exp2, 
         s_num *  s_denom2,  s_denom *  s_num2,  s_exp -  s_exp2
    > operator/( 
        const myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
        >& rhs
    ) { return  myUnit<
         m_num *  m_denom2,  m_denom *  m_num2,  m_exp -  m_exp2, 
        kg_num * kg_denom2, kg_denom * kg_num2, kg_exp - kg_exp2, 
         s_num *  s_denom2,  s_denom *  s_num2,  s_exp -  s_exp2
        >{ repr / rhs.repr };
    }

    // ----------------------------------------
    //   +=, -=
    // --------
    // - *= and /= make no sense 
    // - the unit on which it would be called would be incompatible
    //   with the united value which it should take !!!

    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& operator +=(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) {
        repr += rhs.repr;
        return *this; //return new value by reference
    }

    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& operator -=(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) {
        repr -= rhs.repr;
        return *this; //return new value by reference
    }

    // ----------------------------------------
    // comparison ==, !=, <, >, <=, >=
    // -------------------------------

    inline constexpr bool operator==(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) const {
        return repr == rhs.repr;
    }

    inline constexpr bool operator!=(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) const {
        return repr != rhs.repr;
    }

    inline constexpr bool operator<(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) const {
        return repr < rhs.repr;
    }

    inline constexpr bool operator>(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) const {
        return repr > rhs.repr;
    }

    inline constexpr bool operator<=(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) const {
        return repr <= rhs.repr;
    }

    inline constexpr bool operator>=(
        const myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >& rhs
    ) const {
        return repr >= rhs.repr;
    }

    // ------------------------------------------------------------
    // interactions with double
    // ------------------------

    // binary *
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    > operator*( double rhs ) { 
        return  myUnit<
             m_num,  m_denom,  m_exp, 
            kg_num, kg_denom, kg_exp, 
             s_num,  s_denom,  s_exp
        >{ repr * rhs.repr };
    }

    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& operator *=(double rhs) {
        repr *= rhs.repr;
        return *this; //return new value by reference
    }

    // !!! This makes implicit conversion to double possible !!!
    // operator double() const { return repr; }
    //
    //<< let's only allow this for Numbers, i.e.: for types with 0 exponents.
    //   The reason for this is that this leads to loosing semantics,
    //   so if we want to allow semantic loss, then this semantic loss should be explicit,
    //   Thus leading to easy to read and reason about code with no hidden behaviour.
};


//>> This is template specialization, at this point I had Scott Meyers's presentation,
//   where using template specialization was mentioned so that `Number`-s are compatible
//   with the underlying data type.

template<>
struct myUnit<
     1,1,0, 
     1,1,0, 
     1,1,0
 > {
    double repr = 1;
    // default=1 so that if somebody default constructs
    // don't stays surprised because of zero-ing out everything else.

    // Allows implicit conversion of `Number` to `double`
    operator double() const { return repr; }

    // ----------------------------------------
    // Constructor / copy-constructor
    // ------------------------------
    // - implemented by default

    // Allows implicit conversion of `double` to `Number`
    // TODO: consider making everything default construct to `0`
    myUnit(double initVal = 1) : repr{initVal} {}
    
    // ----------------------------------------
    // unary +,-
    // ---------

    // unary +
    // - is this really needed???
    // - this makes a copy
    myUnit<
        1, 1, 0, 
        1, 1, 0, 
        1, 1, 0
    > operator +() {
        return myUnit<
            1, 1, 0, 
            1, 1, 0, 
            1, 1, 0
        >{ repr };
    }

    // unary -
    myUnit<
        1, 1, 0, 
        1, 1, 0, 
        1, 1, 0
    > operator -() {
        return myUnit<
            1, 1, 0, 
            1, 1, 0, 
            1, 1, 0
        >{ - repr};
    }

    // ----------------------------------------
    // prefix ++,--
    // ------------

    // prefix ++
    myUnit<
        1, 1, 0, 
        1, 1, 0, 
        1, 1, 0
    >& operator ++() {
        repr++;
        return *this; //return new value by reference
    }

    // prefix --
    myUnit<
        1, 1, 0, 
        1, 1, 0, 
        1, 1, 0
    >& operator --() {
        repr--;
        return *this; //return new value by reference
    }

    // ----------------------------------------
    // copy"="
    // -------
    // - copy assignment is implemented by default

    // operator=    ...   implicit

    // ----------------------------------------
    // postfix ++,--
    // -------------

    // postfix ++
    // - requires copy assignment???
    myUnit<
        1, 1, 0, 
        1, 1, 0, 
        1, 1, 0
    > operator ++(int) {
        myUnit<
            1, 1, 0, 
            1, 1, 0, 
            1, 1, 0
        > old = *this; //<< copy old value - I assume it requires copy assignment
        operator++();
        return old;
    }

    // postfix --
    // - requires copy assignment???
    myUnit<
        1, 1, 0, 
        1, 1, 0, 
        1, 1, 0
    > operator --(int) {
        myUnit<
            1, 1, 0, 
            1, 1, 0, 
            1, 1, 0
        > old = *this; //<< copy old value - I assume it requires copy assignment
        operator--();
        return old;
    }
    
    // ----------------------------------------
    // binary +,-,*,/
    // --------------

    // +
    myUnit<
        1,1,0, 
        1,1,0, 
        1,1,0
    > operator+(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) {
        return  myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >{ repr + rhs.repr };
    }

    // -
    myUnit<
        1,1,0, 
        1,1,0, 
        1,1,0
    > operator-(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) {
        return  myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >{ repr - rhs.repr };
    }

    // *
    template<
        uint32_t  m_num2, uint32_t  m_denom2, int  m_exp2, 
        uint32_t kg_num2, uint32_t kg_denom2, int kg_exp2, 
        uint32_t  s_num2, uint32_t  s_denom2, int  s_exp2
    >
    myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
    > operator*( 
        const myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
        >& rhs
    ) { return  myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
        >{ repr * rhs.repr };
    }

    // /
    template<
        uint32_t  m_num2, uint32_t  m_denom2, int  m_exp2, 
        uint32_t kg_num2, uint32_t kg_denom2, int kg_exp2, 
        uint32_t  s_num2, uint32_t  s_denom2, int  s_exp2
    >
    myUnit<
         m_denom2,  m_num2, -  m_exp2, 
        kg_denom2, kg_num2, - kg_exp2, 
         s_denom2,  s_num2, -  s_exp2
    > operator/( 
        const myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
        >& rhs
    ) { return  myUnit<
         m_denom2,  m_num2, -  m_exp2, 
        kg_denom2, kg_num2, - kg_exp2, 
         s_denom2,  s_num2, -  s_exp2
        >{ repr / rhs.repr };
    }

    // ----------------------------------------
    // comparison ==, !=, <, >, <=, >=
    // -------------------------------

    // I had to remove `constexpr` idk why - TODO: find out
    inline constexpr bool operator==(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) const {
        return repr == rhs.repr;
    }

    // I had to remove `constexpr` idk why - TODO: find out
    inline constexpr bool operator!=(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) const {
        return repr != rhs.repr;
    }

    // I had to remove `constexpr` idk why - TODO: find out
    inline constexpr bool operator<(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) const {
        return repr < rhs.repr;
    }

    // I had to remove `constexpr` idk why - TODO: find out
    inline constexpr bool operator>(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) const {
        return repr > rhs.repr;
    }

    // I had to remove `constexpr` idk why - TODO: find out
    inline constexpr bool operator<=(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) const {
        return repr <= rhs.repr;
    }

    // I had to remove `constexpr` idk why - TODO: find out
    inline constexpr bool operator>=(
        const myUnit<
            1,1,0, 
            1,1,0, 
            1,1,0
        >& rhs
    ) const {
        return repr >= rhs.repr;
    }
};


// Explicit [t]ype [er]asure which returns the undrlying double.
// Since we are now not allowing implicit conversion of any unit type to double
// this is a necessary component of the library, for ex. without this displaying values
// would be impossible.
template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
>
double ter(
    myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    > in
) {
    return in.repr;
}


// IF YOU ATTEMPT TO MAKE IT "double" COMPATIBLE THEN COPY AND CREATE NEW VERSION
//
// If implemented like this then it doesn't allow disabling this library's unit checking.
template<
    uint32_t  m_num, uint32_t  m_denom, int  m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int  s_exp
> myUnit<
    1, 1, m_exp, 
    1, 1, kg_exp, 
    1, 1, s_exp
>
toNonScaled(
    myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    > in
) {
    return myUnit<
        1, 1, m_exp, 
        1, 1, kg_exp, 
        1, 1, s_exp
    >{ 
        in.repr 
            * ((double) m_num /  m_denom) 
            * ((double)kg_num / kg_denom)
            * ((double) s_num /  s_denom)
    };
}


template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
> myUnit<
     m_num,  m_denom, m_exp, 
    kg_num, kg_denom, kg_exp, 
         1,     1000, s_exp
>
ToMiliSeconds(
    myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    > in
) {
    return myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
             1,     1000, s_exp
    >{ 
        in.repr * ((double) s_num /  s_denom) * 1000
    };
}

template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
> myUnit<
      1000,        1, m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom, s_exp
>
lengthToKiloMeters(
    myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    > in
) {
    return myUnit<
          1000,        1, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >{ 
        in.repr * ((double) m_num /  m_denom) / 1000.0
    };
}

template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
> myUnit<
     m_num,  m_denom, m_exp, 
    kg_num, kg_denom, kg_exp, 
      3600,        1, s_exp
>
TimeToHours(
    myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    > in
) {
    return myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
          3600,        1, s_exp
    >{ 
        in.repr * ((double) s_num /  s_denom) / 3600.0
    };
}

template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
> myUnit<
     m_num,  m_denom, m_exp, 
    kg_num, kg_denom, kg_exp, 
         1,     3600, s_exp
>
InvTimeToHours(
    myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    > in
) {
    return myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
             1,     3600, s_exp
    >{ 
        in.repr * ((double) s_num /  s_denom) * 3600.0
    };
}


// sit = SI [t]ypes
namespace sit {

    // template<int m_exp, int kg_exp, int s_exp>
    // using nonScaledUnit = myUnit<
    //     1, 1,  m_exp, 
    //     1, 1, kg_exp, 
    //     1, 1,  s_exp
    // >;
    //
    //<< the reason why I am redoing this is so that the compiler and
    //   intelisense can trivially associate MetrePerSecond & etc. with their "constant set"-s

    // ----------------------------------------
    // Number
    // ------

    //using Number_t = nonScaledUnit<0,0,0>;
    //<< replaced with:
    using Number_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;

    // ============================================================
    // Base units
    // ==========

    // ----------------------------------------
    // Length
    // ------

    // ---  exp = 1  ---

    using NanoMetre_t =  myUnit<
           1, 1'000'000'000, 1,  //  m
           1,             1, 0,  // kg
           1,             1, 0   //  s
    >;

    using MicroMetre_t =  myUnit<
           1, 1'000'000, 1,  //  m
           1,         1, 0,  // kg
           1,         1, 0   //  s
    >;

    using MiliMetre_t =  myUnit<
           1, 1000, 1,  //  m
           1,    1, 0,  // kg
           1,    1, 0   //  s
    >;

    using CentiMetre_t =  myUnit<
           1, 100, 1,  //  m
           1,   1, 0,  // kg
           1,   1, 0   //  s
    >;

    using DeciMetre_t =  myUnit<
           1, 10, 1,  //  m
           1,  1, 0,  // kg
           1,  1, 0   //  s
    >;

    // using    Metre_t = nonScaledUnit< 1, 0, 0>;
    //<<replaced with:
    using Metre_t =  myUnit<
           1, 1, 1,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;

    using KiloMetre_t =  myUnit<
        1000, 1, 1,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;
    
    // ---  exp = 2  ---

    using CentiMetre2_t =  myUnit<
           1, 10'000, 2,  //  m
           1,      1, 0,  // kg
           1,      1, 0   //  s
    >;

    using DeciMetre2_t =  myUnit<
           1, 100, 2,  //  m
           1,   1, 0,  // kg
           1,   1, 0   //  s
    >;

    //using    Metre2_t = nonScaledUnit< 2, 0, 0>;
    //<< replaced with:
    using Metre2_t =  myUnit<
           1, 1, 2,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;

    using KiloMetre2_t =  myUnit<
        1'000'000, 1, 2,  //  m
                1, 1, 0,  // kg
                1, 1, 0   //  s
    >;

    // ---  exp = 3  ---

    using CentiMetre3_t =  myUnit<
           1, 1'000'000, 3,  //  m
           1,         1, 0,  // kg
           1,         1, 0   //  s
    >;

    using DeciMetre3_t =  myUnit<
           1, 1000, 3,  //  m
           1,    1, 0,  // kg
           1,    1, 0   //  s
    >;

    //using    Metre3_t = nonScaledUnit< 3, 0, 0>;
    //<< replaced with:
    using Metre3_t =  myUnit<
           1, 1, 3,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;

    using KiloMetre3_t =  myUnit<
        1'000'000'000, 1, 3,  //  m
                    1, 1, 0,  // kg
                    1, 1, 0   //  s
    >;

    // ----------------------------------------
    // Time
    // ----

    // ---  exp = 1  ---

    using NanoSecond_t =  myUnit<
            1,             1, 0,  //  m
            1,             1, 0,  // kg
            1, 1'000'000'000, 1   //  s
    >;

    using MicroSecond_t =  myUnit<
            1,         1, 0,  //  m
            1,         1, 0,  // kg
            1, 1'000'000, 1   //  s
    >;

    using MiliSecond_t =  myUnit<
            1,    1, 0,  //  m
            1,    1, 0,  // kg
            1, 1000, 1   //  s
    >;

    //using   Second_t = nonScaledUnit< 0, 0, 1>;
    //<< replaced with:
    using Second_t =  myUnit<
            1, 1, 0,  //  m
            1, 1, 0,  // kg
            1, 1, 1   //  s
    >;

    using Minute_t =  myUnit<
            1, 1, 0,  //  m
            1, 1, 0,  // kg
           60, 1, 1   //  s
    >;

    using Hour_t =  myUnit<
              1, 1, 0,  //  m
              1, 1, 0,  // kg
           3600, 1, 1   //  s
    >;

    // ---  exp = 2  ---

    //using   Second2_t = nonScaledUnit< 0, 0, 2>;
    //<< replaced with:
    using Second2_t =  myUnit<
            1, 1, 0,  //  m
            1, 1, 0,  // kg
            1, 1, 2   //  s
    >;

    using Minute2_t =  myUnit<
              1, 1, 0,  //  m
              1, 1, 0,  // kg
           3600, 1, 2   //  s
    >;

    using Hour2_t =  myUnit<
                 1, 1, 0,  //  m
                 1, 1, 0,  // kg
           216'000, 1, 2   //  s
    >;

    // ---  exp = 3  ---

    //using   Second3_t = nonScaledUnit< 0, 0, 3>;
    //<< replaced with:
    using Second3_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 0,  // kg
           1, 1, 3   //  s
    >;

    using Minute3_t =  myUnit<
                 1, 1, 0,  //  m
                 1, 1, 0,  // kg
           216'000, 1, 3   //  s
    >;

    using Hour3_t =  myUnit<
                    1, 1, 0,  //  m
                    1, 1, 0,  // kg
           12'960'000, 1, 3   //  s
    >;

    // ----------------------------------------
    // Mass
    // ----

    // ---  exp = 1  ---

    //using Kilogram_t  = nonScaledUnit< 0, 1, 0>;
    //<< replaced with:
    using Kilogram_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 1,  // kg
           1, 1, 0   //  s
    >;

    // ---  exp = 2  ---

    //using Kilogram2_t = nonScaledUnit< 0, 2, 0>;
    //<< replaced with:
    using Kilogram2_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 2,  // kg
           1, 1, 0   //  s
    >;

    // ---  exp = 3  ---

    //using Kilogram3_t = nonScaledUnit< 0, 3, 0>;
    //<< replaced with:
    using Kilogram3_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 3,  // kg
           1, 1, 0   //  s
    >;

    // ============================================================
    // Derived units
    // =============

    //using Hertz_t     = nonScaledUnit< 0, 0,-1>;
    using Hertz_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 0,  // kg
           1, 1,-1   //  s
    >;

    //using Radian_t    = nonScaledUnit< 0, 0, 0>;
    using Radian_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;

    //using Steradian_t = nonScaledUnit< 0, 0, 0>;
    using Steradian_t =  myUnit<
           1, 1, 0,  //  m
           1, 1, 0,  // kg
           1, 1, 0   //  s
    >;

    //using Newton_t    = nonScaledUnit< 1, 1,-2>;
    using Newton_t =  myUnit<
           1, 1, 1,  //  m
           1, 1, 1,  // kg
           1, 1,-2   //  s
    >;

    //using Pascal_t    = nonScaledUnit< 1,-1,-2>;
    using Pascal_t =  myUnit<
           1, 1, 1,  //  m
           1, 1,-1,  // kg
           1, 1,-2   //  s
    >;

    //using Joule_t     = nonScaledUnit< 1, 2,-2>;
    using Joule_t =  myUnit<
           1, 1, 1,  //  m
           1, 1, 2,  // kg
           1, 1,-2   //  s
    >;

    //using Watt_t      = nonScaledUnit< 1, 2,-3>;
    using Watt_t =  myUnit<
           1, 1, 1,  //  m
           1, 1, 2,  // kg
           1, 1,-3   //  s
    >;

    // ============================================================
    // Miscelanious units
    // ==================

    // ----------------------------------------
    // Velocity
    // --------

    //using MetrePerSecond_t = nonScaledUnit< 1, 0,-1>;
    using MetrePerSecond_t = myUnit<
              1, 1, 1,  //  m
              1, 1, 0,  // kg
              1, 1,-1   //  s
    >;

    using KilometrePerHour_t = myUnit<
           1000,    1, 1,  //  m
              1,    1, 0,  // kg
              1, 3600,-1   //  s
    >;

    // ----------------------------------------
    // Acceleration
    // ------------

    //using MetrePerSecond2_t = nonScaledUnit< 1, 0,-2>;
    using MetrePerSecond2_t = myUnit<
              1, 1, 1,  //  m
              1, 1, 0,  // kg
              1, 1,-2   //  s
    >;

    using KilometrePerHour2_t = myUnit<
           1000,    1, 1,  //  m
              1,    1, 0,  // kg
              1, 3600,-2   //  s
    >;

    // ============================================================
    // Abstract Experimental Stuff
    // ===========================

    namespace ExperimentalTypes {
        template<
            uint32_t  m_num, uint32_t  m_denom
        >
        using Length_Experimental_t = myUnit<
                m_num,  m_denom, 1,  //  m
                    1,        1, 0,  // kg
                    1,        1, 0   //  s
        >;

        template<
            uint32_t  s_num, uint32_t  s_denom
        >
        using Time_Experimental_t = myUnit<
                    1,        1, 0,  //  m
                    1,        1, 0,  // kg
                s_num,  s_denom, 1   //  s
        >;

        template<
            uint32_t  m_num, uint32_t  m_denom,
            uint32_t  s_num, uint32_t  s_denom
        >
        using Velocity_Experimental_t = myUnit<
                m_num,  m_denom, 1,  //  m
                    1,        1, 0,  // kg
                s_num,  s_denom,-1   //  s
        >;
    }
}
