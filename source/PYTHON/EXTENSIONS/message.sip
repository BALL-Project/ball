// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.sip,v 1.8 2004/07/01 13:08:43 amoll Exp $
//

class Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	Message();
	Message(const Message& /message/);
	// void setSender(const void* object);
	// const void* getSender() const;
	// void setDeletable(bool flag = true);
	// bool isDeletable() const;
};


class CompositeMessage
 : Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum CompositeMessageType
	{
		UNDEFINED = -1,
		NEW_COMPOSITE,
		REMOVED_COMPOSITE,
		CHANGED_COMPOSITE,
		CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL,
		SELECTED_COMPOSITE,
		DESELECTED_COMPOSITE,
		CENTER_CAMERA,
		NEW_MOLECULE
	};

	CompositeMessage();
	CompositeMessage(const Composite& /composite/, CompositeMessageType /type/);
	CompositeMessage(const CompositeMessage& /message/);
	~CompositeMessage();
	void setComposite(const Composite& /composite/);
	Composite* getComposite() const;
	void setCompositeName(const String& /name/);
	const String& getCompositeName() const;
	void setType(CompositeMessageType /type/);
	CompositeMessageType getType() const;
};


class SceneMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum SceneMessageType
	{
		UNDEFINED = 0,
		REBUILD_DISPLAY_LISTS,
		REDRAW,
		UPDATE_CAMERA,
		REMOVE_COORDINATE_SYSTEM,
		EXPORT_PNG
	};
	SceneMessage(SceneMessageType) throw();
	SceneMessage(const SceneMessage&) throw();
	~SceneMessage() throw();
	
	void setType(SceneMessageType) throw();
	SceneMessageType getType() const throw();
	void setStage(Stage) throw();
	Stage& getStage() throw();
 	const Stage& getStage() const throw();
};


class GenericSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	GenericSelectionMessage() throw();
	GenericSelectionMessage(const GenericSelectionMessage& ) throw();
	~GenericSelectionMessage() throw();
	void setSelection(const PyCompositeList&) throw();
	PyCompositeList getSelection() const throw();
};


class ControlSelectionMessage
	: GenericSelectionMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	ControlSelectionMessage() throw();
};


class NewSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	NewSelectionMessage() throw();
};


class GeometricObjectSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	GeometricObjectSelectionMessage() throw();
	~GeometricObjectSelectionMessage() throw();
	void setSelection(const GeometricObjectList& ) throw();
	const GeometricObjectList& getSelection() const throw();
	void setSelected(bool ) throw();
	bool isSelected() const throw();
};


class RepresentationMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum RepresentationMessageType
	{
		UNDEFINED = -1,
		ADD = 0,
		REMOVE,
		UPDATE,
		SELECTED
	};

	RepresentationMessage() throw();
	RepresentationMessage(Representation& , RepresentationMessageType ) throw();
	void setRepresentation(Representation& ) throw();
	Representation* getRepresentation() throw();
	void setType(RepresentationMessageType ) throw();
	RepresentationMessageType getType() const throw();
};


class MolecularTaskMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum MolecularTaskMessageType
	{
		UNDEFINED = -1,
		BUILD_BONDS,
		CHECK_RESIDUE,
		ADD_HYDROGENS,
		CREATE_DISTANCE_GRID
	};

	MolecularTaskMessage(MolecularTaskMessageType ) throw();
	void setType(MolecularTaskMessageType) throw();
	MolecularTaskMessageType getType() const throw();
};


class ShowDisplayPropertiesMessage
	:Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
		ShowDisplayPropertiesMessage();
};


class CreateRepresentationMessage
	:Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
		CreateRepresentationMessage() throw();
		CreateRepresentationMessage(const PyCompositeList&, 
																ModelType , 
																ColoringMethod ) throw();   
		PyCompositeList getComposites() const throw();
		ModelType getModelType() const throw();
		ColoringMethod getColoringMethod() const throw();
};
	

class NewTrajectoryMessage
	:CompositeMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
		NewTrajectoryMessage() throw();
		void setTrajectoryFile(TrajectoryFile&) throw();
		TrajectoryFile* getTrajectoryFile() throw();
};


class RegularDataMessage
	: CompositeMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

		enum RegularDataMessageType
		{
			UNDEFINED = -1,
			NEW = 100,
			REMOVE,
			UPDATE,
			SELECTED
		};

		RegularDataMessage() throw();
};

class RegularData1DMessage
	: RegularDataMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	RegularData1DMessage(RegularDataMessageType /*type = UNDEFINED*/) throw();
	void setData(RegularData1D& /*data*/) throw();
	RegularData1D* getData() throw();
};

class RegularData2DMessage
	: RegularDataMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	RegularData2DMessage(RegularDataMessageType /*type = UNDEFINED*/) throw();
	void setData(RegularData2D& /*data*/) throw();
	RegularData2D* getData() throw();
};

class RegularData3DMessage
	: RegularDataMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	RegularData3DMessage(RegularDataMessageType /*type = UNDEFINED*/) throw();
	void setData(RegularData3D& /*data*/) throw();
	RegularData3D* getData() throw();
};



