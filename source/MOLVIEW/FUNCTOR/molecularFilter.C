// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFilter.C,v 1.6 2002/02/27 12:21:50 sturm Exp $

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
