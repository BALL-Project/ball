// $Id: mainControl.h,v 1.19 2001/07/25 11:30:08 oliver Exp $

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

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
#	include <BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
#	include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H
#	include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#include <qapplication.h>
#include <qobject.h>
#include <qwidget.h>
#include <qmainwindow.h>

//using namespace std;
//using namespace BALL;
//using namespace BALL::VIEW;


//class ModularWidget;

namespace BALL
{

	namespace VIEW
	{
		class ModularWidget;

		/**	MainControl class.
				{\bf Caveat:} Due to a peculiarity of the QT Meta Object Compiler (MOC)
				you have to specify the full namsespace qualified name of this classes
				when deriving from it.
				So don't use {\tt class foo : public MainControl {};} but 
				{\tt class foo : public BALL::VIEW::MainControl {};} instead. \\ \\
				The class MainControl is a container for the \Ref{CompositeDescriptor} and the
				\Ref{ModularWidget}. It is therefore a storage facility for both the graphical
				representation of the inserted composites and the inserted ModularWidgets.
				It hides the \Ref{CompositeDescriptor} mechanism so that \Ref{Composite} objects
				can be used instead. There are interface methods to access, update and change
				\Ref{Composite} objects. There are also a number of methods to directly access
				\Ref{CompositeDescriptor} objects. These methods are only internally used and
				should only be used with care. \\
				This class is also the root \Ref{ConnectionObject}. Therefore all messages will
				be handled from this class. \\
				The class MainControl is the main adminstration unit for a program and must be
				used by all	applications.
				It is also derived from the QT::QMainWindow and therefore the main
				widget of an application must be derived from this class. Further it has the
				necessary interface methods to create and update the menus of the main application.
				It handles also the general preferences tab \Ref{Preferences} of the main
				application and notifies all
				registered \Ref{ModularWidget} objects if the preferences has changed.
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/mainControl.h}
		*/
		class MainControl
			: public QMainWindow,
				public ConnectionObject,
				public Embeddable
		{
			Q_OBJECT

			public:

			/** @name Applied concepts
			*/
			//@{
			/** Embeddable concept.
					See class \Ref{Embeddable} for information concerning embeddable objects.
			*/
			BALL_EMBEDDABLE(MainControl)
			//@}

			/**	@name	Type Definitions
			*/
			//@{
			
			/** Definition of a descriptor iterator.
			*/
			typedef List<CompositeDescriptor *>::iterator DescriptorIterator;
			//@}

			/**	@name Enumerations
			*/
			//@{
			/**	Standard Popup menu IDs.
					This enum defines symbolic names for the menu IDs of
					the most common popup menus.
					The popups are created, if a menu entry is requested for any
					of the popups.
					@see insertMenuEntry
			*/
			enum PopUpID
			{
				/// File menu
				FILE        = 10001,

				/// File menu sub menu import
				FILE_IMPORT = 10002,

				/// Edit menu
				EDIT,

				/// Build menu 
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
			
			/**	@name Exceptions
			*/
			//@{

			/** PreferencesError exception class.
					This exception is thrown if the memory allocation for the preferences dialog
					\Ref{Preferences} failed.
					@see GeneralException
			*/
			class PreferencesError: public Exception::GeneralException
			{
  			public:
	   			PreferencesError(const char* file, int line, const string& data)
						throw();
			};
			//@}

			/**	@name	Constructors and Destructor
			*/
			//@{

			/** Default Constructor.
					Construct a new MainControl.
					Read the the \Ref{INIFile} {\em inifile} and connects the qt signal 
					{\em aboutToQuit} with the slot \Ref{aboutToExit}.
					Initializes the internal data structures.
					The state of {\em *this} mainControl is:
					\begin{itemize}
					  \item  no \Ref{Composite} objects stored
						\item  no general \Ref{Preferences} dialog added
						\item  no \Ref{MainControlPreferences} dialog added
					\end{itemize}
					@param  parent the new parent widget
					@param  name the new name of the widget
					@param  inifile the new preferences filename
					@return	MainControl new constructed MainControl		
					@see    Composite
					@see    INIFile
					@see    Preferences
					@see    MainControlPreferences
			*/
			MainControl(QWidget* parent = 0, const char* name = 0 , String inifile = ".BALL.preferences")
				throw();

			/** Destructor.
					Default destruction of {\em *this} MainControl.
					Calls \Ref{destroy}
					@see  destroy
			*/
			virtual ~MainControl()
				throw();
					
			// copy ctor needed for Python support only!
			MainControl(const MainControl& main_control)
				throw();

			/** Explicit default initialization.
					Clears all inserted \Ref{CompositeDescriptor}
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Clears all inserted \Ref{CompositeDescriptor}
					Calls \Ref{clear}
					@see  clear
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Insert a new composite (with deletion). 
					Insert a new \Ref{Composite} object to {\em *this} mainControl.
					Every \Ref{Composite} object can be given a name for later
					identification and a geometric center used for drawing the graphical
					representation of {\em composite}. These additional values will be stored
					in an associated \Ref{CompositeDescriptor} object.
					If this method is used to insert composites the composite objects will
					be deleted by the destruction of {\em *this} mainControl. If {\em composite}
					is already inserted it will be removed and reinserted.\\
					{\bf Note:} do not insert statically created \Ref{Composite} objects with
					this method.
					@param composite the new \Ref{Composite} object to be inserted into {\em *this} mainControl
					@param name the name the \Ref{Composite} object should have (Default: {\tt "unkown"})
					@param center the geometric center the \Ref{Composite} object should have (Default: {\tt Vector3(0,0,0)}) 
					@see    Composite
					@see    CompositeDescriptor
					@see    String
					@see    Vector3
			*/
			void insert
				(Composite* composite,
				 const String& name = "unkown",
				 const Vector3& center = Vector3(0,0,0))
				throw();

			/** Insert a new composite (without deletion).
					Insert a new \Ref{Composite} object to {\em *this} mainControl.
					If this method is used to insert composites. The composite objects will
					not be deleted by the destruction of {\em *this} mainControl.
					This method returns the \Ref{CompositeDescriptor} object that is
					created for the given \Ref{Composite} object. See \Ref{CompositeDescriptor}
					for further information.
					If {\em composite} is already inserted {\tt 0} will be returned.\\
					{\bf Note:} statically created \Ref{Composite} objects can be inserted with
					this method.
					@param composite the new \Ref{Composite} object to be inserted into {\em *this} mainControl
					@param name the name the \Ref{Composite} object should have (Default: {\tt "unkown"})
					@param center the geometric center the \Ref{Composite} object should have (Default: {\tt Vector3(0,0,0)}) 
					@return CompositeDescriptor* pointer to the new compositeDescriptor
					@see    insert
					@see    Composite
					@see    CompositeDescriptor
					@see    String
					@see    Vector3
			*/
      CompositeDescriptor* insert
        (const Composite& composite,
         const String& name = "unkown",
				 const Vector3& center = Vector3(0,0,0))
				throw();
 
			/** Insert a new composite_descriptor.
					The \Ref{CompositeDescriptor} {\em composite_descriptor}
					is copied according to the flag {\em deep}.
					If {\em deep} is set to {\tt true} then {\em composite_descriptor} is
					copied deep otherwise a shallow \Ref{CompositeDescriptor} is created.
					See \Ref{CompositeDescriptor} for information concerning deep and shallow
					copies.
					If {\em composite_descriptor} is already inserted and a shallow copy
					should be made {\tt 0} will be returned.
					@param composite_descriptor the new \Ref{CompositeDescriptor} to be inserted into {\em *this} mainControl
					@param deep if {\tt true} then a deep (default) copy of {\em composite_descriptor} is made, {\tt false} a shallow copy is made
					@return CompositeDescriptor* mutable pointer to the new created \Ref{CompositeDescriptor}
					@see    Composite
					@see    CompositeDescriptor
			*/
			CompositeDescriptor* insert(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			/** Remove a composite.
					Remove the \Ref{Composite} {\em composite} from {\em *this} mainControl.
					A notification message \Ref{RemoveCompositeMessage} can be sent to the
					children of {\em *this} mainControl. See \Ref{ConnectionObject} for
					information concerning message queuing and responding.
					@param  composite the \Ref{Composite} to be removed from {\em *this} mainControl
					@param  sent_message if set to {\tt true} {\em *this} mainControl sents a \Ref{RemoveCompositeMessage} through the \Ref{ConnectionObject} tree
					@return bool {\tt true} the \Ref{Composite} was successfully removed, {\tt false} otherwise
					@see    insert
					@see    Composite
					@see    RemoveCompositeMessage
					@see    ConnectionObject
			*/
			bool remove(const Composite& composite, bool sent_message = true)
				throw();

			/** Remove a compositeDescriptor.
					Remove the \Ref{CompositeDescriptor} {\em composite_descriptor} from 
					{\em *this} mainControl. No notification message is sent in this method.			
					@param  composite_descriptor the \Ref{CompositeDescriptor} to be removed from {\em *this} mainControl
					@return bool {\tt true} if {\em composite_descriptor} was successfully removed, {\tt false} otherwise
					@see    insert
					@see    remove
					@see    CompositeDescriptor
			*/
			bool remove(const CompositeDescriptor& composite_descriptor)
				throw();

			/** Mutable inspection of a compositeDescriptor.
					Access a mutable pointer to the \Ref{CompositeDescriptor} of the inserted
					\Ref{Composite} {\em composite}. If {\em composite} is not inserted into
					{\em *this} mainControl then {\tt 0} is returned.
					@param   composite the inserted \Ref{Composite} whose \Ref{CompositeDescriptor} should be returned
					@return  CompositeDescriptor* mutable pointer to \Ref{CompositeDescriptor}, {\tt 0} if {\em composite} is not inserted into {\em *this} mainControl
					@see     insert
					@see     Composite
					@see     CompositeDescriptor
			*/
			CompositeDescriptor* getDescriptor(const Composite& composite)
				throw();

			/** Non-mutable inspection of a compositeDescriptor.
					For further information see \Ref{getDescriptor}.
			*/
			const CompositeDescriptor* getDescriptor(const Composite& composite) const
				throw();
			
			/** Mutable inspection of of all compositeDescriptor objects.
					Access a mutable reference to the list of \Ref{CompositeDescriptor} objects
					of {\em *this} mainControl. Empty if {\em *this} mainControl has no objects
					yet.			
					@return List<CompositeDescriptor*>& mutable reference to the list of \Ref{CompositeDescriptor} objects of {\em *this} mainControl
					@see     insert
					@see     getDescriptor
					@see     CompositeDescriptor
			*/
			List<CompositeDescriptor*>& getDescriptorList()
				throw();

			/** Non-mutable inspection of of all compositeDescriptor objects.
					For further information see \Ref{getDescriptorList}.
			*/
			const List<CompositeDescriptor*>& getDescriptorList() const
				throw();

			/** Change the name of a composite.
					Change the name of {\em composite} to {\em name}.					
					@param  composite the \Ref{Composite} whose name will be changed
					@param  name the new name
					@return bool {\tt true} if {\em composite} is inserted in {\em *this} mainControl and the assignment was successful, {\tt false} otherwise
					@see    getName
					@see    insert
					@see    Composite
					@see    String
			*/
			bool setName(const Composite& composite, const String& name)
				throw();

			/** Mutable inspection of the name.
					Access a mutable pointer to the name of {\em composite}. If the 
					{\em composite} is not inserted into {\em *this} mainControl then {\tt 0}
					is returned.
					@param   composite the \Ref{Composite} whose name should be retrieved
					@return  String* mutable pointer to the name ({\tt 0} if no such composite is inserted)
					@see     setName
					@see     insert
					@see     Composite
					@see     String
			*/
			String* getName(const Composite& composite)
				throw();

			/** Non-mutable inspection of the name.
					For further information see \Ref{getName}.
			*/
			const String* getName(const Composite& composite) const
				throw();

			/** Mutable inspection of a compositeDescriptor to a given name.
					Access a mutable pointer to the \Ref{CompositeDescriptor} of a
					\Ref{Composite} inserted into {\em *this} mainControl with a given string
					{\em name}.
					If more such names are available then the first found 
					\Ref{CompositeDescriptor} with this {\em name} will be returned.
					@param   name the name whose \Ref{CompositeDescriptor} should be retrieved
					@return  CompositeDescriptor* mutable pointer to the \Ref{CompositeDescriptor} ({\tt 0} if no such \Ref{Composite} with such a name exists)
					@see     setName
					@see     insert
					@see     Composite
					@see     CompositeDescriptor
					@see     String
			*/
			CompositeDescriptor* getDescriptor(const String& name)
				throw();

			/** Non-mutable inspection of a compositeDescriptor to a given name.
					For further information see \Ref{getDescriptor}.
			*/
			const CompositeDescriptor* getDescriptor(const String& name) const
				throw();

			/** Change of the center vector.
					Change the center vector of the \Ref{Composite} {\em composite} 
					to the \Ref{Vector3} {\em v}.
					@param  composite the \Ref{Composite} whose center vector should be changed
					@param  v the \Ref{Vector3} that is the new center vector
					@return bool {\tt true} if {\em composite} is inserted in {\em *this} mainControl and the assignment was successful, {\tt false} otherwise
					@see    getCenter
					@see    insert
					@see    Composite
					@see    Vector3
			*/
			bool setCenter(const Composite& composite, const Vector3& v)
				throw();

			/** Mutable inspection of the center vector.
					Access a mutable pointer to the center \Ref{Vector3} of the given
					\Ref{Composite} {\em composite}.
					@param   composite the \Ref{Composite} whose center vector should be retrieved
					@return  Vector3* mutable pointer to the center vector ({\tt 0} if no such \Ref{Composite} exists)
					@see     setCenter
					@see     insert
					@see     Composite
					@see     Vector3
			*/
			Vector3* getCenter(const Composite& composite)
				throw();

			/** Non-mutable inspection of the center vector.
					For further information see \Ref{getCenter}.
			*/
			const Vector3* getCenter(const Composite& composite) const
				throw();

			/** Mutable inspection of a compositeDescriptor to a given center vector.
					Access a mutable pointer to the \Ref{CompositeDescriptor} of a
					\Ref{Composite} inserted into {\em *this} mainControl with a given 
					\Ref{Vector3} {\em center}.
					If more such center \Ref{Vector3} are available then the first found 
					\Ref{CompositeDescriptor} with this {\em center} vector will be returned.
					@param   center the center \Ref{Vector3} whose \Ref{CompositeDescriptor} should be retrieved
					@return  CompositeDescriptor* mutable pointer to the \Ref{CompositeDescriptor} ({\tt 0} if no such \Ref{Composite} with such a center vector exists)
					@see     setCenter
					@see     insert
					@see     Composite
					@see     CompositeDescriptor
					@see     Vector3
			*/
			CompositeDescriptor* getDescriptor(const Vector3& center)
				throw();

			/** Non-mutable inspection of a compositeDescriptor to a given center vector.
					For further information see \Ref{getDescriptor}.
			*/
			const CompositeDescriptor* getDescriptor(const Vector3& center) const
				throw();

			/** Mark a composite for update.
					If a \Ref{Composite} was marked for update then the graphical representation
					of {\em composite} will be created anew when the next time the render engine
					will draw this \Ref{Composite}.
					If {\em composite} is inserted into {\em *this} mainControl {\tt true} will be
					returned, otherwise {\tt false}.
					@param  composite the \Ref{Composite} that should be updated
					@return bool {\tt true} if {\em composite} was successfully marked for update, {\tt false} otherwise
			*/
			bool update(const Composite& composite)
				throw();

			/** Marks all inserted composite objects for update.
					Same as \Ref{update} but all inserted \Ref{Composite} objects
					are marked for update.
					@see update
			*/
			void updateAll()
				throw();

			/** Mutable inspection of the preferences dialog.
					Access the mutual pointer of the general \Ref{Preferences} dialog
					of {\em *this} mainControl.
					@return   Preferences* a pointer to the \Ref{Preferences} dialog, ({\tt 0} if not present)
					@see      Preferences
			*/
			Preferences* getPreferences()
				throw();

			/** Mutable inspection of the inifile.
					Access the mutual reference of the \Ref{INIFile} of {\em *this} mainControl.
					@return      INIFile& mutable reference to the inifile of {\em *this} mainControl
					@see         INIFile
			*/
			INIFile& getINIFile()
				throw();

			/** Non-mutable inspection of the inifile.
					For further information see \Ref{getINIFile}.
			*/
			const INIFile& getINIFile() const
				throw();

			/** Message handling method.
					Handles messages sent by other registered \Ref{ModularWidget} objects.
					Virtual function for overriden the message handling system. Take care to call
					this function when your own {\em onNotify} function is done. This ensures that 
					the message handling mechanism continues.
					There is no need to call this function, because it will be called from the 
					message handling mechanism.
					@param message the pointer to the message the should be processed
					@see   ModularWidget
					@see   Message
		  */
			virtual void onNotify(Message *message)
				throw();
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Test if a given composite is inserted.
					Determines if the given \Ref{Composite} {\em composite} is already inserted into
					{\em *this} mainControl.
					@param composite the \Ref{Composite} that should be checked
					@return bool {\tt true} if {\em composite} is inserted, {\tt false} otherwise
			*/
			bool isInserted(const Composite& composite) const
				throw();

			/** Test if a given compositeDescriptor is inserted.
					Determines if the given \Ref{CompositeDescriptor} {\em compositeDescriptor}
					is already inserted into {\em *this} mainControl.
					@param compositeDescriptor the \Ref{CompositeDescriptor} that should be checked
					@return bool {\tt true} if {\em compositeDescriptor} is inserted, {\tt false} otherwise
			*/
			bool isInserted(const CompositeDescriptor& compositeDescriptor) const
				throw();
			//@}

			public slots:

			/** @name Public slots
			*/
			//@{

			/**	Modular widget initialization.
					This method initializes all registered \Ref{ModularWidget} objects.
					It initializes the menu structure, the preferences dialogs and connects
					every \Ref{ModularWidget} with {\em *this} mainControl.
					This method also creates the first menu entry {\em FILE} with its subentry
					{\em EXIT} to have a means to exit the application.
					See \Ref{ModularWidget} for further information concerning menu structure creation
					and preferences handling of \Ref{ModularWidget} objects.
					Calls \Ref{registerConnectionObject}
					Calls \Ref{initializePreferencesTab}
					Calls \Ref{fetchPreferences}
					Calls \Ref{applyPreferences}
					Calls \Ref{insertMenuEntry}
					Calls \Ref{ModularWidget::initializeWidget}
					Calls \Ref{ModularWidget::initializePreferencesTab}
					Calls \Ref{ModularWidget::fetchPreferences}
					Calls \Ref{ModularWidget::applyPreferences}
					Calls \Ref{Preferences::fetchPreferences}
					Calls \Ref{QMainWindow::show}\\
					{\bf Note:}Call this method to start the application.
					@exception  PreferencesError thrown if the \Ref{Preferences} dialogs cannot be allocated.
					@see        registerConnectionObject
				  @see        ModularWidget
					@see        initializePreferencesTab
					@see        fetchPreferences
					@see        applyPreferences
					@see        insertMenuEntry
					@see        ModularWidget::initializeWidget
					@see        ModularWidget::initializePreferencesTab
					@see        ModularWidget::fetchPreferences
					@see        ModularWidget::applyPreferences
					@see        Preferences::fetchPreferences
					@see        QMainWindow::show
			*/
			virtual void show();

			/** Menu checking method.
					This method checks, enables or disables all inserted menu entries of the
					associated \Ref{ModularWidget} objects registered into {\em *this} mainControl.
					If this method is overridden make sure that it will be called at the end of
					the new {\em checkMenus} method.
					See \Ref{ModularWidget} for further information concerning menu structure creation
					of \Ref{ModularWidget} objects.
					Calls \Ref{ModularWidget::checkMenu}\\
					{\bf Note:} This method will be called internally whenever the menu structure
					needs an update.
				  @see        ModularWidget
				  @see        ModularWidget::checkMenu
			*/
			virtual void checkMenus();

			/** Apply preferences.
					This method calls the method {\em applyPreferences} of all registered
					\Ref{ModularWidget} objects if the apply button of the \Ref{Preferences} dialog
					of {\em *this} mainControl is pressed. It processes also the own method
					{\em applyPreferences}.
					Calls \Ref{ModularWidget::applyPreferences}
					Calls \Ref{applyPreferences}\\
					{\bf Note:} This method will be called internally whenever the apply button
					of the \Ref{Preferences} dialog	is pressed.
				  @see        ModularWidget
				  @see        ModularWidget::applyPreferences
				  @see        applyPreferences
			*/
			virtual void applyPreferencesTab();

			/** Last second cleanup.
					This method stores the preferences and finalizes all \Ref{ModularWidget} objects
					and {\em *this} mainControl.
					Must be called after your own cleanup routine if you override this method.
					Calls \Ref{ModularWidget::writePreferences}
					Calls \Ref{ModularWidget::finalizePreferencesTab}
					Calls \Ref{ModularWidget::finalizeWidget}
					Calls \Ref{Preferences::writePreferences}
					Calls \Ref{writePreferences}
					Calls \Ref{finalizePreferencesTab}
					Calls \Ref{removeModularWidget}
					Calls \Ref{INIFile::write}\\
					{\bf Note:} This method will be called internally if {\em *this} mainControl
					will be destroyed.
				  @see   ModularWidget
					@see   ModularWidget::writePreferences
					@see   ModularWidget::finalizePreferencesTab
					@see   ModularWidget::finalizeWidget
					@see   Preferences::writePreferences
					@see   writePreferences
					@see   finalizePreferencesTab
					@see   removeModularWidget
					@see   INIFile::write
			*/
			virtual void aboutToExit();
			//@}

			public:
			
			/**	@name	Automatic module registration, menu construction and preferences handling.
			*/
			//@{

			/** Return the mainControl.
					This method returns {\em *this} mainControl that should be the root of the
					\Ref{ConnectionObject} tree from a given widget or dialog \Ref{QObject}
					{\em object}. Because we use the qt library every widget or dialog has 
					\Ref{QObject} as a base class. MainControl is the main application and therefore
					all widgets and dialogs are children of {\em *this} mainControl. We use the qt
					\Ref{QObject} tree mechanism to return {\em *this} mainControl from a given
					\Ref{QObject}.\\
					{\bf Note:} this method is used internally from the \Ref{ModularWidget} 
					registration process.
					@return   MainControl* {\em *this} mainControl, the root of the \Ref{ConnectionObject} tree
					@see      ConnectionObject
					@see      ModularWidget
			*/
			static MainControl* getMainControl(const QObject* object)
				throw();
			
			/** Remove a menu entry.
					Remove a menu entry from {\em *this} mainControl.\\
					{\bf Note:} not yet implemented.
					@param ID the menu ID which menu should be removed
					@param name the name of the menu entry
					@param receiver the object to which the menu action is connected
					@param slot the function that will is called by activation of the menu entry
					@param accel the acceleration key
					@param entry_ID the id of the menu entry
					@see   insertMenuEntry
			*/
			void removeMenuEntry
				(int ID, const String& name, const QObject* receiver = 0, 
				 const char* slot = 0, int accel = 0, int entry_ID = -1)
				throw();
 
			/** Insert a new menu entry.
					Insert a new menu entry into {\em *this} mainControl to menu {\em ID} 
					(creates a new menu if {\em ID} not existent).
					See the documentation of the qt library for more information concerning
					menu creation.
					@param ID the menu ID to which the new menu entry should be inserted
					@param name the name of the new menu entry
					@param receiver the object to which the menu action will be connected
					@param slot the function that will be called by activation of the menu entry
					@param accel the acceleration key
					@param entry_ID the id for the new menu entry (default: -1, will create a new one)
					@return int the new entry_ID
					@see   removeMenuEntry
			*/
			int insertMenuEntry
				(int ID, const String& name, const QObject* receiver = 0, 
				 const char* slot = 0, int accel = 0, int entry_ID = -1)
				throw();
			
			/**	Initialize a popup menu.
					Initialize a new popup menu {\em ID}. If {\em *this} mainControl has already
					the popup menu {\em ID} that \Ref{QPopupMenu} is returned.
					See the documentation of the qt library for more information concerning
					the class QPopupMenu.
					@param    ID the ID of the menu entry to be created.
					@return   QPopupMenu* a pointer to the created \Ref{QPopupMenu}
					@see      PopUpID
					@see      QPopupMenu
			*/	
			virtual QPopupMenu* initPopupMenu(int ID)
				throw();

			/** Insert a separator into a popup menu.
					Insert a separator into the popup menu {\em ID}. If the menu {\em ID} is not
					existent, it will be created first.
					@param ID the id of the menu to which a separator will be inserted
					@see   PopUpID
			*/
			void insertPopupMenuSeparator(int ID)
				throw();

			/** Initialize a preferences tab for the mainControl.
					This method creates the preferences tab \Ref{MainControlPreferences} for
					{\em *this} mainControl and inserts it into the \Ref{Preferences} dialog.
					This method is called automatically by the \Ref{show} method at the
					start of the application. This method is used in the same manner as the
					corresponding method in the \Ref{ModularWidget} class. See \Ref{ModularWidget}
					for more information concerning preferences tabs.\\
					{\bf Note:} if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are initialized.
					@param  preferences the \Ref{Preferences} dialog for {\em *this} mainControl
					@see    show
					@see    MainControlPreferences
					@see    Preferences
			*/
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method removes the \Ref{MainControlPreferences} tab from the 
					\Ref{Preferences} dialog of {\em *this} mainControl.
					This method is called automatically by the \Ref{aboutToExit}
					method at the end of the application. This method is used in the same manner as the
					corresponding method in the \Ref{ModularWidget} class. See \Ref{ModularWidget}
					for more information concerning preferences tabs.\\
					{\bf Note:} if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are finalized.
					@param  preferences the \Ref{Preferences} dialog for {\em *this} mainControl
					@see    aboutToExit
					@see    MainControlPreferences
					@see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();
			
			/** Apply the preferences of the specific tab.
					This method applies the preferences of the own tab \Ref{MainControlPreferences}
					to {\em *this} mainControl.
					This method is called automatically by the \Ref{applyPreferencesTab}
					This method is used in the same manner as the
					corresponding method in the \Ref{ModularWidget} class. See \Ref{ModularWidget}
					for more information concerning preferences tabs.\\
					{\bf Note:} if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are applied.
					@param  preferences the \Ref{Preferences} dialog for {\em *this} mainControl
					@see    applyPreferencesTab
					@see    MainControlPreferences
					@see    Preferences
			*/
			virtual void applyPreferences(Preferences &preferences)
				throw();
			
			/** Fetch the widgets preferences from the inifile.
					This method fetches the general preferences of {\em *this} mainControl and
					the preferences of \Ref{MainControlPreferences} from the {\em inifile}.\\
					{\bf Note:} if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are fetched.
					@param  inifile the \Ref{INIFile} that contains the needed values
					@see    INIFile
					@see    MainControlPreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the widgets preferences to the inifile.
					This method writes the general preferences of {\em *this} mainControl and
					the preferences of \Ref{MainControlPreferences} to the {\em inifile}.\\
					{\bf Note:} if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are written.
					@param  inifile the \Ref{INIFile} that contains the needed values
					@see    INIFile
					@see    MainControlPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
			
			/** Add new modular widget.
					Add a new \Ref{ModularWidget} to {\em *this} mainControl.
					This method will be called internally by the modular widget registration
					process.
					@param  widget the \Ref{ModularWidget} to be inserted into {\em *this} mainControl
					@see    ModularWidget
			*/
			void addModularWidget(ModularWidget* widget)
				throw();

			/** Remove modular widget.
					Remove a new \Ref{ModularWidget} from {\em *this} mainControl.
					This method will be called internally by the modular widget registration
					process.
					@param  widget the \Ref{ModularWidget} to be removed from {\em *this} mainControl
					@see    ModularWidget
			*/
			void removeModularWidget(ModularWidget* widget)
				throw();
			//@}
			
			/**	@name	Debugging and Diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} mainControl.
					If the internal state of {\em *this} mainControl is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise.
					Calls \Ref{CompositeDescriptor::isValid}
					@return			bool {\tt true} if the internal state of {\em *this} mainControl is correct (self-validated) and consistent, {\tt false} otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal state dump.
					Dump the current internal state of {\em *this} mainControl to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{CompositeDescriptor::dump}
					@param   s output stream where to output the internal state of {\em *this} mainControl
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{
			/** Persistent stream output and state restorage.
  			  Read persistent mainControl data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} mainControl
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent mainControl data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} mainControl
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}
			
			protected:

			private:

			/**	Create a unique item ID.
			*/
			int getNextID_()
				throw();

			CompositeDescriptor* insert_(const Composite& composite, const String& name, const Vector3& v)
				throw();

			CompositeDescriptor* insert_(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			typedef HashMap<void*, List<CompositeDescriptor*>::Iterator>
				ListIteratorHashMap;

			ListIteratorHashMap composite_map_;
			ListIteratorHashMap descriptor_map_;
			List<CompositeDescriptor*> descriptors_;

			MainControlPreferences *main_control_preferences_;
			Preferences *preferences_dialog_;
			int preferences_id_;
			INIFile	preferences_;
			
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
