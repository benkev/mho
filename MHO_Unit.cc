#include <string>
#include <array>
#include <iostream>
#include "MHO_Unit.hh"
#include "read_units.lex.h"
#include "read_units.tab.h"


namespace hops 
{

    MHO_Unit::MHO_Unit(const std::string& unit) : fExp{} {
        MHO_Unit::Parse(unit);
    }

    //
    // Operator overloads for multiplication and division
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
    //
    MHO_Unit MHO_Unit::operator^(int power) {
        MHO_Unit unit;
        for (int mu=0; mu<NMEAS; mu++)
            unit.fExp[mu] = power*this->fExp[mu];
        return unit;        
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
        perr = yyparse(&el);
        if (perr == 0) { 
            explst_to_arr(el, &mpow);
            yy_delete_buffer(buf);

            // /* Print source and reduced expressions */
            // printf("Source:\n \"%s\"\n\n", meas_exp);
            // printf("Reduced to:\n");
            // for (mu=0; mu<NMEAS; mu++) {
            //     if (mpow.exp[mu]) {
            //         printf("%s^%d", meas_tab[mu], mpow.exp[mu]);
            //         if (mu < NMEAS-1) printf(" * ");
            //         else printf("\n");
            //     }
            // }
        
            for (mu=0; mu<NMEAS; mu++) fExp[mu] = mpow.exp[mu];
        }
    } // void MHO_Unit::Parse(const std::string& repl)

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
        std::string meas_expr; // Measure expression string to be returned
        for (int mu=0; mu<NMEAS; mu++) {
            if (fExp[mu]) {
                // Get a measurement unit from table 
                std::string meastr(meas_tab[mu]); 
                meas_expr.append(meastr);
                if (fExp[mu] != 1) { // Only show non-unity exponents
                    meas_expr.append("^");
                    meas_expr.append(std::to_string(fExp[mu]));
                }
                if (mu < NMEAS-1) meas_expr.append(" * ");
            }
        }
        return meas_expr;
    } // ConstructString()

    //
    // =================  M A I N  ======================
    //
    

} // namespace hops
using namespace hops;

int main(void) {

    char const meas_expr1[] = "m*((kg^2*s^-3/A)^-5*K^5/cd/" \
        "(mol*Hz)^3*s)^2*rad*Jy^(7+2*(4-6))";
    
    char const meas_expr2[] = " A*kg*(m^-1*s^-2)^3";

    char const meas_expr3[] = " A * kg *(m^-1*s^-2)^3  ";

    MHO_Unit Unit0;
    MHO_Unit Unit1(meas_expr1);
    MHO_Unit Unit2(meas_expr2);

    std::cout << "Source measure expression 1:\n";
    std::cout << meas_expr1 << std::endl;
    std::cout << "\nUnit1.GetUnitString():\n";
    std::cout << Unit1.GetUnitString() << std::endl;
    std::cout << "\nUnit0.GetUnitString():\n";
    std::cout << Unit0.GetUnitString() << std::endl;
    std::cout << "\nUnit2.GetUnitString():\n";
    std::cout << "Source measure expression 2:\n";
    std::cout << meas_expr2 << std::endl;
    std::cout << Unit2.GetUnitString() << std::endl;
    
    return 0;            
}
