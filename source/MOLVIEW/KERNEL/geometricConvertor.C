// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricConvertor.C,v 1.3 2002/02/27 12:23:50 sturm Exp $

#include <BALL/MOLVIEW/KERNEL/geometricConvertor.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

  	GeometricConvertor::GeometricConvertor()
			:	ConnectionObject()
    {
    }

  	GeometricConvertor::~GeometricConvertor()
    {
    }

	  void GeometricConvertor::onNotify(Message *message)
    {
			if (RTTI::isKindOf<GeometricObjectSelectionMessage>(*message))
			{
				GeometricObjectSelectionMessage* geometric_selection 
					= RTTI::castTo<GeometricObjectSelectionMessage>(*message);

				// geometric selection is not empty
				if (!geometric_selection->getSelection().empty())
				{
					List<Composite*> selection;
					List<Composite*>::ConstIterator it = geometric_selection->getSelection().begin();
					
					for (; it != geometric_selection->getSelection().end(); ++it)
					{
						Atom* atom = (**it).getAncestor(RTTI::getDefault<Atom>());
						AtomContainer* fragment = (**it).getAncestor(RTTI::getDefault<AtomContainer>());

						if (atom != 0)
						{
							selection.push_back(atom);
						}
						else if (fragment != 0)
						{
							selection.push_back(fragment);
						}
					}

					// new collection not empty
					if (!selection.empty())
					{
						// create a molecular selection message and sent molecular objects
						MolecularSelectionMessage *molecular_selection = new MolecularSelectionMessage;
						molecular_selection->setDeletable(true);
						molecular_selection->setSelection(selection);

						notify_(molecular_selection);
					}
				}
			}
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/geometricConvertor.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
