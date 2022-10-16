//>> Standard
#include <cassert>
#include <cmath>

#include <iostream>
#include <filesystem>
#include <string>
#include <memory>
#include <limits>
#include <typeinfo>

//>> vcpkg
#include <fmt/core.h>

//>> my stuff
#include "sit_uni_5.hpp"


sit::MetrePerSecond_t my_avg_speed(sit::Metre_t d, sit::Second_t t)
{
    return d / t;
}

sit::KilometrePerHour_t my_avg_speed(sit::KiloMetre_t d, sit::Hour_t t)
{
    return d / t;
}

double my_avg_speed_impl_conv(double d, double t) {
    return d / t;
}


// Length_Experimental_t  
// Time_Experimental_t    
// Velocity_Experimental_t


template<
    uint32_t  m_num, uint32_t  m_denom,
    uint32_t  s_num, uint32_t  s_denom
>
auto my_avg_speed_experimental(
    sit::ExperimentalTypes::Length_Experimental_t< m_num,  m_denom > d, 
    sit::ExperimentalTypes::Time_Experimental_t< s_num,  s_denom > t
) {
    return d / t;
}
//<< disadvantage: Currently I don't know how I could write the return type, 
//                 or if it is possible without manually calculating by hand.
//                 Probably it can't be done sanely



