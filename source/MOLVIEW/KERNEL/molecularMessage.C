// $Id: molecularMessage.C,v 1.1 2000/05/14 15:29:36 hekl Exp $

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


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/molecularMessage.iC>
#		endif 

	} // namespace MOLVIEW

} // namespace BALL
