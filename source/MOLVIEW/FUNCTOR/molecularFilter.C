// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFilter.C,v 1.5.2.1 2003/01/07 13:21:21 anker Exp $

#include <BALL/MOLVIEW/FUNCTOR/molecularFilter.h>
#include <BALL/KERNEL/atomContainer.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		MolecularFilter::MolecularFilter()
			throw()
			:	VIEW::Filter()
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
			VIEW::Filter::clear();
		}

		void MolecularFilter::destroy()
			throw()
		{
			VIEW::Filter::destroy();
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

	} // namespace MOLVIEW

} // namespace BALL
