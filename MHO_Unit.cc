#include <string>
#include <array>
#include <iostream>
#include "MHO_Unit.hh"
#include "read_units.lex.h"
#include "read_units.tab.h"


namespace hops 
{
    
    MHO_Unit::MHO_Unit() : fStringRep("") {
        for (int mu=0; mu<NMEAS; mu++) this->fExp[mu] = 0;
    }

    MHO_Unit::MHO_Unit(const std::string& unit) : fExp{} {
        MHO_Unit::Parse(unit);
    }
    
    //
    // Operator overloads for multiplication, division, and exponentiation
    //
    MHO_Unit MHO_Unit::operator*(const MHO_Unit& other) const {
        MHO_Unit unit;
        for (int mu=0; mu<NMEAS; mu++)
            unit.fExp[mu] = this->fExp[mu] + other.fExp[mu];
        return unit;
    }

    MHO_Unit MHO_Unit::operator/(const MHO_Unit& other) const {
        MHO_Unit unit;
        for (int mu=0; mu<NMEAS; mu++)
            unit.fExp[mu] = this->fExp[mu] - other.fExp[mu];
        return unit;
    }

    //
    // Operator overloads for multiplication by a string literal
    //
    // <unit> * <str>: class method
    //
    MHO_Unit MHO_Unit::operator*(const std::string& other) const {
        MHO_Unit unit;
        MHO_Unit other_unit(other);
        for (int mu=0; mu<NMEAS; mu++)
            unit.fExp[mu] = this->fExp[mu] + other_unit.fExp[mu];
        return unit;
    }
    
    // <str> * <unit>: friend function
    MHO_Unit operator*(const std::string& lhs, const MHO_Unit& rhs) {
        MHO_Unit unit;
        MHO_Unit lhs_unit(lhs);
        for (int mu=0; mu<NMEAS; mu++)
            unit.fExp[mu] = lhs_unit.fExp[mu] + rhs.fExp[mu];
        return unit;
    }
        
    //
    // Operator overloads for compound assignment
    //
    MHO_Unit& MHO_Unit::operator*=(const MHO_Unit& other) {
        for (int mu=0; mu<NMEAS; mu++)
            this->fExp[mu] += other.fExp[mu];
        return *this;
    }
    
    MHO_Unit& MHO_Unit::operator/=(const MHO_Unit& other) {
        for (int mu=0; mu<NMEAS; mu++)
            this->fExp[mu] -= other.fExp[mu];
        return *this;
    }
    
    //  
    // Raise the unit to an integer power
    //
    void MHO_Unit::RaiseToPower(int power) {
        for (int mu=0; mu<NMEAS; mu++)
            this->fExp[mu] = power*this->fExp[mu];
    }
    
    //  
    // Yet another raise the unit to an integer power: unit^power
    // with its compound counterpart
    //
    MHO_Unit MHO_Unit::operator^(int power) {
        MHO_Unit unit;
        for (int mu=0; mu<NMEAS; mu++)
            unit.fExp[mu] = power * this->fExp[mu];
        return unit;        
    }

    MHO_Unit MHO_Unit::operator^=(int power) {
        for (int mu=0; mu<NMEAS; mu++)
            this->fExp[mu] *= power;
        return *this;        
    }



    //
    // Invert the unit:
    //
    void  MHO_Unit::Invert() {
        for (int mu=0; mu<NMEAS; mu++)
            this->fExp[mu] = -this->fExp[mu];
    }

    //
    // Equality operator
    //
    bool MHO_Unit::operator==(const MHO_Unit& other) const {
        return (bool *) (this->fExp == other.fExp);
    }
    
    //
    // Inequality operator
    //
    bool MHO_Unit::operator!=(const MHO_Unit& other) const {
        return (bool *) (this->fExp != other.fExp);
    }
    
    //    
    // Assignment operator
    //
    MHO_Unit& MHO_Unit::operator=(const MHO_Unit& other) {
        this->fExp = other.fExp;
        return *this;
    }

    
    //
    // Parse() takes a string and determines the appropriate
    // unit exponents, and sets them in fExp
    //
    void MHO_Unit::Parse(const std::string& repl) {
        char *meas_exp = (char *) repl.std::string::c_str();
        YY_BUFFER_STATE buf;
        expr_list *el; 
        meas_pow mpow;  // A structure with int exp[NMEAS]; reflecting fExp.
        int mu, perr;

        buf = yy_scan_string(meas_exp);
        
        perr = yyparse(&el);  /* Sets el to point at the linked list */

        if (perr == 0) {
            // Convert list of measures to array of their powers
            explst_to_arr_and_free(el, &mpow); 
            for (mu=0; mu<NMEAS; mu++) fExp[mu] = mpow.exp[mu];
        }
        
        yy_delete_buffer(buf);
        
    }       // End MHO_Unit::Parse(const std::string& repl)

    
    //
    // Construct a human-readable string from the base unit exponents
    //
    // In the loop over the NMEAS available measurement units, the exponent
    // array member, fExp, is checked for non-zero exponent. The its position,
    // mu, is used to fetch the corresponding unit from the meas_tab where
    // the units are stored as c-style char arrays. The *char unit is converted
    // into std::string meastr, which is appended to the meas_expr string.
    // The units in meas_expr are altered with the asterisk.
    //
    std::string MHO_Unit::ConstructString() const {
        std::string mexpr; // Measure expression string to work on
        std::string meas_expr; // Measure expression string to be returned
        for (int mu=0; mu<NMEAS; mu++) {
            if (fExp[mu]) {
                // Get a measurement unit from table 
                std::string meastr(meas_tab[mu]); 
                mexpr.append(meastr);
                if (fExp[mu] != 1) { // Only show non-unity exponents
                    mexpr.append("^");
                    mexpr.append(std::to_string(fExp[mu]));
                }
                mexpr.append(" * ");
            }
            // Cut off the surplus " * " tail
            int meas_len = mexpr.length();
            if (meas_len > 3)
                meas_expr = mexpr.substr(0, meas_len-3);
        }
        return meas_expr;
    } // ConstructString()
} // namespace hops


    
//
// =================  M A I N  ======================
//

