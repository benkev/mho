#include <string>
#include <array>
#include "read_units.h"


namespace hops 
{

    class MHO_Unit 
    {
    public:
        MHO_Unit();
        MHO_Unit(const std::string& unit);
        virtual ~MHO_Unit() { };
        
        //setter and getter for string representation
        void SetUnitString(const std::string unit) { Parse(unit); };
        std::string GetUnitString() const { return ConstructString(); }

        //setter and getter for the unit exponrnts
        void SetUnitExp(const std::array<int, NMEAS> fExp);
        std::array<int, NMEAS> GetUnitExp() const { return fExp; }


        // operator overloads for multiplication and division
        MHO_Unit operator*(const MHO_Unit& other) const;
        MHO_Unit operator/(const MHO_Unit& other) const;
        
        //
        // Operator overloads for multiplication by a string literal
        //
        // <unit> * <str>: class method
        MHO_Unit operator*(const std::string& other) const;
        // <unit> *= <str>: class method (Compound assgnt)
        MHO_Unit& operator*=(const std::string& other);
        // <str> * <unit>: friend function        
        friend MHO_Unit operator*(const std::string& lhs, const MHO_Unit& rhs);
        
        // operator overloads for compound assignment
        MHO_Unit& operator*=(const MHO_Unit& other);
        MHO_Unit& operator/=(const MHO_Unit& other);
        
        //raise the unit to an integer power 
        void RaiseToPower(int power);
        
        // Yet another raise the unit to an integer power: unit^power
        MHO_Unit operator^(int power);
        MHO_Unit operator^=(int power);

        //invert the unit:
        void Invert();

        // equality operators
        bool operator==(const MHO_Unit& other) const;
        bool operator!=(const MHO_Unit& other) const;
        
        //assignment operator
        MHO_Unit& operator=(const MHO_Unit& other);

    private:
        
        std::string fStringRep;
        std::array<int, NMEAS> fExp;

        // Constructs a human readable string from the base unit exponents
        virtual std::string ConstructString() const;

        // Parse() takes a string and determines the appropriate
        // unit exponents, and sets them in fExp
    
        virtual void Parse(const std::string& repl); 

    };


}
