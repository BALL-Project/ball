// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangle.C,v 1.1.2.1 2003/01/07 13:22:07 anker Exp $

#include <BALL/STRUCTURE/triangleEdge.h>
#include <BALL/STRUCTURE/triangle.h>
#include <BALL/STRUCTURE/trianglePoint.h>

namespace BALL
{

	Triangle::Triangle()
		throw()
		:	GraphTriangle< TrianglePoint,TriangleEdge,Triangle >()
	{
	}


	Triangle::Triangle(const Triangle& triangle, bool deep)
		throw()
		:	GraphTriangle< TrianglePoint,TriangleEdge,Triangle >
				(triangle,deep)
	{
	}


	Triangle::~Triangle()
		throw()
	{
	}


	void Triangle::set(const Triangle& triangle, bool deep)
		throw()
	{
		if (this != &triangle)
		{
			GraphTriangle< TrianglePoint,TriangleEdge,Triangle >::set
					(triangle,deep);
		}
	}


	Triangle& Triangle::operator = (const Triangle& triangle)
		throw()
	{
		if (this != &triangle)
		{
			GraphTriangle< TrianglePoint,TriangleEdge,Triangle >::
					operator = (triangle);
		}
		return *this;
	}


	void Triangle::setPoint(Position i, TrianglePoint* vertex)
		throw(Exception::IndexOverflow)
	{
		switch (i)
		{
			case 0	:	vertex_[0] = vertex; break;
			case 1	:	vertex_[1] = vertex; break;
			case 2	:	vertex_[2] = vertex; break;
			default	:	throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
	}


	TrianglePoint* Triangle::getPoint(Position i) const
		throw(Exception::IndexOverflow)
	{
		switch (i)
		{
			case 0	:	return vertex_[0];
			case 1	:	return vertex_[1];
			case 2	:	return vertex_[2];
			default	:	throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
	}


	void Triangle::remove(TriangleEdge* edge)
		throw()
	{
		for (Position i = 0; i < 3; i++)
		{
			if (edge_[i] == edge)
			{
				edge_[i] = NULL;
			}
		}
	}


	bool Triangle::operator == (const Triangle&) const
		throw()
	{
		return true;
	}


	bool Triangle::operator != (const Triangle&) const
		throw()
	{
		return false;
	}


	bool Triangle::operator *= (const Triangle&) const
		throw()
	{
		return true;
	}


	std::ostream& operator << (std::ostream& s, const Triangle& triangle)
	{
		s << "TRIANGLE"
			<< triangle.getIndex()
			<< "(["
			<< (triangle.getVertex(0) == NULL
							? -2
							: triangle.getVertex(0)->getIndex()) << ' '
			<< (triangle.getVertex(1) == NULL
							? -2
							: triangle.getVertex(1)->getIndex()) << ' '
			<< (triangle.getVertex(2) == NULL
							? -2
							: triangle.getVertex(2)->getIndex())
			<< "] ["
			<< (triangle.getEdge(0) == NULL
							? -2
							: triangle.getEdge(0)->getIndex()) << ' '
			<< (triangle.getEdge(1) == NULL
							? -2
							: triangle.getEdge(1)->getIndex()) << ' '
			<< (triangle.getEdge(2) == NULL
							? -2
							: triangle.getEdge(2)->getIndex())
			<< "])";
		return s;
	}


}	// namespace BALL
