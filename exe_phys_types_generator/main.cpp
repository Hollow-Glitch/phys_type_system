
#include <fmt/core.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>


//    m - meter    - Length
//    s - second   - Time
//   kg - kilogram - mass
//    A - Ampere   - Current
//    K - Kelvin   - Temperature
// mole - Mole     - ammount of substance
//   cd - candela  - Luminous intensity
// 
// m   s   kg   A   K   mole   cd  PER
// 6 * 6 *  6 * 6 * 6 *    6  * 6        *  6^7
// 
// m^-3  m^-2  m^-1  m^0  m^1  m^2  m^3
// ^----------------------------------^ = 3 + 1 + 3 = 7
// 
//
// For dynamics we need at least the following 3: m, s, kg; and for each of them we have 7 
// 
// Combinatorics then says that we have: 7^3 = 343; types.
// 
// For each type, we want to describe what will be its interaction with every other type in the sys.
// i.e.  outputType name(leftType varL, rightType varR)
// the number of input type combinations uniquely define the function, 
// so we have: 343^2 = 117 649; functions for one operation.
//
// All of this means that
//   num_of_types    = 7^3   =       343
//   num_of_div_ops  = 343^2 =   117 649
//   num_of_mult_ops = 343^2 =   117 649
//   num_of_add_ops  = 343   =       343
//   num_of_sub_ops  = 343   =       343
//                             + -------
//                               236 327


std::string create_compound_name(
    const std::vector<std::string>& units, 
    const std::vector<int>& exponents
) {
    using namespace std;
    assert(units.size() == exponents.size() );

    string out{};

    //>> "preprocessing"
    bool hasUnits = false;
    bool hasNegativeExponents = false;
    for (int i = 0; i < units.size(); i++) {
        if (exponents[i] != 0) {
            hasUnits = true;
        }
        if (exponents[i] < 0) { 
            hasNegativeExponents = true;
        }
    }

    //>> special case for "unit-less" numbers
    if ( ! hasUnits) {
        return "Number";
    }

    //>> units with positive exponents
    for (int i = 0; i < units.size(); i++) {
        auto exp = exponents[i];
        const string& unit_name = units[i];

        if (0 < exp) {
            out.append(unit_name);
        }
        if (1 < exp) {
            out.append(to_string(exp) );
        }
    }

    if (hasNegativeExponents) {
        //>> separator
        out.append("Per");

        //>> units with negative exonents
        for (int i = 0; i < units.size(); i++) {
            auto exp = exponents[i];
            const string& unit_name = units[i];

            if (exp < 0) {
                out.append(unit_name);
            }
            if (exp < -1) {
                out.append(to_string( - exp) );
            }
        }
    }

    return out;
}


std::vector<int> add_vectors(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    std::vector<int> copy{lhs};

    for (int i = 0; i < lhs.size(); i++) {
        copy[i] = lhs[i] + rhs[i];
    }

    return copy;
}


std::vector<int> sub_vectors(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    std::vector<int> copy{lhs};

    for (int i = 0; i < lhs.size(); i++) {
        copy[i] = lhs[i] - rhs[i];
    }

    return copy;
}


struct UnitExponents {
    std::vector<int> exp;
};

struct UnitFunction{
    UnitExponents return_type;
    // UnitExponents lhs; // needed?
    UnitExponents rhs;
};

struct CompUnit { // Compound unit i.e. combination of: kg, m, s, K, ...
    // std::vector<int> exponents;
    UnitExponents  exponents;

    UnitFunction addition_operators;
    UnitFunction subtraction_operators;

    std::vector<UnitFunction> div_operators;
    std::vector<UnitFunction> mult_operators;
};


