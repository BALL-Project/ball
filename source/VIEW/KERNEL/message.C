// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.C,v 1.12 2003/09/19 18:17:59 amoll Exp $

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

Message::Message()
	throw()
	: connection_object_(0),
		deletable_(true)
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
		type_(UNDEFINED),
		composite_(0),
		composite_name_()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::CompositeMessage(const Composite& composite, Index type)
	throw()
	: Message(),
		type_(type),
		composite_((Composite*)&composite),
		composite_name_()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::CompositeMessage(const CompositeMessage& message)
	throw()
	: Message(message),
		type_(message.type_),
		composite_(message.composite_),
		composite_name_(message.composite_name_)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::~CompositeMessage()
	throw()
{
}

SceneMessage::SceneMessage(Type type)
	throw()
	: Message(),
		type_(type)
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
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new SceneMessage" << type_ << std::endl;		
	#endif
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

RepresentationMessage::RepresentationMessage()
	throw()
	: representation_(0),
		type_(UNDEFINED)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RepresentationMessage" << std::endl;		
	#endif
}

RepresentationMessage::RepresentationMessage(const Representation& rep, Type type)
	throw()
	: representation_((Representation*)&rep),
		type_(type)
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

MolecularTaskMessage::MolecularTaskMessage(Type type)
	throw()
	: type_(type)
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


NewTrajectoryMessage::NewTrajectoryMessage()
	throw()
	: CompositeMessage(),
		file_(0)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new NewTrajectoryMessage" << std::endl;		
	#endif
}
	
RegularData3DMessage::RegularData3DMessage(Type type)
	throw()
	: CompositeMessage(),
		data_(0)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RegularData3DMessage" << std::endl;		
	#endif
	setType(type);
}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/message.iC>
#	endif 

} } // namespaces
