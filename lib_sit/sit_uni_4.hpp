
#include <cstdint>

//    m - meter    - Length
//    s - second   - Time
//   kg - kilogram - mass
//    A - Ampere   - Current
//    K - Kelvin   - Temperature
// mole - Mole     - ammount of substance
//   cd - candela  - Luminous intensity


// https://en.wikipedia.org/wiki/SI_derived_unit
//
//
// Those with only - m, kg, s:
//
//    Hz - hertz           - 1/s                      - s^-1
//   rad - radian          - m/m                      - 1
//    sr - steradian       - m^2/m^2                  - 1
//     N - newton          - kg.m/s^2                 - kg.m.s^-2
//    Pa - pascal          - n/m^2                    - kg.m^-1.s^-2
//     J - joule           - m.N, C.V, W.s            - kg.m^2.s^-2
//     W - watt            - J/s, V.A                 - kg.m^2.s^-3
//
//
// The rest:
//
// \degC - degree celsius  - K                        - K
//     C - coulomb         - s.A, F.V                 - 
//     V - volt            - W/A, J/C                 - 
//     F - farad           - C/V, s/\omega            - 
// \omega - ohm            - 1/S, V/A                 - 
//      S - siemens        - 1/\omega, A/V            - 
//     Wb - weber          - J/A, T.m^2, V.s          - 
//      T - tesla          - V.s/m^2, Wb/m^2, N/(A.m) - 
//      H - henry          - V.s/A, \omega.s, Wb/A    - 
//     lm - lumen          - cd.sr                    - 
//     lx - lux            - lm/m^2                   - 
//     Bq - becquerel      - 1/s                      - 
//     Gy - gray           - J/kg                     - 
//     Sv - siever         - J/kg                     - 
//    kat - katal          - mol/s                    - 



// The reason fot the order of template arguments is that most of the time we are considering
// - metre with positive exp
// - kilos with positive exp
// - seconds with both positive and negative exp
//
// So for this struct that is taylored for dynamics mainly, this is the most logical
// order because this way the most often occuring negative exponent will be at the end.
template<
    double  m_sca, int m_exp, 
    double kg_sca, int kg_exp, 
    double  s_sca, int s_exp
> struct myUnit {
    double repr = 1;
    // default=1 so that if somebody default constructs
    // don't stays surprised because of zero-ing out everything else.

    // +
    myUnit<
         m_sca, m_exp, 
        kg_sca, kg_exp, 
         s_sca, s_exp
    > operator+(myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) {
        return  myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp>{ repr + rhs.repr };
    }

    // -
    myUnit<
         m_sca, m_exp, 
        kg_sca, kg_exp, 
         s_sca, s_exp
    > operator-(myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) {
        return  myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp>{ repr - rhs.repr };
    }

    // *
    template<
        double m2_sca,  int m2_exp, 
        double kg2_sca, int kg2_exp,
        double s2_sca,  int s2_exp
    > 
    myUnit<
         m_sca *  m2_sca, m_exp  +  m2_exp, 
        kg_sca * kg2_sca, kg_exp + kg2_exp, 
         s_sca *  s2_sca, s_exp  +  s2_exp
    > 
    operator*(
        myUnit<m2_sca, m2_exp, kg2_sca, kg2_exp, s2_sca, s2_exp> rhs
    ) {
        return myUnit<
             m_sca *  m2_sca, m_exp  +  m2_exp, 
            kg_sca * kg2_sca, kg_exp + kg2_exp, 
             s_sca *  s2_sca, s_exp  +  s2_exp
        >{ repr * rhs.repr };
    }

    // /
    template<
        double  m2_sca, int  m2_exp, 
        double kg2_sca, int kg2_exp, 
        double  s2_sca, int  s2_exp
    >
    myUnit<
         m_sca /  m2_sca, m_exp  -  m2_exp, 
        kg_sca / kg2_sca, kg_exp - kg2_exp, 
         s_sca /  s2_sca, s_exp  -  s2_exp
    > 
    operator/(
        myUnit<m2_sca, m2_exp, kg2_sca, kg2_exp, s2_sca, s2_exp> rhs
    ) {
        return myUnit<
             m_sca /  m2_sca,  m_exp -  m2_exp, 
            kg_sca / kg2_sca, kg_exp - kg2_exp, 
             s_sca /  s2_sca,  s_exp -  s2_exp
        >{ repr / rhs.repr };
    }

    inline constexpr bool operator==(const myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) const {
        return repr == rhs.repr;
    }

    inline constexpr bool operator<(const myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) const {
        return repr < rhs.repr;
    }

    inline constexpr bool operator>(const myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) const {
        return repr > rhs.repr;
    }

    inline constexpr bool operator<=(const myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) const {
        return repr <= rhs.repr;
    }

    inline constexpr bool operator>=(const myUnit<m_sca, m_exp, kg_sca, kg_exp, s_sca, s_exp> rhs) const {
        return repr >= rhs.repr;
    }
};