using namespace hops;

int main(void) {

    char const accel_expr[] = "m/s^2";
    char const force_expr[] = "kg*m/s^2";
    char const energy_expr[] = "kg*m^2/s^2";
    
    char const meas_expr1[] = "m * ((kg^2*s^-3/A)^-5 * K^5/cd/" \
        "(mol*Hz)^3*s)^2 * rad * Jy^(7 + 2*(4 - 6))";
    
    char const meas_expr2[] = " A*kg*(m^-1*s^-2)^3";

    char const meas_expr3[] = " A * kg *(m^-1*s^-2)^3  ";

    printf("MHO_Unit Declarations:\n");
    printf("\nMHO_Unit acc: '%s'\n", accel_expr);
    MHO_Unit acc(accel_expr);
    
    printf("\nMHO_Unit F: '%s'\n", force_expr);
    MHO_Unit F(force_expr);
    
    printf("\nMHO_Unit E: '%s'\n", energy_expr);
    MHO_Unit E;                E.SetUnitString(energy_expr);
    
    printf("\nMHO_Unit mass: '%s'\n", "kg");
    MHO_Unit mass;          mass.SetUnitString("kg");

    MHO_Unit u0;
    
    printf("\nMHO_Unit u1: '%s'\n", meas_expr1);
    MHO_Unit u1(meas_expr1);
    
    printf("\nMHO_Unit u2: '%s'\n", meas_expr2);
    MHO_Unit u2(meas_expr2);

    printf("\nMHO_Unit u3: '%s'\n", meas_expr3);
    MHO_Unit u3(meas_expr3);
    
    printf("\nMHO_Unit u4: '%s'\n", meas_expr2);
    MHO_Unit u4(meas_expr2);
    

    std::cout << "Source measure expression 1:\n";
    std::cout << meas_expr1 << std::endl << std::endl;

    std::cout << "MHO_Unit u1(meas_expr1); u1.GetUnitString():\n";
    std::cout << u1.GetUnitString() << std::endl << std::endl;
    
    std::cout << "u0.GetUnitString() -- empty expression.\n";
    std::cout << u0.GetUnitString() << std::endl;
    
    std::cout << "Source measure expression 2:\n";
    std::cout << meas_expr2 << std::endl << std::endl << std::endl;
    
    std::cout << "MHO_Unit u1(meas_expr2); u2.GetUnitString():\n";
    std::cout << u2.GetUnitString() << std::endl << std::endl;

    std::cout << "u1/u2 = " << std::endl;
    std::cout << (u1/u2).GetUnitString() << std::endl << std::endl;
    
    std::cout << "u2/u1) = " << std::endl;
    std::cout << (u2/u1).GetUnitString() << std::endl << std::endl;
    
    std::cout << "u1*u2 = " << std::endl;
    std::cout << (u1*u2).GetUnitString() << std::endl << std::endl;

    
    std::cout << "u2 == u3 = ";
    std::cout << (u2 == u3 ? "True":"False") << std::endl << std::endl;
    
    std::cout << "u2 != u3 = ";
    std::cout << (u2 != u3 ? "True":"False") << std::endl << std::endl;
    
    std::cout << "u1^(-1) = " << std::endl;
    std::cout << (u1^(-1)).GetUnitString() << std::endl << std::endl;

    u1.Invert();
    std::cout << "u1.Invert(): " << std::endl;
    std::cout << u1.GetUnitString() << std::endl << std::endl;

    u1.RaiseToPower(-1);
    std::cout << "u1.RaiseToPower(-1): " << std::endl;
    std::cout << u1.GetUnitString() << std::endl
              << std::endl;
    
    std::cout << "F = m*a: (mass * acc).GetUnitString():" << std::endl;
    std::cout << (mass * acc).GetUnitString() << std::endl << std::endl;

    std::cout << "E/F = s: (E / F).GetUnitString():" << std::endl;
    std::cout << (E / F).GetUnitString() << std::endl << std::endl;

    u0 = mass*acc; // u0 is F = m * a.
    std::cout << "F = m*a; : (mass * acc).GetUnitString():" << std::endl;
    std::cout << u0.GetUnitString() << std::endl << std::endl;

    F ^= -3;
    std::cout << "F ^= -3;  : F.GetUnitString():" << std::endl;
    std::cout << F.GetUnitString() << std::endl << std::endl;

    u0 = u4 * "kg^-1 * (m^-1 * s^-2)^(-3) / m^2";
    std::cout << "u4 = ";
    std::cout << u4.GetUnitString() << std::endl;
    std::cout << "u0 = u4 * \"kg^-1 * (m^-1 * s^-2)^(-3) / m^2\";" << std::endl;
    std::cout << "u0 = ";
    std::cout << u0.GetUnitString() << std::endl << std::endl;

    u0 = "kg" * acc; // u0 is F = m * a.
    std::cout << "acc = ";
    std::cout << acc.GetUnitString() << std::endl;
    std::cout << "u0 = \"kg\" * acc = ";
    std::cout << u0.GetUnitString() << std::endl << std::endl;

   
    u0 = mass * "m/s^2"; // u0 is F = m * a.acc
    std::cout << "mass = ";
    std::cout << mass.GetUnitString() << std::endl;
    std::cout << "u0 = mass * \"m/s^2\" = ";
    std::cout << u0.GetUnitString() << std::endl << std::endl;

   

    
    
    return 0;            
}
