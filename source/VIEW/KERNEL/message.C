// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.C,v 1.8 2003/08/27 14:35:38 amoll Exp $

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/COMMON/rtti.h>

//#define BALL_VIEW_DEBUG
using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Message::Message()
			throw()
			: connection_object_(0),
				deletable_(false)
		{
		}

		Message::Message(const Message& message)
			throw()
			: connection_object_(message.connection_object_),
				deletable_(message.deletable_)
		{
		}

		Message::~Message()
			throw()
		{
		}

		CompositeMessage::CompositeMessage()
			throw()
			: Message(),
				composite_(0),
				composite_name_()
		{
		}

		CompositeMessage::CompositeMessage(const CompositeMessage& message)
			throw()
			: Message(message),
				composite_(message.composite_),
				composite_name_(message.composite_name_)
		{
		}

		CompositeMessage::~CompositeMessage()
			throw()
		{
		}

		NewCompositeMessage::NewCompositeMessage()
			throw()
			: CompositeMessage()
		{
		}

		NewCompositeMessage::NewCompositeMessage(const CompositeMessage& message)
			throw()
			: CompositeMessage(message)
		{
		}

		NewCompositeMessage::~NewCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<NewCompositeMessage>() << endl;
			#endif 
		}

		RemovedCompositeMessage::RemovedCompositeMessage()
			throw()
			: CompositeMessage()
		{
		}

		RemovedCompositeMessage::RemovedCompositeMessage(const CompositeMessage& message)
			throw()
			: CompositeMessage(message)
		{
		}

		RemovedCompositeMessage::~RemovedCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<RemovedCompositeMessage>() << endl;
			#endif 
		}

		ChangedCompositeMessage::ChangedCompositeMessage()
			throw()
			: CompositeMessage(),
				update_control_(false)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new ChangedCompositeMessage" << std::endl;		
			#endif
		}

	  ChangedCompositeMessage::ChangedCompositeMessage(const ChangedCompositeMessage& message)
			throw()
			: CompositeMessage(message),
				update_control_(message.update_control_)
		{
		}

		ChangedCompositeMessage::~ChangedCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<ChangedCompositeMessage>() << endl;
			#endif 
		}

		SceneMessage::SceneMessage()
			throw()
			: Message(),
				type_(UNDEFINED)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new SceneMessage" << std::endl;		
			#endif
		}

		SceneMessage::SceneMessage(const SceneMessage& message)
			throw()
			: Message(message),
				type_(message.type_),
				camera_(message.camera_)
		{
		}

		SceneMessage::~SceneMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<SceneMessage>() << endl;
			#endif 
		}

		void SceneMessage::setType(Type type)
			throw() 
		{ 
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "SceneMessage::setType " << type << std::endl;
			#endif 

			type_ = type;
		}


		SelectionMessage::SelectionMessage()
			throw()
			: Message(),
				selection_()
		{
		}

		SelectionMessage::SelectionMessage(const SelectionMessage& message)
			throw()
			: Message(message),
				selection_(message.selection_)
		{
		}

		SelectionMessage::~SelectionMessage()
			throw()
		{
		}


		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage()
			throw()
			: Message()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new GeometricObjectSelectionMessage" << std::endl;		
			#endif
		}

		GeometricObjectSelectionMessage::~GeometricObjectSelectionMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<GeometricObjectSelectionMessage>() << endl;
			#endif 
		}

		ControlSelectionMessage::ControlSelectionMessage()
			throw()
			: SelectionMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new ControlSelectionMessage" << std::endl;
			#endif
		}

		NewSelectionMessage::NewSelectionMessage() 
			throw()
		{ 
			#ifdef BALL_VIEW_DEBUG
			  Log.error() << "new NewSelectionMessage" << std::endl;
			#endif
		}

		CompositeSelectedMessage::CompositeSelectedMessage(Composite* composite, bool selected) 
			throw()
			:	CompositeMessage(),
				selected_(selected)
		{
			#ifdef BALL_VIEW_DEBUG
			  Log.error() << "new CompositeSelectedMessage" << std::endl;		
			#endif

			setComposite(composite);
		}

		CenterCameraMessage::CenterCameraMessage()
			throw()
			: CompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
			  Log.error() << "new CenterCameraMessage" << std::endl;		
			#endif
		}

		RepresentationMessage::RepresentationMessage()
			throw()
			: representation_(0),
				type_(UNKNOWN)
		{
			#ifdef BALL_VIEW_DEBUG
			  Log.error() << "new RepresentationMessage" << std::endl;		
			#endif
		}

		void RepresentationMessage::setType(Type type)
			throw() 
		{ 
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "RepresentationMessage::setType " << type << std::endl;
			#endif 

			type_ = type;
		}


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

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/message.iC>
#	endif 

#undef BALL_VIEW_DEBUG
	} // namespace VIEW
} // namespace BALL
