// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glAtomBondModelConnector.C,v 1.3.2.1 2003/01/07 13:21:29 anker Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{
		GLAtomBondModelConnector::GLAtomBondModelConnector()
			throw()
			:	AtomBondModelConnector()
		{
		}

		GLAtomBondModelConnector::GLAtomBondModelConnector
			(const GLAtomBondModelConnector& connector, bool deep)
			throw()
				:	AtomBondModelConnector(connector, deep)
		{
		}

		GLAtomBondModelConnector::~GLAtomBondModelConnector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLAtomBondModelConnector>() << endl;
			#endif 

			destroy();
		}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.iC>
#	endif

	} // namespace MOLVIEW

} // namespace BALL
