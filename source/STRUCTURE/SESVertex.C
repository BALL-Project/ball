// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SESVertex.C,v 1.2.2.1 2003/01/07 13:22:00 anker Exp $

#include <BALL/STRUCTURE/SESEdge.h>
#include <BALL/STRUCTURE/SESFace.h>
#include <BALL/STRUCTURE/SESVertex.h>
#include <BALL/MATHS/vector3.h>


namespace BALL
{

	SESVertex::SESVertex()
		throw()
		: GraphVertex< SESVertex,SESEdge,SESFace >(),
			point_(),
			normal_(),
			atom_(0)
	{
	}


	SESVertex::SESVertex(const SESVertex& sesvertex, bool deep)
		throw()
		: GraphVertex< SESVertex,SESEdge,SESFace >(sesvertex,deep),
			point_(sesvertex.point_),
			normal_(sesvertex.normal_),
			atom_(sesvertex.atom_)
	{
	}


	SESVertex::SESVertex
			(const TVector3<double>&	point,
			 const TVector3<double>&	normal,
			 Index							atom,
			 Index							index)
		throw()
		: GraphVertex< SESVertex,SESEdge,SESFace >(),
			point_(point),
			normal_(normal),
			atom_(atom)
	{
		index_ = index;
	}


	SESVertex::~SESVertex()
		throw()
	{
	}


	void SESVertex::set(const SESVertex& sesvertex, bool deep)
		throw()
	{
		if (this != &sesvertex)
		{
			GraphVertex< SESVertex,SESEdge,SESFace >::set
					(sesvertex,deep);
			point_.set(sesvertex.point_);
			normal_.set(sesvertex.normal_);
			atom_ = sesvertex.atom_;
		}
	}


	SESVertex& SESVertex::operator = (const SESVertex& sesvertex)
		throw()
	{
		if (this != &sesvertex)
		{
			GraphVertex< SESVertex,SESEdge,SESFace >::operator =
					(sesvertex);
			point_.set(sesvertex.point_);
			normal_.set(sesvertex.normal_);
			atom_ = sesvertex.atom_;
		}
		return *this;
	}


	void SESVertex::set(const TVector3<double> point,
			const TVector3<double>& normal,
			Index atom,
			Index index)
		throw()
	{
		point_ = point;
		normal_ = normal;
		atom_ = atom;
		index_ = index;
	}


	void SESVertex::setPoint(const TVector3<double>& point)
		throw()
	{
		point_ = point;
	}


	TVector3<double> SESVertex::getPoint() const
		throw()
	{
		return point_;
	}


	void SESVertex::setNormal(const TVector3<double>& normal)
		throw(Exception::DivisionByZero)
	{
		normal_ = normal;
		normal_.normalize();
	}


	TVector3<double> SESVertex::getNormal() const
		throw()
	{
		return normal_;
	}


	void SESVertex::setAtom(Index atom)
		throw()
	{
		atom_ = atom;
	}


	Index SESVertex::getAtom() const
		throw()
	{
		return atom_;
	}


	bool SESVertex::operator == (const SESVertex&) const
		throw()
	{
		return true;
	}


	bool SESVertex::operator != (const SESVertex&) const
		throw()
	{
		return false;
	}


	bool SESVertex::operator *= (const SESVertex&) const
		throw()
	{
		return true;
	}


	std::ostream& operator << (std::ostream& s, const SESVertex& sesvertex)
	{
		s << "SESVERTEX" << sesvertex.getIndex()
			<< "("
			<< sesvertex.getPoint() << " "
			<< sesvertex.getNormal() << " [";
		SESVertex::ConstEdgeIterator e;
		for (e = sesvertex.beginEdge(); e != sesvertex.endEdge(); e++)
		{
			s << (*e)->getIndex() << ' ';
		}
		s << "] [";
		SESVertex::ConstFaceIterator f;
		for (f = sesvertex.beginFace(); f != sesvertex.endFace(); f++)
		{
			s << (*f)->getIndex() << ' ';
		}
		s << "] " << sesvertex.getAtom() << ")";
		return s;
	}


}	// namespace BALL
