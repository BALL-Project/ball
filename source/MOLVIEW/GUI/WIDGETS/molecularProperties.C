// $Id: molecularProperties.C,v 1.2 2000/11/05 14:35:25 hekl Exp $

#include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

  	MolecularProperties::MolecularProperties(QWidget* parent, const char* name)
			:	QWidget(parent, name),
				ModularWidget(name),
				fragment_db_()
    {
			// register ModularWidget
			registerWidget(this);
    }

  	MolecularProperties::~MolecularProperties()
    {
    }

	  void MolecularProperties::onNotify(Message *message)
    {
			if (RTTI::isKindOf<NewCompositeMessage>(*message))
			{
				NewCompositeMessage *composite_message 
					= RTTI::castTo<NewCompositeMessage>(*message);
				
				// properties will be used only for atom containers
				if (!RTTI::isKindOf<AtomContainer>(*(composite_message->getComposite())))
				{
					NewMolecularMessage* mol_message = new NewMolecularMessage;
					mol_message->setComposite(composite_message->getComposite());
					mol_message->setDeletable(true);
					
					notify_(mol_message);
					
					return;
				}
				
				Log.info() << "> applying molecular properties ... " << endl;
				
				AtomContainer* atom_container 
					= RTTI::castTo<AtomContainer>(*(composite_message->getComposite()));
				
				try
				{
					atom_container->apply(fragment_db_.normalize_names);
				}
				catch(...)
				{
					Log.info() << "  > normalized names failed." << endl;
					return;
				}
				
				Log.info() << "  > normalized names" << endl;
				
				try
				{
					atom_container->apply(fragment_db_.build_bonds);
				}
				catch(...)
				{
					Log.info() << "  > generated missing bonds failed." << endl;
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
    }

		void MolecularProperties::initializeWidget(MainControl& /* main_control */)
		{
		}
		
		void MolecularProperties::finalizeWidget(MainControl& /* main_control */)
		{
		}
		
		void MolecularProperties::checkMenu(MainControl& /* main_control */)
		{
		}
		
#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
