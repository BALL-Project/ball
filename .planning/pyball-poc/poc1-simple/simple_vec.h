// Self-contained C++ class mimicking BALL's TVector3 shape so we can
// validate autowrap end-to-end without depending on BALL.
#pragma once
#include <string>
#include <stdexcept>

namespace pocns {

class SimpleVec
{
public:
    SimpleVec();
    SimpleVec(double x, double y, double z);
    SimpleVec(const SimpleVec& other);
    ~SimpleVec();

    double getX() const;
    double getY() const;
    double getZ() const;
    void   setX(double v);
    void   setY(double v);
    void   setZ(double v);

    double length() const;
    SimpleVec normalize() const;            // returns a copy
    SimpleVec& negate();                    // returns *this

    SimpleVec operator + (const SimpleVec& o) const;
    SimpleVec operator - (const SimpleVec& o) const;
    double    operator * (const SimpleVec& o) const;   // dot product
    SimpleVec operator * (double s) const;             // scale

    bool operator == (const SimpleVec& o) const;
    bool operator != (const SimpleVec& o) const;

    SimpleVec& operator += (const SimpleVec& o);
    SimpleVec& operator -= (const SimpleVec& o);

    // Methods that throw — to validate exception propagation
    SimpleVec divideBy(double s) const;     // throws std::invalid_argument on 0

    std::string repr() const;

private:
    double x_, y_, z_;
};

} // namespace pocns
