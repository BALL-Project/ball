// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangleEdge.C,v 1.1.2.1 2003/01/07 13:22:08 anker Exp $

#include <BALL/STRUCTURE/triangleEdge.h>
#include <BALL/STRUCTURE/triangle.h>
#include <BALL/STRUCTURE/trianglePoint.h>

namespace BALL
{

	TriangleEdge::TriangleEdge()
		throw()
		:	GraphEdge< TrianglePoint,TriangleEdge,Triangle >()
	{
	}


	TriangleEdge::TriangleEdge(const TriangleEdge& edge, bool deep)
		throw()
		:	GraphEdge< TrianglePoint,TriangleEdge,Triangle >(edge,deep)
	{
	}


	TriangleEdge::~TriangleEdge()
		throw()
	{
	}


	void TriangleEdge::setPoint(Position i, TrianglePoint* point)
		throw()
	{
		if (i == 0)
		{
			vertex_[0] = point;
		}
		else
		{
			vertex_[1] = point;
		}
	}


	TrianglePoint* TriangleEdge::getPoint(Position i) const
		throw()
	{
		if (i == 0)
		{
			return vertex_[0];
		}
		else
		{
			return vertex_[1];
		}
	}


	void TriangleEdge::setTriangle(Position i, Triangle* triangle)
		throw()
	{
		if (i == 0)
		{
			face_[0] = triangle;
		}
		else
		{
			face_[1] = triangle;
		}
	}


	Triangle* TriangleEdge::getTriangle(Position i) const
		throw()
	{
		if (i == 0)
		{
			return face_[0];
		}
		else
		{
			return face_[1];
		}
	}


	bool TriangleEdge::operator == (const TriangleEdge& edge) const
		throw()
	{
		return ( ((vertex_[0]->point_ == edge.vertex_[0]->point_) &&
							(vertex_[1]->point_ == edge.vertex_[1]->point_))		||
						 ((vertex_[0]->point_ == edge.vertex_[1]->point_) &&
							(vertex_[1]->point_ == edge.vertex_[0]->point_))				);
	}


	bool TriangleEdge::operator != (const TriangleEdge& edge) const
		throw()
	{
		return ( !(*this == edge) );
	}


	bool TriangleEdge::operator *= (const TriangleEdge&) const
		throw()
	{
		return true;
	}


	std::ostream& operator << (std::ostream& s, const TriangleEdge& edge)
	{
		s << "EDGE" << edge.getIndex()
			<< "("
			<< (edge.getVertex(0) == NULL
							? -2
							: edge.getVertex(0)->getIndex())
			<< "-"
			<< (edge.getVertex(1) == NULL
							? -2
							: edge.getVertex(1)->getIndex())
			<< " "
			<< (edge.getFace(0) == NULL
							? -2
							: edge.getFace(0)->getIndex())
			<< "|"
			<< (edge.getFace(1) == NULL
							? -2
							: edge.getFace(1)->getIndex())
			<< ")";
		return s;
	}


}	// namespace BALL
