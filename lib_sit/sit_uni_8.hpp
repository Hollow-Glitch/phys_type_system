
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


/* 
                                        ___Part I (more talkie talkie)___

    === Project and pre-projectt history ===

    I started this project with only knowing about:
    - nholthous/units
    - mp-units
    but due to the complexity and un-readable-ness of these solutions, I wanted to solve it by my own means.
    
    I started experimenting and the first thing that I implemented was a brute force method.
    Actually it was a program that generated a .cpp and .hpp which would have been the lib itself.
    Unfortunately math is a touch lover, and combinatorics was not kind.
    The solution was to implement using the generator hundreds or 1000-s of types which in their name
    such as: MetrePerSecond2 == m.s^-2; would encode the units and their exponents that the given value has.
    But even though the solutions was working, it was limited to the integer range of the exponents [-3;3]
    thus generating a couple of hundreds of types.
    This would not have been bad if not the number of operators that had to be generated, their count was above 200'000.
    
    After my brute-force experiments pretty soon I found myself using non-type template arguments 
    i.e.: numbers attached to the type.
    I was really surprised not seeing this solution method more often in the wild, and for that reason I thought
    that it is maybe a dead end also.
    But it worked pretty well and a lot less code was able to achieve the same feature-set as my brute force method
    pretty quickly.
    My following steps involved figuring out how I could solve the problem with scaling, i.e.:
    - having for ex.: MiliMetre while it not just being Metre but divided by 1000.
    - I did not want to divide and loose precision if all the units used would have been in MiliMetre -s.
    - So I added 6 additional unsigned template non-type arguments to the class which were modeling rational fractions
      representing the scaling factor corresponding to each unit.
    After that I found the pdf containing a presentation's slides of Scott Meyers.
    The solution to this very problem on those slides was one-to-one to my solution without the scaling part,
    but it contained some sophisticated points about "compatibility" with the `double` type, and also
    it showed that a lot more operators can be considered than I initially started with.
    The idea to specialize the template for the case where the exponents are 0 (in my case: with 1/1 scaling factors)
    also comes from these slides.
    So I added I think everything from these slides to my solution.
    Currently almost every operator and function ar non-member methods because that allows me to skip redundantly
    implementing them for the template specialization for the second time.



    === Exponent order rationale ===

    The reason for the order of template arguments is that most of the time we are considering
    - metre with positive exp
    - kilos with positive exp
    - seconds with both positive and negative exp
    
    So for this struct that is taylored for dynamics mainly, this is the most logical
    order because this way the most often occuring negative exponent will be at the end.
*/



/*
                                        ___Part II (pragmatic infos)___
    === Operators ===
    
    Operators are defined as non-members so that it is needed because of the template specialization
    to redundantly define them for the specialization's fixed arguments.
    
    The following operators are defined:
    
    - prefix      : +, -
                   ++, --
    - postfix     :++, --
    
    - infix       : +, -, *, /
    
      as          : {double} @ {Unit}     ...  implicitly allows:  {int}  @ {Unit}  
                    {Unit}   @ {double}   ...  implicitly allows:  {Unit} @ {int}
                    {Unit}   @ {Unit}
    
      where `@` means any of the infix arithmetics operators.
    
    - assign. ops.:  {Unit} += {Unit}
                     {Unit} -= {Unit}
                     {Unit} *= {double}   ...  implicitly allows:  {Unit} *= {int}
                     {Unit} /= {double}   ...  implicitly allows:  {Unit} /= {int}
                             =            ...  direct asign. op.
                                               is created implicitly by the compiler & we leave it like that
                                               because then our struct is trivially copyable.
    - comparison  : ==, !=, <, >, <=, >=



    === Converter & other functions ===
    
    !!! TODO !!!



    === Implicit conversions ===
    
    For general physical quantities like: kg, m.s^-1, etc.
    we don't allow implicit conversions because that could lead to unintended loss of semantics.
    In order for allow it if and only if the programmer decides to do so a separate function(-s)
    are provided. (At the beggining it was called ter(input)   ...   [t]ype [er]asure)
    
    The only exception is the set of values which are not scaled and have all exponents 0.
    For these a Template specialization ensures the existence for this and only this case.
    
    

    === Default value ===

    Default value = 1
    - so that if somebody default constructs
      don't stays surprised because of zero-ing out everything else.
*/


