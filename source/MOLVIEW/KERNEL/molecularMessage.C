// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularMessage.C,v 1.6 2003/08/26 09:18:07 oliver Exp $

#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>

using namespace std;
namespace BALL
{
	namespace MOLVIEW
	{

		NewMolecularMessage::NewMolecularMessage()
			throw()
			: CompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new MolecularMessage" << std::endl;
			#endif 

		}

		NewMolecularMessage::NewMolecularMessage(const CompositeMessage& message)
			throw()
			: CompositeMessage(message)
		{
		}

		NewMolecularMessage::~NewMolecularMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<NewMolecularMessage>() << std::endl;
			#endif 
		}

		MolecularTaskMessage::MolecularTaskMessage()
			throw()
			: type_(UNKNOWN)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new MolecularTaskMessage" << std::endl;
			#endif 
		}

		void MolecularTaskMessage::setType(Type type)
			throw() 
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "MolecularTaskMessage::setType " << type << std::endl;
			#endif 

			type_ = type;
		}
	} // namespace MOLVIEW
} // namespace BALL
