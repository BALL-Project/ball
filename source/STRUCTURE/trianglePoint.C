// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: trianglePoint.C,v 1.1.2.1 2003/01/07 13:22:08 anker Exp $

#include <BALL/STRUCTURE/triangleEdge.h>
#include <BALL/STRUCTURE/triangle.h>
#include <BALL/STRUCTURE/trianglePoint.h>

namespace BALL
{

	TrianglePoint::TrianglePoint()
		throw()
		:	GraphVertex< TrianglePoint,TriangleEdge,Triangle >(),
			point_(),
			normal_()
	{
	}


	TrianglePoint::TrianglePoint(const TrianglePoint& point, bool deep)
		throw()
		:	GraphVertex< TrianglePoint,TriangleEdge,Triangle >
				(point,deep),
			point_(point.point_),
			normal_(point.normal_)
	{
	}


	TrianglePoint::~TrianglePoint()
		throw()
	{
	}


	void TrianglePoint::set(const TrianglePoint& point, bool deep)
		throw()
	{
		if (this != &point)
		{
			GraphVertex< TrianglePoint,TriangleEdge,Triangle >::set
					(point,deep);
			point_.set(point.point_);
			normal_.set(point.normal_);
		}
	}


	TrianglePoint& TrianglePoint:: operator =
			(const TrianglePoint& point)
		throw()
	{
		if (this != &point)
		{
			GraphVertex< TrianglePoint,TriangleEdge,Triangle >::operator=
					(point);
			point_.set(point.point_);
			normal_.set(point.normal_);
		}
		return *this;
	}


	TVector3<double> TrianglePoint::getPoint() const
		throw()
	{
		return point_;
	}


	void TrianglePoint::setPoint(const TVector3<double>& point)
		throw()
	{
		point_ = point;
	}


	TVector3<double> TrianglePoint::getNormal() const
		throw()
	{
		return normal_;
	}


	void TrianglePoint::setNormal(const TVector3<double>& normal)
		throw(Exception::DivisionByZero)
	{
		normal_ = normal;
		normal_.normalize();
	}


	bool TrianglePoint::operator == (const TrianglePoint& point) const
		throw()
	{
		return (point_ == point.point_);
	}


	bool TrianglePoint::operator != (const TrianglePoint& point) const
		throw()
	{
		return (point_ != point.point_);
	}


	bool TrianglePoint::operator *= (const TrianglePoint& point) const
		throw()
	{
		return (point_ == point.point_);
	}


	std::ostream& operator << (std::ostream& s, const TrianglePoint& point)
	{
		s << "POINT";
		s << point.getIndex();
		s << "( " << point.getPoint() << " " << point.getNormal() << " {";
		TrianglePoint::ConstEdgeIterator e;
		for (e = point.beginEdge(); e != point.endEdge(); e++)
		{
			s << (*e)->getIndex() << " ";
		}
		s << "} [";
		TrianglePoint::ConstFaceIterator f;
		for (f = point.beginFace(); f != point.endFace(); f++)
		{
			s << (*f)->getIndex() << " ";
		}
		s << "] )";
		return s;
	}


}	// namespace BALL
