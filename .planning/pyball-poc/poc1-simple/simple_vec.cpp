#include "simple_vec.h"
#include <cmath>
#include <sstream>

namespace pocns {

SimpleVec::SimpleVec() : x_(0), y_(0), z_(0) {}
SimpleVec::SimpleVec(double x, double y, double z) : x_(x), y_(y), z_(z) {}
SimpleVec::SimpleVec(const SimpleVec& o) : x_(o.x_), y_(o.y_), z_(o.z_) {}
SimpleVec::~SimpleVec() {}

double SimpleVec::getX() const { return x_; }
double SimpleVec::getY() const { return y_; }
double SimpleVec::getZ() const { return z_; }
void   SimpleVec::setX(double v) { x_ = v; }
void   SimpleVec::setY(double v) { y_ = v; }
void   SimpleVec::setZ(double v) { z_ = v; }

double SimpleVec::length() const { return std::sqrt(x_*x_ + y_*y_ + z_*z_); }

SimpleVec SimpleVec::normalize() const {
    double l = length();
    if (l == 0.0) throw std::invalid_argument("zero-length normalize");
    return SimpleVec(x_/l, y_/l, z_/l);
}

SimpleVec& SimpleVec::negate() { x_=-x_; y_=-y_; z_=-z_; return *this; }

SimpleVec SimpleVec::operator+(const SimpleVec& o) const { return SimpleVec(x_+o.x_, y_+o.y_, z_+o.z_); }
SimpleVec SimpleVec::operator-(const SimpleVec& o) const { return SimpleVec(x_-o.x_, y_-o.y_, z_-o.z_); }
double    SimpleVec::operator*(const SimpleVec& o) const { return x_*o.x_ + y_*o.y_ + z_*o.z_; }
SimpleVec SimpleVec::operator*(double s) const { return SimpleVec(x_*s, y_*s, z_*s); }

bool SimpleVec::operator==(const SimpleVec& o) const { return x_==o.x_ && y_==o.y_ && z_==o.z_; }
bool SimpleVec::operator!=(const SimpleVec& o) const { return !(*this == o); }

SimpleVec& SimpleVec::operator+=(const SimpleVec& o) { x_+=o.x_; y_+=o.y_; z_+=o.z_; return *this; }
SimpleVec& SimpleVec::operator-=(const SimpleVec& o) { x_-=o.x_; y_-=o.y_; z_-=o.z_; return *this; }

SimpleVec SimpleVec::divideBy(double s) const {
    if (s == 0.0) throw std::invalid_argument("division by zero");
    return SimpleVec(x_/s, y_/s, z_/s);
}

std::string SimpleVec::repr() const {
    std::ostringstream oss;
    oss << "SimpleVec(" << x_ << ", " << y_ << ", " << z_ << ")";
    return oss.str();
}

} // namespace pocns
