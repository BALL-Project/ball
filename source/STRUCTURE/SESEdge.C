// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SESEdge.C,v 1.2.2.1 2003/01/07 13:21:59 anker Exp $

#include <BALL/STRUCTURE/SESEdge.h>
#include <BALL/STRUCTURE/SESFace.h>
#include <BALL/STRUCTURE/SESVertex.h>
#include <BALL/STRUCTURE/RSEdge.h>
#include <BALL/MATHS/circle3.h>

namespace BALL
{

	SESEdge::SESEdge()
		throw()
		: GraphEdge< SESVertex,SESEdge,SESFace >(),
			circle_(),
			rsedge_(NULL),
			type_(SESEdge::TYPE_CONCAVE)
	{
	}


	SESEdge::SESEdge(const SESEdge& sesedge, bool deep)
		throw()
		: GraphEdge< SESVertex,SESEdge,SESFace >(sesedge,deep),
			circle_(sesedge.circle_),
			rsedge_(NULL),
			type_(sesedge.type_)
	{
		if (deep)
		{
			rsedge_ = sesedge.rsedge_;
		}
	}


	SESEdge::SESEdge(SESVertex* vertex0,
			SESVertex* vertex1,
			SESFace* face0,
			SESFace* face1,
			const TCircle3<double>& circle,
			RSEdge* rsedge,
			Type type,
			Index index)
		throw()
		: GraphEdge< SESVertex,SESEdge,SESFace >
					(vertex0,vertex1,face0,face1,index),
			circle_(circle),
			rsedge_(rsedge),
			type_(type)
	{
	}


	SESEdge::~SESEdge()
		throw()
	{
	}


	void SESEdge::set(const SESEdge& sesedge, bool deep)
		throw()
	{
		if (this != &sesedge)
		{
			GraphEdge< SESVertex,SESEdge,SESFace >::set(sesedge,deep);
			if (deep)
			{
				rsedge_ = sesedge.rsedge_;
			}
			else
			{
				rsedge_ = NULL;
			}
			circle_.set(sesedge.circle_);
			type_ = sesedge.type_;
		}
	}


	SESEdge& SESEdge::operator = (const SESEdge& sesedge)
		throw()
	{
		if (this != &sesedge)
		{
			GraphEdge< SESVertex,SESEdge,SESFace >::operator = (sesedge);
			rsedge_ = sesedge.rsedge_;
			circle_.set(sesedge.circle_);
			type_ = sesedge.type_;
		}
		return *this;
	}


	void SESEdge::set
			(SESVertex*			vertex0,
			 SESVertex*			vertex1,
			 SESFace*				face0,
			 SESFace*				face1,
			 const TCircle3<double>&	circle,
			 RSEdge*				rsedge,
			 Type								type,
			 Index							index)
		throw()
	{
		GraphEdge< SESVertex,SESEdge,SESFace >::set
				(vertex0,vertex1,face0,face1,index);
		circle_.set(circle);
		rsedge_ = rsedge;
		type_ = type;
	}


	void SESEdge::setCircle(const TCircle3<double>& circle)
		throw()
	{
		circle_ = circle;
	}


	TCircle3<double> SESEdge::getCircle() const
		throw()
	{
		return circle_;
	}


	void SESEdge::setRSEdge(RSEdge* rsedge)
		throw()
	{
		rsedge_ = rsedge;
	}


	RSEdge* SESEdge::getRSEdge() const
		throw()
	{
		return rsedge_;
	}


	void SESEdge::setType(SESEdge::Type type)
		throw()
	{
		type_ = type;
	}


	SESEdge::Type SESEdge::getType() const
		throw()
	{
		return type_;
	}


	bool SESEdge::operator == (const SESEdge& sesedge) const
		throw()
	{
		return (((vertex_[0] == sesedge.vertex_[0]) &&
						 (vertex_[1] == sesedge.vertex_[1])		) ||
						((vertex_[1] == sesedge.vertex_[0]) &&
						 (vertex_[0] == sesedge.vertex_[1])		)		);
	}


	bool SESEdge::operator != (const SESEdge& sesedge) const
		throw()
	{
		return (((vertex_[0] != sesedge.vertex_[0]) ||
						 (vertex_[1] != sesedge.vertex_[1])		) &&
						((vertex_[1] != sesedge.vertex_[0]) ||
						 (vertex_[0] != sesedge.vertex_[1])		)		);
	}


	bool SESEdge::operator *= (const SESEdge&) const
		throw()
	{
		return true;
	}


	bool SESEdge::isFree() const
		throw()
	{
		if (rsedge_ == NULL)
		{
			return false;
		}
		else
		{
			return rsedge_->isFree();
		}
	}


	std::ostream& operator << (std::ostream& s, const SESEdge& sesedge)
	{
		return (s << "SESEDGE" << sesedge.getIndex() << "(["
							<< ((sesedge.getVertex(0) == NULL)
											? -2
											: sesedge.getVertex(0)->getIndex())
							<< ' '
							<< ((sesedge.getVertex(1) == NULL)
											? -2
											: sesedge.getVertex(1)->getIndex())
							<< "] ["
							<< ((sesedge.getFace(0) == NULL)
											? -2
											: sesedge.getFace(0)->getIndex())
							<< ' '
							<< ((sesedge.getFace(1) == NULL)
											? -2
											: sesedge.getFace(1)->getIndex())
							<< "] "
							<< sesedge.getCircle() << ' '
							<< ((sesedge.getRSEdge() == NULL)
											? -2
											: sesedge.getRSEdge()->getIndex())
							<< ((sesedge.getType() == SESEdge::TYPE_CONCAVE)
											? " concave)"
											: ((sesedge.getType() == SESEdge::TYPE_CONVEX)
															? " convex)"
															: " singular)"))
						);
	}


}	// namespace BALL
