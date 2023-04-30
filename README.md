This "mho" directory is a standalone "sandbox" where the C++ class MHO_Unit is
being developed. This class is intended to be a part of the HOPS-4.0 project at

https://github.mit.edu/barrettj/hops-git

Specifically, the two files:

hops-git/source/cpp_src/Utilities/include/MHO_Unit.hh

hops-git/source/cpp_src/Utilities/src/MHO_Unit.cc

An objects of MHO_Unit class holds an expression of measurement units (let us
denote it as EMU or just emu). For example, define:

    MHO_Unit a("m/s^2");
    MHO_Unit m("kg");
    MHO_Unit F(); // Empty expression

The class defines algebraic operations "*" and "/" between the emus, as well as
exponentiation operation with an integer exponent "^". We can write:

    F = m*a;
    
and see the result:

    std::cout << F.GetUnitString() << std::endl;
--> m * s^-2 

The class also defines the logical operations: equality "==" and inequality "!=".
operations are also defined

Many more test examples are in the file MHO_Unit.cc, in main().

Eventually, we may include SI prefixes, like kilo, Mega, etc.

