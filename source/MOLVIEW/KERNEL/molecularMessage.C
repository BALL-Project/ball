// $Id: molecularMessage.C,v 1.3 2001/05/13 15:02:41 hekl Exp $

#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		NewMolecularMessage::NewMolecularMessage()
			throw()
			:
			CompositeMessage()
		{
		}

		NewMolecularMessage::NewMolecularMessage(const CompositeMessage& message)
			throw()
			:
			CompositeMessage(message)
		{
		}

		NewMolecularMessage::~NewMolecularMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<NewMolecularMessage>() << endl;
			#endif 
		}

		RemovedMolecularMessage::RemovedMolecularMessage()
			throw()
			:
			RemovedCompositeMessage()
		{
		}

		RemovedMolecularMessage::RemovedMolecularMessage(const CompositeMessage& message)
			throw()
			:
			RemovedCompositeMessage(message)
		{
		}

		RemovedMolecularMessage::~RemovedMolecularMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<RemovedMolecularMessage>() << endl;
			#endif 
		}

		ChangedMolecularMessage::ChangedMolecularMessage()
			throw()
			:
			ChangedCompositeMessage()
		{
		}

	  ChangedMolecularMessage::ChangedMolecularMessage(const CompositeMessage& message)
			throw()
			:
			ChangedCompositeMessage(message)
		{
		}

		ChangedMolecularMessage::~ChangedMolecularMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ChangedMolecularMessage>() << endl;
			#endif 
		}


		MolecularSelectionMessage::MolecularSelectionMessage()
			throw()
			:
			SelectionMessage()
		{
		}

		MolecularSelectionMessage::MolecularSelectionMessage(const SelectionMessage& message)
			throw()
			:
			SelectionMessage(message)
		{
		}

		MolecularSelectionMessage::~MolecularSelectionMessage()
			throw()
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