int main() {
    using namespace sit;
    using namespace fmt;

    {
        Metre_t    dist{10};
        Kilogram_t mass{ 5};

        KiloMetre_t dist_passed {100};
        Hour_t      time_it_took{  2};
        auto test     = dist * dist_passed; // << very  INTERESTING  but seems to be  CORRECT!!!
        auto velocity = dist_passed / time_it_took;
        auto test_inverse = Number_t{} / velocity;

        fmt::print("{}\n", typeid(dist_passed ).name() );
        fmt::print("{}\n", typeid(time_it_took).name() );
        fmt::print("{}\n", typeid(test        ).name() );
        fmt::print("{}\n", typeid(velocity    ).name() );
        fmt::print("{}\n", typeid(test_inverse).name() );

        fmt::print("{}km\n",   (double)dist_passed );
        fmt::print("{}h\n",    (double)time_it_took);
        fmt::print("{}km/h\n", (double)velocity    );

        // auto     some_force_1 = Kilogram_t{2} * Metre_t{5} / Second2_t{10};
        // Newton_t some_force_2 = Kilogram_t{2} * Metre_t{5} / Second2_t{10};
        // if (some_force_1 == some_force_2) {
        //     print("some_force_1 == some_force_2\n");
        // } else {
        //     print("some_force_1 != some_force_2\n");
        // }
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        auto distance = KiloMetre_t{90};
        auto dist2 = distance * distance;
        auto time = Hour_t{2};
        fmt::print("{} km^2\n", dist2.repr);
        // fmt::print("distance / time = {} km/h\n", sit::ter(distance / time) );
        // fmt::print("distance / time = {}  m/s\n", sit::ter(sit::toBase(distance / time)) );
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        auto crate_mass  = Kilogram_t{20};
        auto crate_accel = MetrePerSecond2_t{10};

        Newton_t crate_force = crate_mass * crate_accel;

        print("crate_mass  = {}kg\n",       crate_mass );
        print("crate_accel = {}m/s^2\n",    crate_accel);
        print("crate_force = {}kg.m/s^2\n", crate_force);
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        KiloMetre_t dist_passed {100};
        Hour_t      time_it_took{  2};
        auto velocity = dist_passed / time_it_took;

        Metre_t  dist_passed_si  = toNonScaled(dist_passed );
        Second_t time_it_took_si = toNonScaled(time_it_took);
        auto     velocity_si     = toNonScaled(velocity);  // even linting works now :D

        print("dist_passed  = {}km\n",   (double)dist_passed );
        print("time_it_took = {}h\n",    (double)time_it_took);
        print("velocity     = {}km/h\n", (double)velocity    );
        print("\n");
        print("dist_passed_si  = {}m\n",   (double)dist_passed_si );
        print("time_it_took_si = {}s\n",   (double)time_it_took_si );
        print("velocity_si     = {}m/s\n", (double)velocity_si );
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        print("Simple functions:\n\n");
        KiloMetre_t dist_passed {100};
        Hour_t      time_it_took{  2};
        auto velocity = dist_passed / time_it_took;

        Metre_t          dist_passed_si  = toNonScaled(dist_passed );
        Second_t         time_it_took_si = toNonScaled(time_it_took);
        MetrePerSecond_t velocity_si     = toNonScaled(velocity);  // even linting works now :D

        auto some1 = my_avg_speed(dist_passed, time_it_took);
        // my_avg_speed(dist_passed_si, time_it_took);   //<< throws error, yupppiiiii
        // my_avg_speed(dist_passed, time_it_took_si);   //<< throws error, yupppiiiii
        auto some2 = my_avg_speed(dist_passed_si, time_it_took_si);
        double some3 = my_avg_speed_impl_conv(dist_passed, time_it_took);
        double some4 = my_avg_speed_impl_conv(dist_passed_si, time_it_took);
        double some5 = my_avg_speed_impl_conv(dist_passed, time_it_took_si);
        double some6 = my_avg_speed_impl_conv(dist_passed_si, time_it_took_si);

        print("my_avg_speed           (dist_passed,    time_it_took)    = {:>11.5f} km/h\n", some1);
        print("my_avg_speed           (dist_passed_si, time_it_took_si) = {:>11.5f}  m/s\n",  some2);
        print("my_avg_speed_impl_conv (dist_passed,    time_it_took)    = {:>11.5f} km/h  !!! with implicit conversion to double\n", some3);
        print("my_avg_speed_impl_conv (dist_passed_si, time_it_took)    = {:>11.5f}  m/h  !!! with implicit conversion to double\n", some4);
        print("my_avg_speed_impl_conv (dist_passed,    time_it_took_si) = {:>11.5f} km/s  !!! with implicit conversion to double\n", some5);
        print("my_avg_speed_impl_conv (dist_passed_si, time_it_took_si) = {:>11.5f}  m/s  !!! with implicit conversion to double\n", some6);

        assert(toNonScaled(some1) == some2);
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        print("Experimental generic function:\n\n");
        KiloMetre_t dist_passed {100};
        Hour_t      time_it_took{  2};

        Metre_t          dist_passed_si  = toNonScaled(dist_passed );
        Second_t         time_it_took_si = toNonScaled(time_it_took);

        auto res1 = my_avg_speed_experimental( dist_passed, time_it_took );
        auto res2 = my_avg_speed_experimental( dist_passed_si, time_it_took_si );
        print("res1 = {}km/h\n", res1);
        print("res2 = {}m/s\n", res2);
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        MiliSecond_t mili_sec{2};
        Minute_t     min{3};
        
        auto not_sane_time_unit = mili_sec * min;
        auto sane_time_unit     = toNonScaled(mili_sec * min);

        print("not_sane_time_unit = {}[ms.min]\n", not_sane_time_unit);
        print("sane_time_unit     = {}s^2\n", sane_time_unit);

        print(" - - - - - - - - - - - - - - - \n");

        MiliSecond_t mm{2};
        KiloMetre_t  km{3};
        
        auto not_sane_length_unit = mm * km;
        auto sane_length_unit     = toNonScaled(not_sane_length_unit);

        print("not_sane_length_unit = {}[mm.km]\n", not_sane_length_unit);
        print("sane_length_unit     = {}m^2\n", sane_length_unit);
    }
    print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    {
        KiloMetre_t dist_passed {100};
        Hour_t      time_it_took{  2};
        KilometrePerHour_t velo_1 = dist_passed / time_it_took;

        Metre_t     dist_passed_si  = toNonScaled(dist_passed );
        Second_t    time_it_took_si = toNonScaled(time_it_took);
        MetrePerSecond_t velo_2 = dist_passed_si / time_it_took_si;

        KiloMetre_t        dist_3 = lengthToKiloMeters(dist_passed_si);
        Hour_t             time_3 = TimeToHours(time_it_took_si);

        // using KilometrePerHour_t = myUnit<
        //        1000,    1, 1,  //  m
        //           1,    1, 0,  // kg
        //           1, 3600,-1   //  s
        // >;
        //KilometrePerHour_t velo_3 = lengthToKiloMeters(TimeToHours(velo2)); //<< this gives error and it should!!!

        // KilometrePerHour_t velo_3 = lengthToKiloMeters(InvTimeToHours(velo_2));
        auto velo_3 = lengthToKiloMeters(InvTimeToHours(velo_2));

        print("vel1 = {}km/h\n", velo_1);
        print("vel2 = {}m/s\n", velo_2);
        print("vel3 = {}km/h  (should equal to vel1)\n", velo_3);
    }

    return 0;
}









 