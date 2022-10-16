//>> Standard
#include <cassert>
#include <cmath>

#include <iostream>
#include <filesystem>
#include <string>
#include <memory>
#include <limits>

//>> vcpkg
#include <fmt/core.h>

//>> my stuff
#include "sit_uni.hpp"

// ================================================================================
// Some template metaprogramming basics
// ====================================

template<int i> struct Fac
{
    static const int result = i * Fac<i-1>::result;
};
 
template<> struct Fac<1>
{
    static const int result = 1;
};
 

// ================================================================================
// Demo
// ====


sit::m_per_s_t my_avg_speed(sit::m_t d, sit::s_t t)
{
    return d / t;
}


int main() {
    {
        auto meter = myUnit<1,0,0>{10};
        fmt::print("meter = {}\n", meter.repr);

        auto meter3 = meter * meter * meter;
        //myUnit<5,0,0> should_get_error = meter3;
        //auto still_should_be_wrong = meter + meter3;
        auto five_kilos = myUnit<0,1,0>{5};
        auto velocity   = myUnit<1,0,0>{1000} / myUnit<0,0,1>{60 * 60};
        auto newton     = myUnit<0,1,0>{2}    * myUnit<1,0,0>{5} * myUnit<0,0,-2>{10};
    }

    { // now the same but with typedefs
        using namespace sit;
        using namespace fmt;

        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

        auto velocity     = m(1000) / s(60 * 5);
        auto acceleration = velocity / s(10);
        auto newton       = m(2) * kg(5) / s2(10);

        print("velocity     = {}m/s\n",     ter(velocity) );
        print("acceleration = {}m/s2\n",    ter(acceleration) );
        print("newton       = {}m.kg/s2\n", ter(newton) );
        print("\n");

        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

        //>> test my avera func
        auto dist = m(1000);
        auto time = s(60*60);
        // auto incorrect = my_avg_speed(time, dist);
        auto correct = my_avg_speed(dist, time);
        print("correct = {}m/s\n", ter(correct) );
        print("\n");

        // print("test default one: {}m\n", ter(m{}) ); 
        //                                       ^
        //          Very problematic, now we lost default construction to `1`

        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

        Newton_t val1 = Newton(5.5);
        Newton_t val2 = Newton(5.5);
        Newton_t val3 = Newton(10);

        print("val1 = {}\n", ter(val1) );
        print("val2 = {}\n", ter(val2) );
        print("val3 = {}\n", ter(val3) );
        print("\n");
        print("val1 == val2 :equiv: {}\n", val1 == val2);
        print("val1 == val3 :equiv: {}\n", val1 == val3);
        print("\n");
        print("val1 <  val2 :equiv: {}\n", val1 <  val2);
        print("val1 <  val3 :equiv: {}\n", val1 <  val3);
        print("\n");
        print("val1 <= val2 :equiv: {}\n", val1 <= val2);
        print("val1 <= val3 :equiv: {}\n", val1 <= val3);
        print("\n");
        print("val1 >  val2 :equiv: {}\n", val1 >  val2);
        print("val1 >  val3 :equiv: {}\n", val1 >  val3);
        print("\n");
        print("val1 >= val2 :equiv: {}\n", val1 >= val2);
        print("val1 >= val3 :equiv: {}\n", val1 >= val3);

        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

        print("sizeof(double)  = {}\n", sizeof(double)  );
        print("sizeof(Second)  = {}\n", sizeof(Second_t)  );
        print("sizeof(Newton)  = {}\n", sizeof(Newton_t)  );
        print("sizeof(m_per_s) = {}\n", sizeof(m_per_s_t) );

        print(" - \n");
        print("---\n");
        print(" - \n");

        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

        // myUnit2<std::ratio<1000,1>, 1, 0, 0>
        auto a = std::ratio<1000, 1>{};
        print("min = {}\n", std::numeric_limits<long double>::min() );
        print("max = {}\n", std::numeric_limits<long double>::max() );

    }
    fmt::print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        auto distance = KiloMetre(90);
        auto time = Hour(2);
        fmt::print("distance / time = {} m/s\n", sit::ter(distance / time) );
    }

    return 0;
}









 