/*
                                        ___Part III (problems, ideas, etc.)___

    - if this affects performance negatively enough we would like all of this to be compilable away, i.e.:
      - using a #define variable and a prerpocessor #if #endif we would switch the templates and the
        unit name identifiers to just typedef/using-s of double
      - This does not come for free
      - any conversion function that without this could be made smart and use the [non-type_templ.var.]-s
        have to be implemented differently because when we flip the switch, nothing will be there.
        ex.:
            myUnit< 1, 1, m_exp,  1, 1, kg_exp,  1, 1, s_exp >
            toNonScaled( myUnit< m_num,  m_denom, m_exp, kg_num, kg_denom, kg_exp, s_num,  s_denom, s_exp > in ) {
                return in.repr * ((double) m_num /  m_denom) * ((double)kg_num / kg_denom) * ((double) s_num /  s_denom);
            } // this code has been simplified and probably wouldn't compile

        when used in this form would look like this:
            Metre_t  dist_passed_si  = toNonScaled(dist_passed );

        but the consequence is that code like this wouldn't be switchable to pure-compilation.
        In order for it to be so, if we assume that only scaling and integer exponents are in the game still
        then in order for it to be pure-compile-able the calling site would have to contain every information
        needed to perform the operation that the above code would do, while this new modified implementation of the
        above, would then have more template inputs, somehow it would have to check the relevant coefficients
        so that they are equal to that of the input. So:
            Calling site (correct):
                KiloMetre_t dist_passed {100};
                ...
                Metre_t  dist_passed_si  = toNonScaled<1,1,  1000,1,   1,1>(dist_passed );
                                                        m     kg       s
            Calling site (incorrect):
                Metre_t dist_passed {100}; //<< incorrect because of this
                ...
                Metre_t  dist_passed_si  = toNonScaled<1,1,  1000,1,   1,1>(dist_passed );
                                                        m     kg       s
            Callee site:
                myUnit< 1, 1, m_exp,  1, 1, kg_exp,  1, 1, s_exp >
                template <...// the usual
                    m_num_cur, m_denom_cur, kg_num_cur, kg_denom_cur, s_num_cur, s_denom_cur
                >
                toNonScaled( myUnit< m_num,  m_denom, m_exp, kg_num, kg_denom, kg_exp, s_num,  s_denom, s_exp > in ) {
                    static_assert( m_num   ==  m_num_cur   );
                    static_assert( m_denom ==  m_denom_cur );
                    static_assert(kg_num   == kg_num_cur   );
                    static_assert(kg_denom == kg_denom_cur );
                    static_assert( s_num   ==  s_num_cur   );
                    static_assert( s_denom ==  s_denom_cur );

                    return in.repr * ((double) m_num /  m_denom) * ((double)kg_num / kg_denom) * ((double) s_num /  s_denom);
                } // this code has been simplified and probably wouldn't compile
*/

// const Mass kilogram(1); // each of these constants sets its
// const Distance meter(1); // internal val field to 1.0
// const Time second(1);
//
//<< to be commented on: is not problematic when it comes to compile-to-pure-double functionality.



//  Wrapper arround double which contains during compile data needed for dimensional analysis.
//
// Template arguments:
//   numerator      | denominator      | exponent   |  unit
//  ----------------+------------------+------------+---------
//   uint32_t  m_num, uint32_t  m_denom, int m_exp, | metre
//   uint32_t kg_num, uint32_t kg_denom, int kg_exp,| kilogram
//   uint32_t  s_num, uint32_t  s_denom, int s_exp  | second
// 
// Default value = 1
//  
// Number_t  =type_equal=  struct myUnit< 1,1,0, 1,1,0, 1,1,0 >
// - is specialized so that it implicitly converts to double
template<
    uint32_t  m_num, uint32_t  m_denom, int m_exp, 
    uint32_t kg_num, uint32_t kg_denom, int kg_exp, 
    uint32_t  s_num, uint32_t  s_denom, int s_exp
