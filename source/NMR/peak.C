// $Id: peak.C,v 1.5 2000/09/21 23:05:12 amoll Exp $

#include<BALL/NMR/peak.h>
#include<BALL/MATHS/common.h>

namespace BALL
{

	Peak1D::Peak1D()
		: value_(0),
			width_(0.1),
			height_(1),
			atom_(0)
	{
	}

	Peak1D::Peak1D(const Peak1D& peak)
		: value_(peak.value_),
			width_(peak.width_),
			height_(peak.height_),
			atom_(peak.atom_)
	{
	}

	Peak1D::~Peak1D()
	{
	}

	float Peak1D::getValue() const
	{
		return value_;
	}

	float Peak1D::getWidth() const
	{
		return width_;
	}

	float Peak1D::getHeight() const
	{
		return height_;
	}

	const Atom* Peak1D::getAtom() const
	{
		return atom_;
	}

	void Peak1D::setValue(float value)
	{
		value_ = value;
	}

	void Peak1D::setWidth(float value)
	{
		width_ = value;
	}

	void Peak1D::setAtom(const Atom* atom)
	{
		atom_ = atom;
	}

	void Peak1D::setHeight(float value)
	{
		height_ = value;
	}

	void Peak1D::operator = (const Peak1D& peak) 
	{
		value_ = peak.value_;
		width_ = peak.width_;
		height_ = peak.height_;
		atom_ = peak.atom_;
	}

	bool Peak1D::operator == (const Peak1D& peak) const
	{
		return Maths::isEqual(value_, peak.value_);
	}

	bool Peak1D::operator < (const Peak1D& peak) const
	{
		return Maths::isLess(value_, peak.value_);
	}

	ostream& operator << (ostream& os, const Peak1D& peak)
	{
		return os << peak.getValue();
	}

}	// namespace BALL
