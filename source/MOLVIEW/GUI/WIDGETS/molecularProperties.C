// $Id: molecularProperties.C,v 1.6 2001/07/15 17:47:46 oliver Exp $

#include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

  	MolecularProperties::MolecularProperties(QWidget* parent, const char* name)
			throw()
			:	QWidget(parent, name),
				ModularWidget(name),
				fragment_db_()
    {
			// register ModularWidget
			registerWidget(this);
    }

  	MolecularProperties::~MolecularProperties()
			throw()
    {
    }

	  void MolecularProperties::onNotify(Message *message)
			throw()
    {
			if (RTTI::isKindOf<NewCompositeMessage>(*message))
			{
				NewCompositeMessage *composite_message 
					= RTTI::castTo<NewCompositeMessage>(*message);
				
				// properties will be used only for atom containers
				if (!RTTI::isKindOf<AtomContainer>(*(composite_message->getComposite())))
				{
					// ??????????????? warum wird eine MolecularMessage gesendet wenn
					// composite kein AtomContainer ?????????????????????????????????
					//
					//					NewMolecularMessage* mol_message = new NewMolecularMessage;
					//					mol_message->setComposite(composite_message->getComposite());
					//					mol_message->setDeletable(true);
					
					//					notify_(mol_message);
					
					return;
				}
				
				Log.info() << "> applying molecular properties ... " << endl;
				
				AtomContainer* atom_container 
					= RTTI::castTo<AtomContainer>(*(composite_message->getComposite()));
				
				try
				{
					atom_container->apply(fragment_db_.normalize_names);
				}
				catch (Exception::GeneralException e)
				{
					Log.error() << " > normalize named failed: " << e << endl;
				}
				catch (...)
				{
					Log.error() << "  > normalized names failed." << endl;
					return;
				}
				
				Log.info() << "  > normalized names" << endl;
				
				try
				{
					atom_container->apply(fragment_db_.build_bonds);
				}
				catch (Exception::GeneralException e)
				{
					Log.error() << " > generate missing bonds - failed: " << e << endl;
				}
				catch (...)
				{
					Log.error() << "  > generate missing bonds - failed." << endl;
					return;
				}
				
				Log.info() << "  > generated missing bonds" << endl;
				
				if (atom_container->getName() == "")
				{
					atom_container->setName(composite_message->getCompositeName());
				}
				
				Log.info() << "> applying molecular properties ... finished" << endl;
				
				// continue with molecular message
				NewMolecularMessage* mol_message = new NewMolecularMessage;
				mol_message->setComposite(composite_message->getComposite());
				mol_message->setDeletable(true);
				
				notify_(mol_message);
			}
			else if (RTTI::isKindOf<GeometricObjectSelectionMessage>(*message))
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
#			include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