> struct myUnit {
    double repr = 1;

    // Implemented by default;
    // - assignment operator=
    // - Constructor
};


template<>
struct myUnit<
     1,1,0, 
     1,1,0, 
     1,1,0
 > {
    double repr = 1;

    // For general notes and explanations see the general template struct above.
    //
    // Differences to the general template
    // - we allow implicit conversion to double

    // Implemented by default;
    // - assignment operator=

    myUnit(double initVal = 1) : repr{initVal} {}
    //
    //<< Allows the following to compile:
    //
    //     sit::Number_t test_func_for_impl_conv_of_Number_to_double(double doub) {
    //         return ((doub + 2)*doub)/5;
    //     } //       ^
    //                ^
    // without it, here the compiler would complain thet it can't convert from double to Number_t


    // Allows implicit conversion of `Number` to `double`
    operator double() const { return repr; }
};



// ================================================================================
// non-member operators
// ====================

// --------------------------------------------------------------------------------
// prefix + - ++ --
// ----------------

// infix: +{Unit}
// - is this really needed???
// - this makes a copy
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator +(
    const myUnit<
        m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
        s_num,  s_denom,  s_exp
    > in
) {
    return in;
}


// prefix: -{Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator -(
    const myUnit<
        m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
        s_num,  s_denom,  s_exp
    > in
) {
    return myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >{ - in.repr};
}


// prefix ++
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
>& operator ++(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& in
) {
    ++(in.repr);
    return in; //return new value by reference
}


// prefix --
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
>& operator --(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& in
) {
    --(in.repr);
    return in; //return new value by reference
}


// --------------------------------------------------------------------------------
// postfix ++ --
// -------------

 // postfix ++
 // - requires copy assignment???
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
 myUnit<
      m_num,  m_denom,  m_exp, 
     kg_num, kg_denom, kg_exp, 
      s_num,  s_denom,  s_exp
 > operator ++(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& in
     , int
 ) {
     myUnit<
          m_num,  m_denom,  m_exp, 
         kg_num, kg_denom, kg_exp, 
          s_num,  s_denom,  s_exp
     > old = in; //<< requires copy asign., which is implicitly present
     in.repr ++;
     return old;
 }


 // postfix --
 // - requires copy assignment???
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
 myUnit<
      m_num,  m_denom,  m_exp, 
     kg_num, kg_denom, kg_exp, 
      s_num,  s_denom,  s_exp
 > operator --(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& in
     , int
 ) {
     myUnit<
          m_num,  m_denom,  m_exp, 
         kg_num, kg_denom, kg_exp, 
          s_num,  s_denom,  s_exp
     > old = in; //<< copy old value - I assume it requires copy assignment
     in.repr --;
     return old;
 }


// --------------------------------------------------------------------------------
// infix arithmetics + - * /
// internal= 4 & interact= 4
// -------------------------

// infix: {Unit} + {Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator+(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
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
    >{ lhs.repr + rhs.repr };
}


// infix: {Unit} - {Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator-(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
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
    >{ lhs.repr - rhs.repr };
}


// infix  : {Unit} * {double}
// enables: {Unit} * {int}    through implicit conversion of {int}->{double}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator*(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const double rhs 
) { 
    return  myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >{ lhs.repr * rhs };
}


// infix  : {double} * {Unit}
// enables: {int}    * {Unit}    through implicit conversion of {int}->{double}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator*(
    const double lhs,
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& rhs 
) { 
    return  myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >{ lhs * rhs.repr };
}


