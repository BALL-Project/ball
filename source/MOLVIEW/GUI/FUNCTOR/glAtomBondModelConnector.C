// $Id: glAtomBondModelConnector.C,v 1.3.4.1 2002/10/18 14:48:26 amoll Exp $

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

	} // namespace MOLVIEW

} // namespace BALL
