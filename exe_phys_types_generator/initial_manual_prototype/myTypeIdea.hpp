
#ifndef myTypeIdea
#define myTypeIdea

#define COMPILE_AWAY false

#if COMPILE_AWAY == true
typedef double Meter;
typedef double Second;
typedef double MeterPerSecond;
typedef double SecondPerMeter;
#else

//================================================================================
//>> forward declarations
struct Meter;
struct Meter2;
struct Meter3;
struct PerMeter;
struct PerMeter2;
struct PerMeter3;

struct Second;
struct MeterPerSecond;
struct SecondPerMeter;


//================================================================================
//>> declarations
struct Meter {
    double repr;

    // Meter2 operator*(const Meter&  rhs) {
    //     return Meter2(repr * rhs.repr);
    // }
    Meter2 operator*(const Meter&  rhs);

    // Meter3 operator*(const Meter2& rhs) {
    //     return Meter3(repr * rhs.repr);
    // }
    Meter3 operator*(const Meter2& rhs);

    double operator/(const Meter&  rhs);
    MeterPerSecond operator/(const Second&  rhs);
};

struct Meter2 {
    double repr;

    // Meter3 operator*(const Meter& rhs) {
    //     return Meter3(repr * rhs.repr);
    // }
    Meter3 operator*(const Meter& rhs);
};

struct Meter3 {
    double repr;

    // Meter2 operator/(const Meter& rhs) {
    //     return Meter2{repr / rhs.repr};
    // }
    Meter2 operator/(const Meter& rhs);

    // Meter operator/(const Meter2& rhs) {
    //     return Meter{repr / rhs.repr};
    // }
    Meter operator/(const Meter2& rhs);
};



struct Second {
    double repr;

    double operator/(const Second& rhs);
    SecondPerMeter operator/(const Meter& rhs);
};

struct MeterPerSecond {
    double repr;
};

struct SecondPerMeter {
    double repr;
};


//================================================================================
//>> function implementations
double Meter::operator/(const Meter&  rhs) {
    return repr / rhs.repr;
}

Meter2 Meter::operator*(const Meter&  rhs) {
    return Meter2(repr * rhs.repr);
}

Meter3 Meter::operator*(const Meter2& rhs) {
    return Meter3(repr * rhs.repr);
}

Meter3 Meter2::operator*(const Meter& rhs) {
    return Meter3(repr * rhs.repr);
}

Meter2 Meter3::operator/(const Meter& rhs) {
    return Meter2{repr / rhs.repr};
}

Meter  Meter3::operator/(const Meter2& rhs) {
    return Meter{repr / rhs.repr};
}

double         Second::operator/(const Second& rhs) {
    return repr / rhs.repr;
}

MeterPerSecond  Meter::operator/(const Second& rhs) {
    return MeterPerSecond(repr / rhs.repr);
}

SecondPerMeter Second::operator/(const Meter&  rhs) {
    return SecondPerMeter(repr / rhs.repr);
}

#endif

#endif
