// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.C,v 1.4.2.1 2003/01/07 13:20:53 anker Exp $

#include <BALL/KERNEL/global.h>
#include <BALL/MATHS/vector3.h>
namespace BALL 
{

	bool clone_bonds = true;
	
	// this is a workaround for the SUNPro CC 6.0ea compiler
	Vector3 BALL_vtable_constructing_vector;

} // namespace BALL
