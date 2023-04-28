#include <string>
#include <array>
#include "MHO_Unit.hh"
#include "read_units.h"
#include "read_units.lex.h"
#include "read_units.tab.h"


namespace hops 
{

    MHO_Unit::MHO_Unit(const std::string& unit) {
        MHO_Unit::Parse(unit);
    }
    // operator overloads for multiplication and division
    MHO_Unit operator*(const MHO_Unit& other) const {
        // for (int mu=0; mu<NMEAS; mu++)
        //     fExp[mu] = power*fExp[mu];

        // int newValue = this->value * other.value;
        // return MyClass(newValue);
    }
    MHO_Unit operator/(const MHO_Unit& other) const;

    //raise the unit to an integer power 
    void MHO_Unit::RaiseToPower(int power) {
        for (int mu=0; mu<NMEAS; mu++)
            fExp[mu] = power*fExp[mu];
    }

    //invert the unit:
    void  MHO_Unit::Invert() {
        for (int mu=0; mu<NMEAS; mu++)
            fExp[mu] = -fExp[mu];
    }


    // Parse() takes a string and determines the appropriate
    // unit exponents, and sets them in fExp
    
    void  MHO_Unit::Parse(const std::string& repl) {
        char *meas_exp = (char *) repl.std::string::c_str();
        YY_BUFFER_STATE buf;
        expr_list *el; 
        meas_pow mpow;
        int mu, perr;

        buf = yy_scan_string(meas_exp);
        perr = yyparse(&el);
        if (perr == 0) { 
            explst_to_arr(el, &mpow);
            yy_delete_buffer(buf);

            /* Print source and reduced expressions */
            printf("Source:\n \"%s\"\n\n", meas_exp);
            printf("Reduced to:\n");
            for (mu=0; mu<NMEAS; mu++) {
                if (mpow.exp[mu]) {
                    printf("%s^%d", meas_tab[mu], mpow.exp[mu]);
                    if (mu < NMEAS-1) printf(" * ");
                    else printf("\n");
                }
            }
        
            for (mu=0; mu<NMEAS; mu++) fExp[mu] = mpow.exp[mu];
        }
    }

} // namespace hops
