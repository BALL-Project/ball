// $Id: molecularFilter.C,v 1.1 2000/05/16 20:53:58 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/molecularFilter.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		MolecularFilter::MolecularFilter()
			:	Filter()
		{
		}

		MolecularFilter::~MolecularFilter()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MolecularFilter>() << endl;
			#endif 

			destroy();
		}

		void MolecularFilter::visit(Composite& composite)
		{
			bool result = false;

			if (RTTI::isKindOf<BaseFragment>(composite))
			{
				result = true;
			}
			else if (RTTI::isKindOf<Atom>(composite))
			{
				result = true;
			}

			setResult_(result);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/molecularFilter.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
