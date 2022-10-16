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
#include "sit_uni_4.hpp"


// sit::m_per_s_t my_avg_speed(sit::m_t d, sit::s_t t)
// {
//     return d / t;
// }


int main() {
    using namespace sit;
    using namespace fmt;

    {
        Metre_t    dist{10};
        Kilogram_t mass{ 5};

        KiloMetre_t  dist_passed{100};
        Hour_t      time_it_took{  1};

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
        auto distance = KiloMetre_t(90);
        auto dist2 = distance * distance;
        auto time = Hour_t(2);
        fmt::print("{} km^2\n", dist2.repr);
        // fmt::print("distance / time = {} km/h\n", sit::ter(distance / time) );
        // fmt::print("distance / time = {}  m/s\n", sit::ter(sit::toBase(distance / time)) );
    }

    return 0;
}









 