// sit = SI [t]ypes
namespace sit {
    // Base units and their exp variants of interest to me(i.e.: m, kg, s):
    typedef myUnit< 1000, 1, 1, 0, 1, 0> KiloMetre_t;
    typedef myUnit<  0.1, 1, 1, 0, 1, 0> DeciMetre_t;
    typedef myUnit< 0.01, 1, 1, 0, 1, 0> CentiMetre_t;

    typedef myUnit< 1000, 1, 1, 0, 3600, 1> KiloMetrePerHour_t;

    typedef myUnit< 1, 0, 1, 0,   60, 1> Minute_t;
    typedef myUnit< 1, 0, 1, 0, 3600, 1> Hour_t;

    typedef myUnit< 1, 1, 1, 0, 1, 0> Metre_t;
    typedef myUnit< 1, 0, 1, 1, 1, 0> Kilogram_t;
    typedef myUnit< 1, 0, 1, 0, 1, 1> Second_t;
                        
    typedef myUnit< 1, 2, 1, 0, 1, 0> Metre2_t;
    typedef myUnit< 1, 0, 1, 2, 1, 0> Second2_t;
    typedef myUnit< 1, 0, 1, 0, 1, 2> Kilogram2_t;
                        
    typedef myUnit< 1, 3, 1, 0, 1, 0> Metre3_t;
    typedef myUnit< 1, 0, 1, 3, 1, 0> Second3_t;
    typedef myUnit< 1, 0, 1, 0, 1, 3> Kilogram3_t;
                        
    // // Base units and their exp variants as symbols:
    // typedef myUnit< 1, 1, 1, 0, 0> m_t;
    // typedef myUnit< 1, 1, 0, 0, 1> s_t;
    // typedef myUnit< 1, 1, 0, 1, 0> kg_t;
    //                     
    // typedef myUnit< 1, 1, 2, 0, 0> m2_t;
    // typedef myUnit< 1, 1, 0, 0, 2> s2_t;
    // typedef myUnit< 1, 1, 0, 2, 0> kg2_t;
    //                     
    // typedef myUnit< 1, 1, 3, 0, 0> m3_t;
    // typedef myUnit< 1, 1, 0, 0, 3> s3_t;
    // typedef myUnit< 1, 1, 0, 3, 0> kg3_t;

    // // Derived units of interest to me which only use - m, kg, s:
    // typedef myUnit< 1, 1, 0, 0,-1> Hertz_t;
    // typedef myUnit< 1, 1, 0, 0, 0> Radian_t;
    // typedef myUnit< 1, 1, 0, 0, 0> Steradian_t;
    // typedef myUnit< 1, 1, 1, 1,-2> Newton_t;
    // typedef myUnit< 1, 1, 1,-1,-2> Pascal_t;
    // typedef myUnit< 1, 1, 1, 2,-2> Joule_t;
    // typedef myUnit< 1, 1, 1, 2,-3> Watt_t;

    // // Derived units as symbols
    // typedef myUnit< 1, 1, 0, 0,-1> Hz_t;
    // typedef myUnit< 1, 1, 0, 0, 0> rad_t;
    // typedef myUnit< 1, 1, 0, 0, 0> sr_t;
    // typedef myUnit< 1, 1, 1, 1,-2> N_t;
    // typedef myUnit< 1, 1, 1,-1,-2> Pa_t;
    // typedef myUnit< 1, 1, 1, 2,-2> J_t;
    // typedef myUnit< 1, 1, 1, 2,-3> W_t;

    // // Miscelanious derived units
    // typedef myUnit< 1, 1, 1, 0,-1> MetrePerSecond_t;
    // typedef myUnit< 1, 1, 1, 0,-1> m_per_s_t;

