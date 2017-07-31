class Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:
	Message() throw();
	Message(const Message&) throw();
};

class CompositeMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:

	enum CompositeMessageType
	{
		UNDEFINED = -1,
		NEW_COMPOSITE,
		REMOVED_COMPOSITE,
		CHANGED_COMPOSITE,
		CHANGED_COMPOSITE_HIERARCHY,
		SELECTED_COMPOSITE,
		DESELECTED_COMPOSITE,
		CENTER_CAMERA,
		NEW_MOLECULE
	};

	CompositeMessage() throw();
	CompositeMessage(const Composite& , CompositeMessageType , bool = true) throw();
	CompositeMessage(const CompositeMessage&) throw();
	void setComposite(const Composite&) throw();
	Composite* getComposite() const throw();
	void setCompositeName(const String&) throw();
	const String& getCompositeName() const throw();
	void setType(CompositeMessageType) throw();
	CompositeMessageType getType() const throw();
	void setUpdateRepresentations(bool) throw();
	bool updateRepresentations() const throw();
	void setShowSelectionInfos(bool) throw();
	bool showSelectionInfos() throw();
};

class SceneMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:

	enum SceneMessageType
	{
		UNDEFINED = 0,
		REBUILD_DISPLAY_LISTS,
		REDRAW,
		UPDATE_CAMERA,
		REMOVE_COORDINATE_SYSTEM,
		EXPORT_PNG,
		EXPORT_POVRAY,
		ENTER_ROTATE_MODE,
		ENTER_PICKING_MODE,
		ENTER_MOVE_MODE
	};
	SceneMessage(SceneMessageType = UNDEFINED) throw();
	SceneMessage(const SceneMessage&) throw();

	void setType(SceneMessageType) throw();
	SceneMessageType getType() const throw();
	void setStage(Stage) throw();
	Stage& getStage() throw();
};

class GenericSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:
	GenericSelectionMessage() throw();
	GenericSelectionMessage(const GenericSelectionMessage& ) throw();
	void setSelection(const PyCompositeList&) throw();
	PyCompositeList getSelection() const throw();
};

class ControlSelectionMessage
	: GenericSelectionMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:
	ControlSelectionMessage() throw();
};

class NewSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:
	NewSelectionMessage() throw();
	bool openItems() throw();
	void setOpenItems(bool) throw();
};

class GeometricObjectSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	#include <list>
	using std::list;
%End
	public:
	GeometricObjectSelectionMessage() throw();
	void setSelection(const list<GeometricObject*>& ) throw();
	const list<GeometricObject*>& getSelection() const throw();
	void setSelected(bool ) throw();
	bool isSelected() const throw();
};

class RepresentationMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:

	enum RepresentationMessageType
	{
		UNDEFINED = -1,
		ADD = 0,
		REMOVE,
		UPDATE,
		SELECTED,
		STARTED_UPDATE,
		FINISHED_UPDATE,
		ADD_TO_GEOMETRIC_CONTROL
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

	MolecularTaskMessage(MolecularTaskMessageType = UNDEFINED) throw();
	void setType(MolecularTaskMessageType) throw();
	MolecularTaskMessageType getType() const throw();
};


class ShowDisplayPropertiesMessage
	:Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:
	ShowDisplayPropertiesMessage();
};


class CreateRepresentationMessage
	:Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End

	public:
	CreateRepresentationMessage() throw();
	CreateRepresentationMessage(const PyCompositeList&, ModelType,
		ColoringMethod) throw();
	ModelType getModelType() const throw();
	ColoringMethod getColoringMethod() const throw();
};

class DeselectControlsMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
%End
	public:
	DeselectControlsMessage();
};
