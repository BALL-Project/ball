// $Id: geometricConvertor.C,v 1.2.4.1 2002/10/18 14:48:30 amoll Exp $

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

	} // namespace MOLVIEW

} // namespace BALL
