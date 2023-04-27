#include <string>
#include <array>
#include "MHO_Unit.hh"
#include "read_units.h"
#include "read_units.lex.h"
#include "read_units.tab.h"


namespace hops 
{

    MHO_Unit:: MHO_Unit(const std::string& unit) {
        MHO_Unit::Parse(unit);
    }


    // Parse() takes a string and determines the appropriate
    // unit exponents, and sets them in fExp
    
    void MHO_Unit::Parse(const std::string& repl) {
        char *meas_exp = (char *) repl.std::string::c_str();
        YY_BUFFER_STATE buf;
        expr_list *el; 
        meas_pow mpow;
        int mu, perr;

        buf = yy_scan_string(meas_exp);
        perr = yyparse(&el);
        if (perr == 0) { 
            explst_to_dims(el, &mpow);
            yy_delete_buffer(buf);

            /* Print source and reduced expressions */
            printf("Source:\n \"%s\"\n\n", meas_exp);
            printf("Reduced to:\n");
            for (mu=0; mu<NMEAS; mu++) {
                if (mpow.mea[mu]) {
                    printf("%s^%d", meas_tab[mu], mpow.dim[mu]);
                    if (mu < NMEAS-1) printf(" * ");
                    else printf("\n");
                }
            }
        
            for (mu=0; mu<NMEAS; mu++) fExp[mu] = mpow.mea[mu];
        }
    }

} // namespace hops