int main() {
    using namespace std;

    vector<string> units {
        "Meter", 
        "Kilogram", 
        "Second"
    };
    //int max_exp =  1;
    //int min_exp = -1;
    // or
    int max_exp =  3;
    int min_exp = -3;
    string repr_var_name = "repr";
    string header_guard_symbol = "GEN_UNITS";

    // int exponents = max_exp + (- min_exp) + 1; // +1 for :  (...)^0  =  1  =  i.e. no unit

    {   //
        std::vector<int> exponents { -1, 2, -1 };
        string out = create_compound_name(units, exponents);
        fmt::print("exponents= {{{}, {}, {}}}\n", exponents[0], exponents[1], exponents[2]);
        fmt::print("out = {}\n", out);
    }

    {   //
        vector<CompUnit> gen_comp_units{};

        fmt::print("Exponent sequence iteratively\n");
        for (int i = min_exp; i <= max_exp; i++) {
            for (int j = min_exp; j <= max_exp; j++) {
                for (int k = min_exp; k <= max_exp; k++) {
                    string comp_unit_name = {};
                    UnitExponents unit_exponents{vector{i,j,k}};

                    //string out = create_compound_name(units, vector{i,j,k});
                    // this_unit.name = out;
                    //
                    //<< replaced with non-string based solution
                    CompUnit this_unit{};
                    this_unit.exponents.exp = vector{i,j,k};

                    //>> add addition operator
                    this_unit.addition_operators = UnitFunction{
                        .return_type = unit_exponents.exp,
                        .rhs = unit_exponents.exp
                    };

                    //>> add subtraction operator
                    this_unit.addition_operators = UnitFunction{
                        .return_type = unit_exponents.exp,
                        .rhs = unit_exponents.exp
                    };

                    //>> add division operators
                    for (int b = min_exp; b <= max_exp; b++) {
                        for (int n = min_exp; n <= max_exp; n++) {
                            for (int m = min_exp; m <= max_exp; m++) {
                                auto rhs_exponents = vector{b,n,m};
                                auto return_exponents = sub_vectors(unit_exponents.exp, rhs_exponents);

                                //>> Skip generating a function if the exponents of its return unit would be out
                                //   of the predefined min_exp and max_exp bounds.
                                bool skip_this_func = false;
                                for (int ret_exp_idx = 0; ret_exp_idx < return_exponents.size(); ret_exp_idx++) {
                                    if (return_exponents[ret_exp_idx] < min_exp || max_exp < return_exponents[ret_exp_idx]) {
                                        skip_this_func = true;
                                        break;
                                    }
                                }
                                if (skip_this_func) {
                                    continue;
                                }

                                this_unit.div_operators.push_back(
                                    UnitFunction{
                                        .return_type = return_exponents,
                                        .rhs = rhs_exponents
                                    }
                                );
                            }
                        }
                    }

                    //>> add multiplication operators
                    for (int b = min_exp; b <= max_exp; b++) {
                        for (int n = min_exp; n <= max_exp; n++) {
                            for (int m = min_exp; m <= max_exp; m++) {
                                auto rhs_exponents = vector{b,n,m};
                                auto return_exponents = add_vectors(unit_exponents.exp, rhs_exponents);

                                //>> Skip generating a function if the exponents of its return unit would be out
                                //   of the predefined min_exp and max_exp bounds.
                                bool skip_this_func = false;
                                for (int ret_exp_idx = 0; ret_exp_idx < return_exponents.size(); ret_exp_idx++) {
                                    if (return_exponents[ret_exp_idx] < min_exp || max_exp < return_exponents[ret_exp_idx]) {
                                        skip_this_func = true;
                                        break;
                                    }
                                }
                                if (skip_this_func) {
                                    continue;
                                }

                                this_unit.mult_operators.push_back(
                                    UnitFunction{
                                        .return_type = return_exponents,
                                        .rhs = rhs_exponents
                                    }
                                );
                            }
                        }
                    }

                    gen_comp_units.push_back(this_unit);
                    
                    //fmt::print("\n");
                    //fmt::print("exponents= {{{}, {}, {}}}\n", i, j, k);
                    //fmt::print("out = {}\n", out);
                }
            }
        }

        // ============================================================
        // generate header file content
        // ============================
        string header_file_content{};

        // #ifndef GEN_UNITS
        // #define GEN_UNITS
        // 
        // #define COMPILE_AWAY false
        // 
        // #if COMPILE_AWAY == true
        // typedef double Meter;
        // typedef double Second;
        // typedef double MeterPerSecond;
        // typedef double SecondPerMeter;
        // #else
        header_file_content.append(fmt::format("#ifndef {}\n", header_guard_symbol) );
        header_file_content.append(fmt::format("#define {}\n\n", header_guard_symbol) );
        header_file_content.append("#define COMPILE_AWAY false\n");

        header_file_content.append("#if COMPILE_AWAY == true\n");
            //>> add typedef-s
            for (CompUnit& comp_unit : gen_comp_units) {
                vector<int>& exps = comp_unit.exponents.exp;
                header_file_content.append(
                    fmt::format("typedef double {};\n", create_compound_name(units, exps) ) 
                );
            }
            header_file_content.append("\n\n");
        header_file_content.append("#else\n");

        //>> add forward declarations
        header_file_content.append("// Forward declarations\n");
        for (CompUnit& comp_unit : gen_comp_units) {
            vector<int>& exps = comp_unit.exponents.exp;
            header_file_content.append(
                fmt::format("struct {};\n", create_compound_name(units, exps) ) 
            );
        }
        header_file_content.append("\n\n");

        //>> add declarations
        for (CompUnit& comp_unit : gen_comp_units) {
            vector<int>& exps = comp_unit.exponents.exp;
            if (comp_unit.exponents.exp.size() < 3) {
                assert(false);
            }
            fmt::print("{:>2} {:>2} {:>2}\n", exps[0], exps[1], exps[2]);

            header_file_content.append(
                fmt::format("struct {} {{\n", create_compound_name(units, exps) ) 
            );

            header_file_content.append(
                fmt::format("    double {};\n\n", repr_var_name)
            );

            //>> add addition operator
            header_file_content.append(
                fmt::format("    {} operator+(const {}& rhs);\n", 
                    create_compound_name(units, exps),
                    create_compound_name(units, exps)
                )
            );

            //>> add subtraction operator
            header_file_content.append(
                fmt::format("    {} operator-(const {}& rhs);\n", 
                    create_compound_name(units, exps),
                    create_compound_name(units, exps)
                )
            );

            //>> add multiplication operators
            for (UnitFunction& mult_op : comp_unit.mult_operators) {
                header_file_content.append(
                    fmt::format("    {} operator*(const {}& rhs);\n", 
                        create_compound_name(units, mult_op.return_type.exp),
                        create_compound_name(units, mult_op.rhs.exp)
                    )
                );
            }

            //>> add division operators
            for (UnitFunction& div_op : comp_unit.div_operators) {
                header_file_content.append(
                    fmt::format("    {} operator/(const {}& rhs);\n", 
                        create_compound_name(units, div_op.return_type.exp),
                        create_compound_name(units, div_op.rhs.exp)
                    )
                );
            }

            header_file_content.append(
                fmt::format("}};\n")
            );
            header_file_content.append("\n\n");
        }
        header_file_content.append("#endif\n");
        header_file_content.append("#endif\n");

        // ============================================================
        // generate source file content
        // ============================
        string source_file_content{};

        source_file_content.append("#include \"gen_units.hpp\"\n");

        for (CompUnit& comp_unit : gen_comp_units) {
            vector<int>& exps = comp_unit.exponents.exp;

            //>> add addition operator
            source_file_content.append(
                fmt::format("    {} {}::operator+(const {}& rhs) {{ return {} {{ {} + rhs.{} }}; }}\n", 
                    create_compound_name(units, exps),
                    create_compound_name(units, exps),
                    create_compound_name(units, exps),
                    create_compound_name(units, exps),
                    repr_var_name,
                    repr_var_name
                )
            );

            //>> add subtraction operator
            source_file_content.append(
                fmt::format("    {} {}::operator-(const {}& rhs) {{ return {} {{ {} - rhs.{} }}; }}\n", 
                    create_compound_name(units, exps),
                    create_compound_name(units, exps),
                    create_compound_name(units, exps),
                    create_compound_name(units, exps),
                    repr_var_name,
                    repr_var_name
                )
            );

            //>> add multiplication operators
            for (UnitFunction& mult_op : comp_unit.mult_operators) {
                source_file_content.append(
                    fmt::format("    {} {}::operator*(const {}& rhs) {{ return {} {{ {} * rhs.{} }}; }} \n", 
                        create_compound_name(units, mult_op.return_type.exp),
                        create_compound_name(units, exps),
                        create_compound_name(units, mult_op.rhs.exp),
                        create_compound_name(units, mult_op.return_type.exp),
                        repr_var_name,
                        repr_var_name
                    )
                );
            }

            //>> add division operators
            for (UnitFunction& div_op : comp_unit.div_operators) {
                source_file_content.append(
                    fmt::format("    {} {}::operator/(const {}& rhs) {{ return {} {{ {} / rhs.{} }}; }} \n", 
                        create_compound_name(units, div_op.return_type.exp),
                        create_compound_name(units, exps),
                        create_compound_name(units, div_op.rhs.exp),
                        create_compound_name(units, div_op.return_type.exp),
                        repr_var_name,
                        repr_var_name
                    )
                );
            }

        }

        fmt::print("{}\n", header_file_content);
        fmt::print("{}\n", source_file_content);


        std::string vertex_code;
        std::string fragment_code;
        std::ofstream header_file;
        std::ofstream source_file;
        // ensure ifstream objects can throw exceptions:
        header_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        source_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            header_file.open("gen_units.hpp");
            header_file << header_file_content;
            header_file.close();

            source_file.open("gen_units.cpp");
            source_file << source_file_content;
            source_file.close();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        fmt::print("!!!!\n");
        fmt::print("!!!!  ==== YOU MIGHT HAVE TO RUN THIS PROGRAM OUT OF VIS-STUD TO GET THE FILES ====\n");
        fmt::print("!!!!\n");
    }

}


