#include <string>
#include <array>

#define NMEAS 12

namespace hops 
{

    class MHO_Unit 
    {
    public:

        MHO_Unit(const std::string& unit);
        virtual ~MHO_Unit();
        
        //setter and getter for string representation
        void SetUnitString(const std::string unit){ Parse(unit); };


        // operator overloads for multiplication and division
        MHO_Unit operator*(const MHO_Unit& other) const;
        MHO_Unit operator/(const MHO_Unit& other) const;
        
        // operator overloads for compound assignment
        MHO_Unit& operator*=(const MHO_Unit& other);
        MHO_Unit& operator/=(const MHO_Unit& other);
        
        //raise the unit to an integer power 
        void RaiseToPower(int power);
        
        //invert the unit:
        void Invert();

        // equality operator
        bool operator==(const MHO_Unit& other) const;
        
        //assignment operator
        MHO_Unit& operator=(const MHO_Unit& other);

    private:
        
        std::string fStringRep;
        std::array<int, NMEAS> fExp; 

        // Parse() takes a string and determines the appropriate
        // unit exponents, and sets them in fExp
    
        virtual void Parse(const std::string& repl); 

    };

}
