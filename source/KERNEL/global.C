// $Id: global.C,v 1.4 2000/01/08 20:52:11 oliver Exp $

#include <BALL/KERNEL/global.h>
#include <BALL/MATHS/vector3.h>
namespace BALL 
{

	bool clone_bonds = true;
	
	// this is a workaround for the SUNPro CC 6.0ea compiler
	Vector3 BALL_vtable_constructing_vector;

} // namespace BALL
