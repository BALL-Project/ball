// $Id: molecularFilter.C,v 1.5.4.4 2002/11/26 15:28:27 oliver Exp $

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
