// $Id: mainControl.h,v 1.7 2000/10/24 20:33:28 oliver Exp $

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
#define BALL_VIEW_GUI_KERNEL_MAINCONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H
#	include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#include <qwidget.h>
#include <qmainwindow.h>

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;


class ModularWidget;

namespace BALL
{

	namespace VIEW
	{

		/**		MainControl class.
					{\bf Caveat:} Due to a peculiarity of the QT Meta Object Compiler (MOC)
					you have to specify the full namsespace qualified name of this classes
					when deriving from it.
					So don't use {\tt class foo : public MainControl {};} but 
					{\tt class foo : public BALL::VIEW::MainControl {};} instead.

					The MainControl is a container for the \Ref{CompositeDescriptor} and for the
					\Ref{ModularWidget}. It is therefore a storage facility for both the graphical
					representation of the inserted composites and the inserted ModularWidgets.
					It is the main adminstration unit for a program and must be used by all
					applications. It is also derived from the QT::QMainWindow and therefore the main
					widget of an application must be derived from this class.
					class.
					\\
					{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/mainControl.h}
					\\
					{\bf Category:} \Ref{CompositeDescriptor} container\\
					{\bf Category:} \Ref{ModularWidget} container\\
					{\bf Concept:} embeddable design pattern\\
					\\
		*/
		class MainControl
			: public QMainWindow,
				public ConnectionObject,
				public Embeddable
		{
			Q_OBJECT
			public:
			BALL_EMBEDDABLE(MainControl)

			/**	@name	Type Definitions
					Define a descriptor iterator
			*/
			//@{
			typedef List<CompositeDescriptor *>::iterator DescriptorIterator;
			//@}

			/**	@name Enums
			*/
			//@{
			/**	Standard Popup menu IDs.
					This enum defines symbolic names for the menu IDs of
					the most common popup menus.
					The popups are created, if a menu entry is requested for any
					of the popups.
					@see addMenuEntry
			*/
			enum PopUpID
			{
				/// File menu
				FILE        = 10001,
				FILE_IMPORT = 10002,
				/// Edit menu
				EDIT,
				/// Build
				BUILD,
				/// Display menu 
				DISPLAY,
				/// Tools menu
				TOOLS,
				/// Help menu
				HELP,
				/// Userdefined menus
				USER
			};
			//@}
			
			/**	@name Constructors and Destructors
			*/
			//@{

			/** Default Constructor
					Constructs a new MainControl
					
					@param  parent the new parent widget
					@param  name the new name of the widget
					@param  option_filename the new option filename
					@return	MainControl - new constructed MainControl				
			*/
			MainControl(QWidget* parent = 0, const char* name = 0 , String option_filename = ".BALL.options");

			/**	Copy constructor.
					BAUSTELLE: what does the copy ctor do exactly? 
					How to solve problems arising from the fact that QMainWindow's copy ctor is private?
			*/
			MainControl(const MainControl& main_control);

			/** Destructor
					Default destruction of {\em *this} MainControl.
					Calls \Ref{MainControl::destroy}
					@see  MainControl::destroy
			*/
			virtual ~MainControl();

			/** Clears all inserted \Ref{CompositeDescriptor}
			*/
			virtual void clear();

			/** Clears all inserted \Ref{CompositeDescriptor}
					Calls \Ref{MainControl::clear}
					@see  MainControl::clear
			*/
			virtual void destroy();
			//@}

			/**	@name Exceptions
			*/
			//@{
			class OptionsFileError
				: public Exception::GeneralException
			{
  			public:
	   			OptionsFileError(const char* file, int line, const string& data);
			};
			//@}

			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Inserts a new composite. 
					The use of this function indicates, that by destruction of the MainControl 
					the inserted composite can be deleted as well. Take care not to insert
					static objects or references with this function.
					@param composite the new composite
					@param name the name of the composite
					@param center the center of the composite
			*/
			void insert
				(Composite* composite,
				 const String& name = "unkown",
				 const Vector3& center = Vector3(0,0,0));

			/** Inserts a new composite.
					With this function static composites or references of composites can be inserted.
					If you want that the inserted composite should be destroyed by destruction of
					the MainControl you should use the function above.
					@param composite the new composite
					@param name the name of the composite
					@param center the center of the composite
					@return CompositeDescriptor* - pointer to the new compositeDescriptor
			*/
      CompositeDescriptor* insert
        (const Composite& composite,
         const String& name = "unkown",
				 const Vector3& center = Vector3(0,0,0));
 
			/** Inserts a new \Ref{CompositeDescriptor} composite_descriptor.
					The {\em composite_descriptor} is copied according to the flag
					{\em deep}. If {\em deep} = {\tt true} then the composite_descriptor is
					copied deep otherwise a shallow CompositeDescriptor is created.
					@param composite_descriptor the new CompositeDescriptor to be inserted
					@param deep the flag that states if a deep (default) or shallow copy should be made
					@return CompositeDescriptor* - mutable pointer to the new created CompositeDescriptor
			*/
			CompositeDescriptor* insert
				(CompositeDescriptor& composite_descriptor, bool deep = true);

			/** Removes a given \Ref{Composite}.
					@param  composite the composite to be removed
					@return bool - indicated if the remove operation was successfull
			*/
			bool remove
				(const Composite& composite, bool sent_message = true);

			/** Removes a given \Ref{CompositeDescriptor}.
					@param  composite_descriptor the composite_descriptor to be removed
					@param  sent_message a boolean flag that indicates, whether a message of the action should be sent or not.
					@return bool - indicated if the remove operation was successfull
			*/
			bool remove
				(const CompositeDescriptor& composite_descriptor);

			/** Mutable inspection of the \Ref{CompositeDescriptor} of a given \Ref{Composite}.
					Access a mutable reference to the CompositeDescriptor of {\em composite}.
					@param   composite the composite whose CompositeDescriptor should be returned
					@return  CompositeDescriptor* - mutable pointer to the CompositeDescriptor
			*/
			CompositeDescriptor* getDescriptor
				(const Composite& composite);

			/** Constant inspection of the \Ref{CompositeDescriptor} of a given \Ref{Composite}.
					Access a constant reference to the CompositeDescriptor of {\em composite}.
					@param   composite the composite whose CompositeDescriptor should be returned
					@return  CompositeDescriptor* - constante pointer to the CompositeDescriptor
			*/
			const CompositeDescriptor* getDescriptor
				(const Composite& composite) const;
			
			/** Mutable inspection of the list of all \Ref{CompositeDescriptor} of the MainControl
					Access a mutable reference to the list of CompositeDescriptors.
					@return List<CompositeDescriptor*>& - mutable reference to the list of CompositeDescriptor					
			*/
			List<CompositeDescriptor*>& getDescriptorList();

			/** Constant inspection of the list of all \Ref{CompositeDescriptor} of the MainControl
					Access a constant reference to the list of CompositeDescriptors.
					@return List<CompositeDescriptor*>& - constant reference to the list of CompositeDescriptor					
			*/
			const List<CompositeDescriptor*>& getDescriptorList() const;

			/** Change of the Composite's name.
					Change the name of {\em composite} to {\em name}.
					@param  composite the composite whose name will be changed
					@param  name the new name
					@return bool - indicates if the operation was successful
					@see    MainControl::getName
			*/
			bool setName
				(const Composite& composite, const String& name);

			/** Mutable inspection of the name.
					Access a mutable pointer to the name of {\em composite}.
					@param   composite the composite whose name should be retrieved
					@return  String* - mutable pointer to the name (0 indicates that no such composite is inserted)
					@see     MainControl::setName
			*/
			String* getName(const Composite& composite);

			/** Constant inspection of the name.
					Access a constant pointer to the name of {\em composite}.
					@param   composite the composite whose name should be retrieved
					@return  String* - constant pointer to the name (0 indicates that no such composite is inserted)
					@see     MainControl::setName
			*/
			const String* getName(const Composite& composite) const;

			/** Mutable inspection of the \Ref{CompositeDescriptor} of a given name.
					Access a mutable pointer to the descriptor of a composite with a given name.
					@param   name the name whose CompositeDescriptor should be retrieved
					@return  CompositeDescriptor* - mutable pointer to the CompositeDescriptor (0 indicates that no such composite with such a name exists)
			*/
			CompositeDescriptor* getDescriptor(const String& name);

			/** Constant inspection of the \Ref{CompositeDescriptor} of a given name.
					Access a constant pointer to the descriptor of a composite with a given name.
					@param   name the name whose CompositeDescriptor should be retrieved
					@return  CompositeDescriptor* - constant pointer to the CompositeDescriptor (0 indicates that no such composite with such a name exists)
			*/
			const CompositeDescriptor* getDescriptor(const String& name) const;

			/** Change of the Composite's center.
					Change the center of {\em composite} to the center {\em v}.
					@param  composite the composite whose center should be changed
					@param  v the new center
					@return bool - indicates if the operation was successful
					@see    MainControl::getCenter
			*/
			bool setCenter(const Composite& composite, const Vector3& v);

			/** Mutable inspection of the center.
					Access a mutable pointer to the center of {\em composite}.
					@param   composite the composite whose center should be retrieved
					@return  Vector3* - mutable pointer to the center (0 indicates that no such composite exists)
					@see     MainControl::setCenter
			*/
			Vector3* getCenter(const Composite& composite);

			/** Constant inspection of the center.
					Access a constant pointer to the center of {\em composite}. 
					@param   composite the composite whose center should be retrieved
					@return  Vector3* - constant pointer to the center of {\em composite} (0 indicates that no such composite exists)
					@see     MainControl::setCenter
			*/
			const Vector3* getCenter(const Composite& composite) const;

			/** Mutable inspection of the \Ref{CompositeDescriptor} for a given center.
					Access a mutable pointer to the CompositeDescriptor of a {\em center}.
					@param   center the center of a CompositeDescriptor
					@return  CompositeDescriptor* - mutable pointer to a CompositeDescriptor (0 indicates that no such CompositeDescriptor exists)
					@see     MainControl::setCenter
			*/
			CompositeDescriptor* getDescriptor(const Vector3& center);

			/** Constant inspection of the \Ref{CompositeDescriptor} for a given center.
					Access a constant pointer to the CompositeDescriptor of a {\em center}.
					@param   center the center of a CompositeDescriptor
					@return  CompositeDescriptor* - constant pointer to a CompositeDescriptor (0 indicates that no such CompositeDescriptor exists)
					@see     MainControl::setCenter
			*/
			const CompositeDescriptor* getDescriptor(const Vector3& center) const;

			/** Marks the \Ref{CompositeDescriptor} of {\em composite} for update.
					@param  composite the composite whose CompositeDescriptor should be updated
					@return bool - indicates if the operation was successful
			*/
			bool update(const Composite& composite);

			/** Marks all \Ref{CompositeDescriptor}`s for update.
					@see MainControl::update
			*/
			void updateAll();

			/** Returns a pointer to the options
			*/
			virtual Options *getOptions();

			/** Handles messages sent by other \Ref{ConnectionObject}`s.
					Virtual function for overriden the message handling system. Take care to call
					this function when your own {\em onNotify} function is done. This ensured that 
					the message handling mechanism continues.
					There is no need to call this function, because it will be called from the 
					message handling mechanism.
					@param message the pointer to the message the should be processed
		  */
			virtual void onNotify(Message *message);
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Determines if the given {\em composite} is already inserted.
					@param composite the composite that should be checked
					@return bool - indicates if the composite is inserted or not
			*/
			bool isInserted(const Composite& composite) const;

			/** Determines if the given {\em compositeDescriptor} is already inserted.
					@param compositeDescriptor the compositeDescriptor that should be checked
					@return bool - indicates if the compositeDescriptor is inserted or not
			*/
			bool isInserted(const CompositeDescriptor& compositeDescriptor) const;
			//@}

			/** @name Public slots
			*/
			public slots:
			//@{
			/**	Initialize the modular widgets and display the main window.
			*/
			virtual void show();

			/** Checks, enables or disables all the inserted menu entries
					as the checkMenu-Function of the inserted ModularWidgets indicates.
			*/
			virtual void checkMenus();
			//@}

			public:
			
			/**	@name	Debugging and Diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} MainControl.
					If the internal state of {\em *this} MainControl is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					@return			bool -
											{\tt true} if the internal state of {\em *this} MainControl is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const;

			/** Internal state dump.
					Dump the current internal state of {\em *this} MainControl to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s - output stream where to output the internal state of {\em *this} MainControl
					@param   depth - the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{
			/** Persistent stream input and state restorage.
					Not yet implemented
			*/
			virtual void read(std::istream& s);

			/* Persistent stream output and state storage.
 				 Not yet implemented
			*/
			virtual void write(std::ostream& s) const;
			//@}

			/**	@name	Automatic module registration and menu construction.
			*/
			//@{
			/**
			*/
			static MainControl* getMainControl(const QWidget* widget);
			
			/**
			*/
			void removeMenuEntry
				(int ID, const String& name, const QObject* receiver = 0, 
				 const char* slot = 0, int accel = 0, int entry_ID = -1);
 
			/**
			*/
			int insertMenuEntry
				(int ID, const String& name, const QObject* receiver = 0, 
				 const char* slot = 0, int accel = 0, int entry_ID = -1);
 
			/**	Create a unique item ID.
			*/
			int getNextID();

			/**	Initialize one of the popup menus.
					The ID is one of the enum PopUpID
			*/	
			virtual QPopupMenu* initPopupMenu(int ID);
				
			/**
			*/
			void addModularWidget(ModularWidget* widget);

			/**
			*/
			void removeModularWidget(ModularWidget* widget);
			//@}
			
			
			protected:

			void register_();

			void unregister_();

			private:

			CompositeDescriptor* insert_
				(const Composite& composite,
				 const String& name,
				 const Vector3& v);

			CompositeDescriptor* insert_
				(CompositeDescriptor& composite_descriptor,
				 bool deep = true);

			typedef HashMap<void*, List<CompositeDescriptor*>::Iterator>
				ListIteratorHashMap;

			ListIteratorHashMap composite_map_;
			ListIteratorHashMap descriptor_map_;
			List<CompositeDescriptor*> descriptors_;

			Options options_;
			
			static int current_id_;
			
			// used to acces the global instance of MainControl
			static MainControl* theMainControl_;

			/**	A list containing all modular widgets.
					This list is modified by \Ref{addModularWidget} and
					\Ref{removeModularWidget}.
			*/
			List<ModularWidget*>	modular_widgets_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/mainControl.iC>
#		endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
