// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SASEdge.C,v 1.1.2.1 2003/01/07 13:21:58 anker Exp $

#include <BALL/STRUCTURE/SASEdge.h>
#include <BALL/STRUCTURE/SASFace.h>
#include <BALL/STRUCTURE/SASVertex.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/circle3.h>


namespace BALL
{

	SASEdge::SASEdge()
		throw()
		: GraphEdge< SASVertex,SASEdge,SASFace >(),
			circle_(),
			angle_()
	{
	}


	SASEdge::SASEdge(const SASEdge& sasedge, bool deep)
		throw()
		: GraphEdge< SASVertex,SASEdge,SASFace >(sasedge,deep),
			circle_(sasedge.circle_),
			angle_(sasedge.angle_)
	{
	}


	SASEdge::SASEdge(SASVertex* vertex0,
			SASVertex* vertex1,
			SASFace* face0,
			SASFace* face1,
			const TCircle3<double>& circle,
			const TAngle<double>& angle,
			Index index)
		throw()
		: GraphEdge< SASVertex,SASEdge,SASFace >
				(vertex0,vertex1,face0,face1,index),
			circle_(circle),
			angle_(angle)
	{
	}


	SASEdge::~SASEdge()
		throw()
	{
	}


	void SASEdge::set(const SASEdge& sasedge, bool deep)
		throw()
	{
		if (this != &sasedge)
		{
			GraphEdge< SASVertex,SASEdge,SASFace >::set(sasedge,deep);
			circle_.set(sasedge.circle_);
			angle_.set(sasedge.angle_);
		}
	}


	SASEdge& SASEdge::operator = (const SASEdge& sasedge)
		throw()
	{
		if (this != &sasedge)
		{
			GraphEdge< SASVertex,SASEdge,SASFace >::operator = (sasedge);
			circle_.set(sasedge.circle_);
			angle_.set(sasedge.angle_);
		}
		return *this;
	}


	void SASEdge::set
			(SASVertex* vertex0,
			 SASVertex* vertex1,
			 SASFace* face0,
			 SASFace* face1,
			 const TCircle3<double>& circle,
			 const TAngle<double>& angle,
			 Index index)
		throw()
	{
		GraphEdge< SASVertex,SASEdge,SASFace >::set
				(vertex0,vertex1,face0,face1,index);
		circle_.set(circle);
		angle_.set(angle);
	}


	void SASEdge::setCircle(const TCircle3<double>& circle)
		throw()
	{
		circle_ = circle;
	}


	TCircle3<double> SASEdge::getCircle() const
		throw()
	{
		return circle_;
	}


	void SASEdge::setAngle(const TAngle<double>& angle)
		throw()
	{
		angle_ = angle;
	}


	TAngle<double> SASEdge::getAngle() const
		throw()
	{
		return angle_;
	}


	bool SASEdge::operator == (const SASEdge&) const
		throw()
	{
		return true;
	}


	bool SASEdge::operator != (const SASEdge&) const
		throw()
	{
		return false;
	}


	bool SASEdge::operator *= (const SASEdge&) const
		throw()
	{
		return true;
	}


	bool SASEdge::isFree() const
		throw()
	{
		return (vertex_[0] == NULL);
	}


	std::ostream& operator << (std::ostream& s, const SASEdge& sasedge)
	{
		return (s << "SASEDGE" << sasedge.getIndex() << "(["
							<< ((sasedge.getVertex(0) == NULL)
											? -2
											: sasedge.getVertex(0)->getIndex()) << ' '
							<< ((sasedge.getVertex(1) == NULL)
											? -2
											: sasedge.getVertex(1)->getIndex()) << "] ["
							<< ((sasedge.getFace(0) == NULL)
											? -2
											: sasedge.getFace(0)->getIndex()) << ' '
							<< ((sasedge.getFace(1) == NULL)
											? -2
											: sasedge.getFace(1)->getIndex()) << "] "
							<< sasedge.getCircle()
						);
	}


}	// namespave BALL
