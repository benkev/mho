#include <string>
#include <array>

#define NMEAS 12

namespace hops 
{

    class MHO_Unit 
    {
    public:
        MHO_Unit() : fStringRep() { };
        MHO_Unit(const std::string& unit);
        virtual ~MHO_Unit() { };
        
        //setter and getter for string representation
        void SetUnitString(const std::string unit) { Parse(unit); };
        std::string GetUnitString() const { return ConstructString(); }


        // operator overloads for multiplication and division
        MHO_Unit operator*(const MHO_Unit& other) const;
        MHO_Unit operator/(const MHO_Unit& other) const;
        
        // operator overloads for compound assignment
        MHO_Unit& operator*=(const MHO_Unit& other);
        MHO_Unit& operator/=(const MHO_Unit& other);
        
        //raise the unit to an integer power 
        void RaiseToPower(int power);
        
        // Yet another raise the unit to an integer power: unit^power
        MHO_Unit operator^(int power);

        //invert the unit:
        void Invert();

        // equality operator
        bool operator==(const MHO_Unit& other) const;
        
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