// infix: {Unit} * {Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp,
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
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
    >& rhs
) { return  myUnit<
         m_num *  m_num2,  m_denom *  m_denom2,  m_exp +  m_exp2, 
        kg_num * kg_num2, kg_denom * kg_denom2, kg_exp + kg_exp2, 
         s_num *  s_num2,  s_denom *  s_denom2,  s_exp +  s_exp2
    >{ lhs.repr * rhs.repr };
}


// infix  : {Unit} / {double}
// enables: {Unit} / {int}    through implicit conversion of {int}->{double}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
> operator/(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const double rhs 
) { 
    return  myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >{ lhs.repr / rhs };
}


// infix  : {double} / {Unit}
// enables: {int}    / {Unit}    through implicit conversion of {int}->{double}
//      !!! careful this means inversion !!!
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_denom,  m_num, -  m_exp, 
    kg_denom, kg_num, - kg_exp, 
     s_denom,  s_num, -  s_exp
> operator/(
    const double lhs,
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& rhs 
) { 
    return  myUnit<
         m_denom,  m_num, -  m_exp, 
        kg_denom, kg_num, - kg_exp, 
         s_denom,  s_num, -  s_exp
    >{ lhs / rhs.repr };
}


// infix: {Unit} / {Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp,
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
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num2,  m_denom2,  m_exp2, 
        kg_num2, kg_denom2, kg_exp2, 
         s_num2,  s_denom2,  s_exp2
    >& rhs
) { return  myUnit<
         m_num *  m_denom2,  m_denom *  m_num2,  m_exp -  m_exp2, 
        kg_num * kg_denom2, kg_denom * kg_num2, kg_exp - kg_exp2, 
         s_num *  s_denom2,  s_denom *  s_num2,  s_exp -  s_exp2
    >{ lhs.repr / rhs.repr };
}


// --------------------------------------------------------------------------------
// asign. op.-s += -= *= /=
// ------------------------

// {Unit} += {Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
>& operator +=(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    lhs.repr += rhs.repr;
    return lhs; //return new value by reference
}


// {Unit} -= {Unit}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
>& operator -=(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    lhs.repr -= rhs.repr;
    return lhs; //return new value by reference
}

// {Unit} *= {double}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
>& operator *=(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& lhs,
    double rhs
) {
    lhs.repr *= rhs;
    return lhs; //return new value by reference
}


// {Unit} /= {double}
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
myUnit<
     m_num,  m_denom,  m_exp, 
    kg_num, kg_denom, kg_exp, 
     s_num,  s_denom,  s_exp
>& operator /=(
    myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& lhs,
    double rhs
) {
    lhs.repr /= rhs;
    return lhs; //return new value by reference
}


// --------------------------------------------------------------------------------
// comparison ==, !=, <, >, <=, >=
// -------------------------------

//  ==
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
inline constexpr bool operator==(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    return lhs.repr == rhs.repr;
}


//  !=
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
inline constexpr bool operator!=(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    return lhs.repr != rhs.repr;
}


//  <
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
inline constexpr bool operator<(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    return lhs.repr < rhs.repr;
}


//  >
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
inline constexpr bool operator>(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    return lhs.repr > rhs.repr;
}


//  <=
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
inline constexpr bool operator<=(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    return lhs.repr <= rhs.repr;
}


//  >=
template<
    uint32_t  m_num,  uint32_t  m_denom,  int m_exp, 
    uint32_t kg_num,  uint32_t kg_denom,  int kg_exp, 
    uint32_t  s_num,  uint32_t  s_denom,  int s_exp
>
inline constexpr bool operator>=(
    const myUnit<
         m_num,  m_denom, m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom, s_exp
    >& lhs,
    const myUnit<
         m_num,  m_denom,  m_exp, 
        kg_num, kg_denom, kg_exp, 
         s_num,  s_denom,  s_exp
    >& rhs
) {
    return lhs.repr >= rhs.repr;
}

// ================================================================================
// converter functions
// ===================

// Explicit [t]ype [er]asure which returns the underlying double.
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
