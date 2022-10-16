//>> Standard
#include <cassert>
#include <cmath>

#include <iostream>
#include <filesystem>
#include <string>
#include <memory>

//>> vcpkg
#include <fmt/core.h>

//>> my stuff
//import UnitsCheckout;
// #include "UnitsCheckout.cpp"

#include <algorithm>
#include <units/format.h>
#include <units/isq/si/international/length.h>
#include <units/isq/si/international/speed.h>
#include <units/isq/si/length.h>
#include <units/isq/si/speed.h>
#include <units/isq/si/time.h>
#include <units/quantity_io.h>
#include <iostream>

using namespace units::isq;


constexpr Speed auto avg_speed(Length auto d, Time auto t)
{
    return d / t;
}


void bip_bup_mp_units_new()
{
    using namespace units::isq::si::literals;
    using namespace units::isq::si::references;
    using namespace units::aliases::isq::si::international;

    constexpr Speed auto v1 = 110 * (km / h);
    constexpr Speed auto v2 = mi_per_h(70.);
    constexpr Speed auto v3 = avg_speed(220_q_km, 2_q_h);
    constexpr Speed auto v4 = avg_speed(si::length<si::international::mile>(140), si::time<si::hour>(2));
    constexpr Speed auto v5 = quantity_cast<si::speed<si::metre_per_second>>(v3);
    constexpr Speed auto v6 = quantity_cast<si::metre_per_second>(v4);
    constexpr Speed auto v7 = quantity_cast<int>(v6);
     
    // std::cout << v1 << '\n';                                  // 110 km/h
    // std::cout << v2 << '\n';                                  // 70 mi/h
    // std::cout << std::format("{}", v3) << '\n';               // 110 km/h
    // std::cout << std::format("{:*^14}", v4) << '\n';          // ***70 mi/h****
    // std::cout << std::format("{:%Q in %q}", v5) << '\n';      // 30.5556 in m/s
    // std::cout << std::format("{0:%Q} in {0:%q}", v6) << '\n'; // 31.2928 in m/s
    // std::cout << std::format("{:%Q}", v7) << '\n';            // 31
    //
    //<< produced build errors, hence trying out displaying with {fmt}
    fmt::print("{}\n", v1);
    fmt::print("{}\n", v2);
    fmt::print("{}\n", v3);
    fmt::print("{}\n", v4);
    fmt::print("{}\n", v5);
    fmt::print("{}\n", v6);
    fmt::print("{}\n", v7);

    constexpr auto velo       = 50 * (km / h);
    constexpr auto time_delta = 50 * s;
    constexpr auto distance   = 50 * m;
    //auto time_delta = s<>(50);
    //auto distance   = m<>(50);

    // auto asdf = avg_speed(velo, time_delta);
    auto hjkl = avg_speed(distance, time_delta);

    // fmt::print("avg_speed(velo, time_delta) = {}", avg_speed(velo, time_delta));
    fmt::print("avg_speed(velo, time_delta) = {}\n", avg_speed(distance, time_delta));
}


int main() {
    {   // mp-units
        bip_bup_mp_units_new();
    }

    return 0;
}









 