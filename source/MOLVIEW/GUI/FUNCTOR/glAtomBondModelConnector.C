// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glAtomBondModelConnector.C,v 1.4 2002/02/27 12:23:46 sturm Exp $

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

		Line* GLAtomBondModelConnector::createLine_()
    {
			return new GLLine();
    }

	  TwoColoredLine* GLAtomBondModelConnector::createTwoColoredLine_()
    {
			return new GLTwoColoredLine();
    }

		Tube* GLAtomBondModelConnector::createTube_()
    {
			return new GLTube();
    }

		TwoColoredTube* GLAtomBondModelConnector::createTwoColoredTube_()
    {
			return new GLTwoColoredTube();
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
