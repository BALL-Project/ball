// $Id: molecularFilter.C,v 1.5.4.1 2002/10/18 14:48:23 amoll Exp $

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

	} // namespace MOLVIEW

} // namespace BALL
