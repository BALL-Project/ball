// $Id: molecularMessage.C,v 1.2 2000/05/16 15:43:35 hekl Exp $

#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		NewMolecularMessage::NewMolecularMessage()
			:
			CompositeMessage()
		{
		}

		NewMolecularMessage::NewMolecularMessage(const CompositeMessage& message, bool /* deep */)
			:
			CompositeMessage(message)
		{
		}

		NewMolecularMessage::~NewMolecularMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<NewMolecularMessage>() << endl;
			#endif 
		}

		RemovedMolecularMessage::RemovedMolecularMessage()
			:
			RemovedCompositeMessage()
		{
		}

		RemovedMolecularMessage::RemovedMolecularMessage(const CompositeMessage& message, bool /* deep */)
			:
			RemovedCompositeMessage(message)
		{
		}

		RemovedMolecularMessage::~RemovedMolecularMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<RemovedMolecularMessage>() << endl;
			#endif 
		}

		ChangedMolecularMessage::ChangedMolecularMessage()
			:
			ChangedCompositeMessage()
		{
		}

	  ChangedMolecularMessage::ChangedMolecularMessage(const CompositeMessage& message, bool /* deep */)
			:
			ChangedCompositeMessage(message)
		{
		}

		ChangedMolecularMessage::~ChangedMolecularMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ChangedMolecularMessage>() << endl;
			#endif 
		}


		MolecularSelectionMessage::MolecularSelectionMessage()
			:
			SelectionMessage()
		{
		}

		MolecularSelectionMessage::MolecularSelectionMessage(const SelectionMessage& message, bool /* deep */)
			:
			SelectionMessage(message)
		{
		}

		MolecularSelectionMessage::~MolecularSelectionMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MolecularSelectionMessage>() << endl;
			#endif 
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/molecularMessage.iC>
#		endif 

	} // namespace MOLVIEW

} // namespace BALL
