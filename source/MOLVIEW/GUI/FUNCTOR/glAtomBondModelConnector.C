// $Id: glAtomBondModelConnector.C,v 1.1 2000/09/23 15:39:13 hekl Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{
		GLAtomBondModelConnector::GLAtomBondModelConnector()
			:	AtomBondModelConnector()
		{
		}

		GLAtomBondModelConnector::GLAtomBondModelConnector
			(const AtomBondModelConnector& connector, bool deep)
				:	AtomBondModelConnector(connector, deep)
		{
		}

		GLAtomBondModelConnector::~GLAtomBondModelConnector()
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
