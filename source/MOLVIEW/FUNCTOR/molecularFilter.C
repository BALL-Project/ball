// $Id: molecularFilter.C,v 1.4 2001/05/13 15:02:39 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/molecularFilter.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		MolecularFilter::MolecularFilter()
			throw()
			:	Filter()
		{
		}

		MolecularFilter::~MolecularFilter()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MolecularFilter>() << endl;
			#endif 

			destroy();
		}

		void MolecularFilter::clear()
			throw()
		{
			Filter::clear();
		}

		void MolecularFilter::destroy()
			throw()
		{
			Filter::destroy();
		}

		void MolecularFilter::visit(Composite& composite)
			throw()
		{
			bool result = false;

			if (RTTI::isKindOf<AtomContainer>(composite))
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
