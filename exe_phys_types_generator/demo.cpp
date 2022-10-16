

// #include "myTypeIdea.hpp"
// #include "myTypeIdeaTestFuncs.hpp"
//<< replaced with generated one
#include "gen_units.cpp"

MeterPerSecond averageSpeed(Meter distance, Second time) {
    return distance / time;
}


int main () {
    double just_checking{15.0F};
    Meter distance{15.0F};
    Second time{6.0F};
    auto velocity = distance / time;
    //auto velocity_calc_by_func = averageSpeed(time, distance); //<<caught! yess :)
    // SecondPerMeter velocity_calc_by_func = averageSpeed(distance, time); // also caught :)
    MeterPerSecond velocity_calc_by_func = averageSpeed(distance, time);
    // MeterPerSecond dangerous_teritory = averageSpeed(0.6F, 0.5F); // caught!!!
    // auto insanity = averageSpeed(0.6F, 0.5F); // caught!!!

    auto distanceCube = distance * distance * distance;
    auto timedDistanceCube = distanceCube * Second(2);
    auto wrong = timedDistanceCube / velocity_calc_by_func;

    auto m_kg_s = Meter(5) * Kilogram(7) * Second(11);
    auto m_per_s = Meter(2) / Second(6);
    auto m_kg_s__per__m_per_s = m_kg_s / m_per_s;

    return 0;
}