    // // TODO: now that I added 'num' and 'denum' this seems to be wrong
    // //
    // // ter = [t]ype [er]asure, i.e. (convert to/return the) underlying double representation
    // template<uint32_t num, uint32_t denum, int m_exp, int kg_exp, int s_exp>
    // double ter(myUnit<num, denum, m_exp, kg_exp, s_exp> unit) {
    //     return unit.repr;
    // }

    // double ter(double unit) {
    //     return unit;
    // }

    // template<uint32_t num, uint32_t denum, int m_exp, int kg_exp, int s_exp> 
    // myUnit<1, 1, m_exp, kg_exp, s_exp>
    // toBase(myUnit<num, denum, m_exp, kg_exp, s_exp> in) {
    //     return myUnit<1, 1, m_exp, kg_exp, s_exp>{ in.repr * ((double)num / denum) };
    // }

    // // // NON-SI unit constructor functions
    // // inline Second_t Minute(double minute) { //EXPERIMENTAL
    // //     return Second_t{60 * minute};
    // // }

    // // inline Second_t Hour(double hour) { //EXPERIMENTAL
    // //     return Second_t{3600 * hour};
    // // }
}



// // CURENT PROBLEM WITH THIS APPROACH
// // - following line showcases it neatly:
// //
// //     print("test default one: {}m\n", ter(m{}) );
// //
// // - default construction to 1 now can't really work...
// 
// //>> the main point of this all
// sit::Metre_t     KiloMetre(double repr)  { return sit::Metre_t{1000 * repr}; }  // << THIS IS THE
// sit::Metre_t     CentiMetre(double repr) { return sit::Metre_t{0.01 * repr}; }  // << MAIN POINT HERE
// 
// sit::Second_t    Minute(double repr)     { return sit::Second_t{60 * repr}; }
// sit::Second_t    Hour(double repr)       { return sit::Second_t{3600 * repr}; }
// 
// sit::Metre_t     m(double repr)          { return sit::Metre_t{repr}; }
// sit::Second_t    s(double repr)          { return sit::Second_t{repr}; }
// sit::Kilogram_t  kg(double repr)         { return sit::Kilogram_t{repr}; }
// 
// sit::Metre2_t    m2(double repr)        { return sit::Metre2_t{repr}; }
// sit::Second2_t   s2(double repr)        { return sit::Second2_t{repr}; }
// sit::Kilogram2_t kg2(double repr)       { return sit::Kilogram2_t{repr}; }
// 
// sit::Metre_t     Metre(double repr)      { return sit::Metre_t{repr}; }
// sit::Second_t    Second(double repr)     { return sit::Second_t{repr}; }
// sit::Kilogram_t  Kilogram(double repr)   { return sit::Kilogram_t{repr}; }
// 
// sit::Metre2_t    Metre2(double repr)    { return sit::Metre2_t{repr}; }
// sit::Second2_t   Second2(double repr)   { return sit::Second2_t{repr}; }
// sit::Kilogram2_t Kilogram2(double repr) { return sit::Kilogram2_t{repr}; }
// 
// sit::Metre3_t    Metre3(double repr)    { return sit::Metre3_t{repr}; }
// sit::Second3_t   Second3(double repr)   { return sit::Second3_t{repr}; }
// sit::Kilogram3_t Kilogram3(double repr) { return sit::Kilogram3_t{repr}; }
// 
// sit::m3_t        m3(double repr)        { return sit::Metre3_t{repr}; }
// sit::s3_t        s3(double repr)        { return sit::Second3_t{repr}; }
// sit::kg3_t       kg3(double repr)       { return sit::Kilogram3_t{repr}; }
// 
// sit::Hertz_t     Hertz(double repr)     { return sit::Hertz_t{repr}; }
// sit::Radian_t    Radian(double repr)    { return sit::Radian_t{repr}; }
// sit::Steradian_t Steradian(double repr) { return sit::Steradian_t{repr}; }
// sit::Newton_t    Newton(double repr)    { return sit::Newton_t{repr}; }
// sit::Pascal_t    Pascal(double repr)    { return sit::Pascal_t{repr}; }
// sit::Joule_t     Joule(double repr)     { return sit::Joule_t{repr}; }
// sit::Watt_t      Watt(double repr)      { return sit::Watt_t{repr}; }


