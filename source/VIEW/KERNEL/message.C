// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DATATYPE/dataset.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{

Message::Message()
	
	: connection_object_(0),
		deletable_(true)
{
}

Message::Message(const Message& message)
	
	: connection_object_(message.connection_object_),
		deletable_(message.deletable_)
{
}

Message::~Message()
	
{
}


DatasetMessage::DatasetMessage(Dataset* set, DatasetMessage::Type type)
	: Message(),
		dataset_(set),
		type_(type),
		dataset_type_("")
{
}

DatasetMessage::DatasetMessage(const DatasetMessage& msg)
	: Message(),
		dataset_(msg.dataset_),
		type_(msg.type_),
		dataset_type_(msg.dataset_type_)
{
}

bool DatasetMessage::isValid() const
{
	return dataset_ != 0 && dataset_->getType() != "";
}


CompositeMessage::CompositeMessage()
	
	: Message(),
		type_(UNDEFINED),
		composite_(0),
		composite_name_(),
		update_representations_(true),
		show_selection_infos_(true)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::CompositeMessage(const Composite& composite, CompositeMessageType type, bool update_representations)
	
	: Message(),
		type_(type),
		composite_((Composite*)&composite),
		composite_name_(),
		update_representations_(update_representations),
		show_selection_infos_(true)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::CompositeMessage(const CompositeMessage& message)
	
	: Message(message),
		type_(message.type_),
		composite_(message.composite_),
		composite_name_(message.composite_name_),
		update_representations_(message.update_representations_),
		show_selection_infos_(message.show_selection_infos_)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::~CompositeMessage()
	
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing " << this << " CompositeMessage" << endl;
	#endif 
}

SceneMessage::SceneMessage(SceneMessageType type)
	: Message(),
		type_(type),
		stage_()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new SceneMessage " << type << " " << this << std::endl;		
	#endif
}

SceneMessage::SceneMessage(const SceneMessage& message)
	: Message(message),
		type_(message.type_),
		stage_(message.stage_)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new SceneMessage2 " << type_ << " " << this << std::endl;		
	#endif
}

SceneMessage::~SceneMessage()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing " << this << " SceneMessage" << endl;
	#endif 
}

void SceneMessage::setType(SceneMessageType type)
{ 
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "SceneMessage::setType " << type << std::endl;
	#endif 

	type_ = type;
}


GenericSelectionMessage::GenericSelectionMessage()
	: Message(),
		selection_()
{
}

GenericSelectionMessage::GenericSelectionMessage(const GenericSelectionMessage& message)
	
	: Message(message),
		selection_(message.selection_)
{
}

GenericSelectionMessage::~GenericSelectionMessage()
	
{
}


GeometricObjectSelectionMessage::GeometricObjectSelectionMessage()
	
	: Message()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new GeometricObjectSelectionMessage" << std::endl;		
	#endif
}

GeometricObjectSelectionMessage::~GeometricObjectSelectionMessage()
	
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class " << RTTI::getName<GeometricObjectSelectionMessage>() << endl;
	#endif 
}

ControlSelectionMessage::ControlSelectionMessage()
	
	: GenericSelectionMessage()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new ControlSelectionMessage" << std::endl;
	#endif
}

NewSelectionMessage::NewSelectionMessage() 
	
	: Message(),
		open_(false)
{ 
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new NewSelectionMessage" << std::endl;
	#endif
}

RepresentationMessage::RepresentationMessage()
	
	: representation_(0),
		type_(UNDEFINED)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RepresentationMessage " << this<< std::endl;		
	#endif
}

RepresentationMessage::RepresentationMessage(Representation& rep, RepresentationMessageType type)
	
	: representation_(&rep),
		type_(type)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RepresentationMessage " << type << " " << this << std::endl;		
	#endif
}

RepresentationMessage::~RepresentationMessage()
	
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing " << this << " RepresentationMessage" << endl;
	#endif 
}

void RepresentationMessage::setType(RepresentationMessageType type)
	 
{ 
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "RepresentationMessage::setType " << type << std::endl;
	#endif 

	type_ = type;
}

MolecularTaskMessage::MolecularTaskMessage(MolecularTaskMessageType type)
	
	: type_(type)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new MolecularTaskMessage" << std::endl;
	#endif 
}

void MolecularTaskMessage::setType(MolecularTaskMessageType type)
	 
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "MolecularTaskMessage::setType " << type << std::endl;
	#endif 

	type_ = type;
}


CreateRepresentationMessage::CreateRepresentationMessage()
	 
	: Message(), 
		composites_(),
		model_type_(MODEL_UNKNOWN),
		coloring_method_(COLORING_UNKNOWN)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CreateRepresentationMessage" << std::endl;		
	#endif
}

CreateRepresentationMessage::CreateRepresentationMessage(const list<Composite*>& composites,
		ModelType model_type, ColoringMethod coloring_method)
 	 
	: Message(),
		composites_(composites), 
		model_type_(model_type),
		coloring_method_(coloring_method)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CreateRepresentationMessage" << std::endl;		
	#endif
}

TransformationMessage::TransformationMessage()
	
	: Message(),
		matrix_()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new TransformationMessage" << std::endl;		
	#endif
}

TransformationMessage::TransformationMessage(const Matrix4x4& m)
	
	: Message(),
		matrix_(m)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new TransformationMessage" << std::endl;		
	#endif
}

FinishedSimulationMessage::FinishedSimulationMessage()
	
	: Message()
{
}

DockingFinishedMessage::DockingFinishedMessage()
	
	: Message(),
		conformation_set_(0)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new DockingFinishedMessage" << std::endl;
	#endif
}

DockingFinishedMessage::DockingFinishedMessage(bool abort)
	
	: Message(),
		conformation_set_(0)
{
	abort_ = abort;
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new DockingFinishedMessage" << std::endl;
	#endif
}

DockingFinishedMessage::~DockingFinishedMessage()
	
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing " << this << "DockingFinishedMessage" << std::endl;
	#endif
}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/message.iC>
#	endif 

} } // namespaces
