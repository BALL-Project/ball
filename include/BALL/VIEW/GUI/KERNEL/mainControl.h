// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.h,v 1.28 2003/02/21 16:07:30 anhi Exp $

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

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#include <qobject.h>
#include <qwidget.h>
#include <qmainwindow.h>
#include <qlabel.h>

namespace BALL
{
	namespace VIEW
	{
		class ModularWidget;

		/**	MainControl class.
				<b>Caveat:</b> Due to a peculiarity of the QT Meta Object Compiler (MOC)
				you have to specify the full namsespace qualified name of this classes when deriving from it.
				So don't use <tt>class foo : public MainControl {</tt>;} but 
				<tt>class foo : public BALL::VIEW::MainControl {</tt>;} instead.  \par
  \par
				The class MainControl is a container for the  \link CompositeDescriptor CompositeDescriptor \endlink  and the
				 \link ModularWidget ModularWidget \endlink . It is therefore a storage facility for both the graphical
				representation of the inserted composites and the inserted ModularWidgets.
				It hides the  \link CompositeDescriptor CompositeDescriptor \endlink  mechanism so that  \link Composite Composite \endlink  objects
				can be used instead. There are interface methods to access, update and change
				 \link Composite Composite \endlink  objects. There are also a number of methods to directly access
				 \link CompositeDescriptor CompositeDescriptor \endlink  objects. These methods are only internally used and
				should only be used with care.  \par
				This class is also the root  \link ConnectionObject ConnectionObject \endlink . Therefore all messages will
				be handled from this class.  \par
				The class MainControl is the main adminstration unit for a program and must be
				used by all	applications.
				It is also derived from the QT::QMainWindow and therefore the main
				widget of an application must be derived from this class. Further it has the
				necessary interface methods to create and update the menus of the main application.
				It handles also the general preferences tab  \link Preferences Preferences \endlink  of the main
				application and notifies all
				registered  \link ModularWidget ModularWidget \endlink  objects if the preferences has changed.
				<b>Definition:</b>BALL/VIEW/GUI/KERNEL/mainControl.h
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
					 \link Preferences Preferences \endlink  failed.
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
					Read the the  \link INIFile INIFile \endlink  {\em inifile} and connects the qt signal 
					{\em aboutToQuit} with the slot  \link aboutToExit aboutToExit \endlink .
					Initializes the internal data structures.
					The state of {\em *this} mainControl is:
					\begin{itemize}
					  \item  no  \link Composite Composite \endlink  objects stored
						\item  no general  \link Preferences Preferences \endlink  dialog added
						\item  no  \link MainControlPreferences MainControlPreferences \endlink  dialog added
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
					Calls  \link clear clear \endlink 
			*/
			virtual ~MainControl()
				throw();
					
			// copy ctor needed for Python support only!
			MainControl(const MainControl& main_control)
				throw();

			/** Explicit default initialization.
					Clears all inserted  \link CompositeDescriptor CompositeDescriptor \endlink 
			*/
			virtual void clear()
				throw();

			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Insert a new composite (with deletion). 
					Insert a new  \link Composite Composite \endlink  object to {\em *this} mainControl.
					Every  \link Composite Composite \endlink  object can be given a name for later
					identification and a geometric center used for drawing the graphical
					representation of {\em composite}. These additional values will be stored
					in an associated  \link CompositeDescriptor CompositeDescriptor \endlink  object.
					If this method is used to insert composites the composite objects will
					be deleted by the destruction of {\em *this} mainControl. If {\em composite}
					is already inserted it will be removed and reinserted. \par
					<b>Note:</b> do not insert statically created  \link Composite Composite \endlink  objects with this method.
					@param composite the new  \link Composite Composite \endlink  object to be inserted into {\em *this} mainControl
					@param name the name the  \link Composite Composite \endlink  object should have (Default: <tt>"unkown"</tt>)
					@param center the geometric center the  \link Composite Composite \endlink  object should have (Default: <tt>Vector3(0,0,0)</tt>) 
					@see    Composite
					@see    CompositeDescriptor
					@see    Vector3
			*/
			void insert(Composite* composite, const String& name = "unkown", const Vector3& center = Vector3(0,0,0))
				throw();

			/** Insert a new composite (without deletion).
					Insert a new  \link Composite Composite \endlink  object to {\em *this} mainControl.
					If this method is used to insert composites. The composite objects will
					not be deleted by the destruction of {\em *this} mainControl.
					This method returns the  \link CompositeDescriptor CompositeDescriptor \endlink  object that is
					created for the given  \link Composite Composite \endlink  object. See  \link CompositeDescriptor CompositeDescriptor \endlink  for further information.
					If {\em composite} is already inserted <tt>0</tt> will be returned. \par
					<b>Note:</b> statically created  \link Composite Composite \endlink  objects can be inserted with this method.
					@param composite the new  \link Composite Composite \endlink  object to be inserted into {\em *this} mainControl
					@param name the name the  \link Composite Composite \endlink  object should have (Default: <tt>"unkown"</tt>)
					@param center the geometric center the  \link Composite Composite \endlink  object should have (Default: <tt>Vector3(0,0,0)</tt>) 
					@return CompositeDescriptor* pointer to the new compositeDescriptor
					@see    insert
					@see    Composite
					@see    CompositeDescriptor
					@see    Vector3
			*/
      CompositeDescriptor* insert(const Composite& composite, const String& name = "unkown", const Vector3& center = Vector3(0,0,0))
				throw();
 
			/** Insert a new composite_descriptor.
					The  \link CompositeDescriptor CompositeDescriptor \endlink  {\em composite_descriptor} is copied according to the flag {\em deep}.
					If {\em deep} is set to <tt>true</tt> then {\em composite_descriptor} is
					copied deep otherwise a shallow  \link CompositeDescriptor CompositeDescriptor \endlink  is created.
					See  \link CompositeDescriptor CompositeDescriptor \endlink  for information concerning deep and shallow copies.
					If {\em composite_descriptor} is already inserted and a shallow copy should be made <tt>0</tt> will be returned.
					@param composite_descriptor the new  \link CompositeDescriptor CompositeDescriptor \endlink  to be inserted into {\em *this} mainControl
					@param deep if <tt>true</tt> then a deep (default) copy of {\em composite_descriptor} is made, 
										<tt>false</tt> a shallow copy is made
					@return CompositeDescriptor* mutable pointer to the new created  \link CompositeDescriptor CompositeDescriptor \endlink 
					@see    Composite
					@see    CompositeDescriptor
			*/
			CompositeDescriptor* insert(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			/** Remove a composite.
					Remove the  \link Composite Composite \endlink  {\em composite} from {\em *this} mainControl.
					A notification message  \link RemoveCompositeMessage RemoveCompositeMessage \endlink  can be sent to the
					children of {\em *this} mainControl. See  \link ConnectionObject ConnectionObject \endlink  for
					information concerning message queuing and responding.
					@param  composite the  \link Composite Composite \endlink  to be removed from {\em *this} mainControl
					@param  sent_message if set to <tt>true</tt> {\em *this} mainControl sents a  \link RemoveCompositeMessage RemoveCompositeMessage \endlink  
										through the  \link ConnectionObject ConnectionObject \endlink  tree
					@return bool <tt>true</tt> the  \link Composite Composite \endlink  was successfully removed, <tt>false</tt> otherwise
					@see    insert
					@see    Composite
					@see    RemoveCompositeMessage
					@see    ConnectionObject
			*/
			bool remove(const Composite& composite, bool sent_message = true)
				throw();

			/** Remove a compositeDescriptor.
					Remove the  \link CompositeDescriptor CompositeDescriptor \endlink  {\em composite_descriptor} from 
					{\em *this} mainControl. No notification message is sent in this method.			
					@param  composite_descriptor the  \link CompositeDescriptor CompositeDescriptor \endlink  to be removed from {\em *this} mainControl
					@return bool <tt>true</tt> if {\em composite_descriptor} was successfully removed, <tt>false</tt> otherwise
					@see    insert
					@see    remove
					@see    CompositeDescriptor
			*/
			bool remove(const CompositeDescriptor& composite_descriptor)
				throw();

			/** Mutable inspection of a compositeDescriptor.
					Access a mutable pointer to the  \link CompositeDescriptor CompositeDescriptor \endlink  of the inserted
					 \link Composite Composite \endlink  {\em composite}. If {\em composite} is not inserted into
					{\em *this} mainControl then <tt>0</tt> is returned.
					@param   composite the inserted  \link Composite Composite \endlink  whose  \link CompositeDescriptor CompositeDescriptor \endlink  should be returned
					@return  CompositeDescriptor* mutable pointer to  \link CompositeDescriptor CompositeDescriptor \endlink , <tt>0</tt> if {\em composite} 
										is not inserted into {\em *this} mainControl
					@see     insert
					@see     Composite
					@see     CompositeDescriptor
			*/
			CompositeDescriptor* getDescriptor(const Composite& composite)
				throw();

			/** Non-mutable inspection of a compositeDescriptor.
					For further information see  \link getDescriptor getDescriptor \endlink .
			*/
			const CompositeDescriptor* getDescriptor(const Composite& composite) const
				throw();
			
			/** Mutable inspection of of all compositeDescriptor objects.
					Access a mutable reference to the list of  \link CompositeDescriptor CompositeDescriptor \endlink  objects
					of {\em *this} mainControl. Empty if {\em *this} mainControl has no objects yet.			
					@return List<CompositeDescriptor*>& mutable reference to the list of  \link CompositeDescriptor CompositeDescriptor \endlink  objects 
										of {\em *this} mainControl
					@see     insert
					@see     getDescriptor
					@see     CompositeDescriptor
			*/
			List<CompositeDescriptor*>& getDescriptorList()
				throw();

			/** Non-mutable inspection of of all compositeDescriptor objects.
					For further information see  \link getDescriptorList getDescriptorList \endlink .
			*/
			const List<CompositeDescriptor*>& getDescriptorList() const
				throw();

			/** Change the name of a composite.
					Change the name of {\em composite} to {\em name}.					
					@param  composite the  \link Composite Composite \endlink  whose name will be changed
					@param  name the new name
					@return bool <tt>true</tt> if {\em composite} is inserted in {\em *this} mainControl and the assignment was successful
					@see    getName
					@see    insert
					@see    Composite
			*/
			bool setName(const Composite& composite, const String& name)
				throw();

			/** Mutable inspection of the name.
					Access a mutable pointer to the name of {\em composite}. 
					If the {\em composite} is not inserted into {\em *this} mainControl then <tt>0</tt> is returned.
					@param   composite the  \link Composite Composite \endlink  whose name should be retrieved
					@return  String* mutable pointer to the name (<tt>0</tt> if no such composite is inserted)
					@see     setName
					@see     Composite
			*/
			String* getName(const Composite& composite)
				throw();

			/** Non-mutable inspection of the name.
					For further information see  \link getName getName \endlink .
			*/
			const String* getName(const Composite& composite) const
				throw();

			/** Mutable inspection of a compositeDescriptor to a given name.
					Access a mutable pointer to the  \link CompositeDescriptor CompositeDescriptor \endlink  of a
					 \link Composite Composite \endlink  inserted into {\em *this} mainControl with a given string {\em name}.
					If more such names are available then the first found 
					 \link CompositeDescriptor CompositeDescriptor \endlink  with this {\em name} will be returned.
					@param   name the name whose  \link CompositeDescriptor CompositeDescriptor \endlink  should be retrieved
					@return  CompositeDescriptor* mutable pointer to the  \link CompositeDescriptor CompositeDescriptor \endlink  
										(<tt>0</tt> if no such  \link Composite Composite \endlink  with such a name exists)
					@see     CompositeDescriptor
			*/
			CompositeDescriptor* getDescriptor(const String& name)
				throw();

			/** Non-mutable inspection of a compositeDescriptor to a given name.
					For further information see  \link getDescriptor getDescriptor \endlink .
			*/
			const CompositeDescriptor* getDescriptor(const String& name) const
				throw();

			/** Change of the center vector.
					Change the center vector of the  \link Composite Composite \endlink  {\em composite} to the  \link Vector3 Vector3 \endlink  {\em v}.
					@param  composite the  \link Composite Composite \endlink  whose center vector should be changed
					@param  v the  \link Vector3 Vector3 \endlink  that is the new center vector
					@return bool <tt>true</tt> if {\em composite} is inserted in {\em *this} mainControl and the assignment was successful
					@see    getCenter
					@see    Vector3
			*/
			bool setCenter(const Composite& composite, const Vector3& v)
				throw();

			/** Mutable inspection of the center vector.
					Access a mutable pointer to the center  \link Vector3 Vector3 \endlink  of the given
					 \link Composite Composite \endlink  {\em composite}.
					@param   composite the  \link Composite Composite \endlink  whose center vector should be retrieved
					@return  Vector3* mutable pointer to the center vector (<tt>0</tt> if no such  \link Composite Composite \endlink  exists)
					@see     Vector3
			*/
			Vector3* getCenter(const Composite& composite)
				throw();

			/** Non-mutable inspection of the center vector.
					For further information see  \link getCenter getCenter \endlink .
			*/
			const Vector3* getCenter(const Composite& composite) const
				throw();

			/** Mutable inspection of a compositeDescriptor to a given center vector.
					Access a mutable pointer to the  \link CompositeDescriptor CompositeDescriptor \endlink  of a
					 \link Composite Composite \endlink  inserted into {\em *this} mainControl with a given  \link Vector3 Vector3 \endlink  {\em center}.
					If more such center  \link Vector3 Vector3 \endlink  are available then the first found 
					 \link CompositeDescriptor CompositeDescriptor \endlink  with this {\em center} vector will be returned.
					@param   center the center  \link Vector3 Vector3 \endlink  whose  \link CompositeDescriptor CompositeDescriptor \endlink  should be retrieved
					@return  CompositeDescriptor* mutable pointer to the  \link CompositeDescriptor CompositeDescriptor \endlink  
										(<tt>0</tt> if no such  \link Composite Composite \endlink  with such a center vector exists)
					@see     CompositeDescriptor
					@see     Vector3
			*/
			CompositeDescriptor* getDescriptor(const Vector3& center)
				throw();

			/** Non-mutable inspection of a compositeDescriptor to a given center vector.
					For further information see  \link getDescriptor getDescriptor \endlink .
			*/
			const CompositeDescriptor* getDescriptor(const Vector3& center) const
				throw();

			/** Mark a composite for update.
					If a  \link Composite Composite \endlink  was marked for update then the graphical representation
					of {\em composite} will be created anew when the next time the render engine will draw this  \link Composite Composite \endlink .
					If {\em composite} is inserted into {\em *this} mainControl <tt>true</tt> will be
					returned, otherwise <tt>false</tt>.
					@param  composite the  \link Composite Composite \endlink  that should be updated
					@return bool <tt>true</tt> if {\em composite} was successfully marked for update, <tt>false</tt> otherwise
			*/
			bool update(const Composite& composite)
				throw();

			/** Marks all inserted composite objects for update.
					Same as  \link update update \endlink  but all inserted  \link Composite Composite \endlink  objects are marked for update.
					@see update
			*/
			void updateAll()
				throw();

			/** Mutable inspection of the preferences dialog.
					Access the mutual pointer of the general  \link Preferences Preferences \endlink  dialog of {\em *this} mainControl.
					@return   Preferences* a pointer to the  \link Preferences Preferences \endlink  dialog, (<tt>0</tt> if not present)
					@see      Preferences
			*/
			Preferences* getPreferences()
				throw();

			/** Mutable inspection of the inifile.
					Access the mutual reference of the  \link INIFile INIFile \endlink  of {\em *this} mainControl.
					@return      INIFile& mutable reference to the inifile of {\em *this} mainControl
					@see         INIFile
			*/
			INIFile& getINIFile()
				throw();

			/** Non-mutable inspection of the inifile.
					For further information see  \link getINIFile getINIFile \endlink .
			*/
			const INIFile& getINIFile() const
				throw();

			/** Message handling method.
					Handles messages sent by other registered  \link ModularWidget ModularWidget \endlink  objects.
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
					Determines if the given  \link Composite Composite \endlink  {\em composite} is already inserted into
					{\em *this} mainControl.
					@param composite the  \link Composite Composite \endlink  that should be checked
					@return bool <tt>true</tt> if {\em composite} is inserted, <tt>false</tt> otherwise
			*/
			bool isInserted(const Composite& composite) const
				throw();

			/** Test if a given compositeDescriptor is inserted.
					Determines if the given  \link CompositeDescriptor CompositeDescriptor \endlink  {\em compositeDescriptor}
					is already inserted into {\em *this} mainControl.
					@param compositeDescriptor the  \link CompositeDescriptor CompositeDescriptor \endlink  that should be checked
					@return bool <tt>true</tt> if {\em compositeDescriptor} is inserted, <tt>false</tt> otherwise
			*/
			bool isInserted(const CompositeDescriptor& compositeDescriptor) const
				throw();

			public slots:

			//@}
			/** @name Public slots
			*/
			//@{

			/**	Modular widget initialization.
					This method initializes all registered  \link ModularWidget ModularWidget \endlink  objects.
					It initializes the menu structure, the preferences dialogs and connects
					every  \link ModularWidget ModularWidget \endlink  with {\em *this} mainControl.
					This method also creates the first menu entry {\em FILE} with its subentry
					{\em EXIT} to have a means to exit the application.
					See  \link ModularWidget ModularWidget \endlink  for further information concerning menu structure creation
					and preferences handling of  \link ModularWidget ModularWidget \endlink  objects.
					Calls  \link registerConnectionObject registerConnectionObject \endlink 
					Calls  \link initializePreferencesTab initializePreferencesTab \endlink 
					Calls  \link fetchPreferences fetchPreferences \endlink 
					Calls  \link applyPreferences applyPreferences \endlink 
					Calls  \link insertMenuEntry insertMenuEntry \endlink 
					Calls  \link ModularWidget::initializeWidget ModularWidget::initializeWidget \endlink 
					Calls  \link ModularWidget::initializePreferencesTab ModularWidget::initializePreferencesTab \endlink 
					Calls  \link ModularWidget::fetchPreferences ModularWidget::fetchPreferences \endlink 
					Calls  \link ModularWidget::applyPreferences ModularWidget::applyPreferences \endlink 
					Calls  \link Preferences::fetchPreferences Preferences::fetchPreferences \endlink 
					Calls  \link QMainWindow::show QMainWindow::show \endlink  \par
					<b>Note:</b>Call this method to start the application.
					@exception  PreferencesError thrown if the  \link Preferences Preferences \endlink  dialogs cannot be allocated.
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
					associated  \link ModularWidget ModularWidget \endlink  objects registered into {\em *this} mainControl.
					If this method is overridden make sure that it will be called at the end of 
					the new {\em checkMenus} method.
					See  \link ModularWidget ModularWidget \endlink  for further information concerning menu structure creation 
					of  \link ModularWidget ModularWidget \endlink  objects.
					Calls  \link ModularWidget::checkMenu ModularWidget::checkMenu \endlink  \par
					<b>Note:</b> This method will be called internally whenever the menu structure
					needs an update.
				  @see        ModularWidget
				  @see        ModularWidget::checkMenu
			*/
			virtual void checkMenus();

			/** Apply preferences.
					This method calls the method {\em applyPreferences} of all registered
					 \link ModularWidget ModularWidget \endlink  objects if the apply button of the  \link Preferences Preferences \endlink  dialog
					of {\em *this} mainControl is pressed. It processes also the own method {\em applyPreferences}.
					Calls  \link ModularWidget::applyPreferences ModularWidget::applyPreferences \endlink 
					Calls  \link applyPreferences applyPreferences \endlink  \par
					<b>Note:</b> This method will be called internally whenever the apply button
					of the  \link Preferences Preferences \endlink  dialog	is pressed.
				  @see        ModularWidget
				  @see        ModularWidget::applyPreferences
				  @see        applyPreferences
			*/
			virtual void applyPreferencesTab();

			/** Last second cleanup.
					This method stores the preferences and finalizes all  \link ModularWidget ModularWidget \endlink  objects
					and {\em *this} mainControl.
					Must be called after your own cleanup routine if you override this method.
					Calls  \link ModularWidget::writePreferences ModularWidget::writePreferences \endlink 
					Calls  \link ModularWidget::finalizePreferencesTab ModularWidget::finalizePreferencesTab \endlink 
					Calls  \link ModularWidget::finalizeWidget ModularWidget::finalizeWidget \endlink 
					Calls  \link Preferences::writePreferences Preferences::writePreferences \endlink 
					Calls  \link writePreferences writePreferences \endlink 
					Calls  \link finalizePreferencesTab finalizePreferencesTab \endlink 
					Calls  \link removeModularWidget removeModularWidget \endlink 
					Calls  \link INIFile::write INIFile::write \endlink  \par
					<b>Note:</b> This method will be called internally if {\em *this} mainControl will be destroyed.
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

			public:
			
			//@}
			/**	@name	Automatic module registration, menu construction and preferences handling.
			*/
			//@{

			/** Return the mainControl.
					This method returns {\em *this} mainControl that should be the root of the
					 \link ConnectionObject ConnectionObject \endlink  tree from a given widget or dialog  \link QObject QObject \endlink 
					{\em object}. Because we use the qt library every widget or dialog has 
					 \link QObject QObject \endlink  as a base class. MainControl is the main application and therefore
					all widgets and dialogs are children of {\em *this} mainControl. We use the qt
					 \link QObject QObject \endlink  tree mechanism to return {\em *this} mainControl from a given  \link QObject QObject \endlink . \par
					<b>Note:</b> this method is used internally from the  \link ModularWidget ModularWidget \endlink  registration process.
					@return   MainControl* {\em *this} mainControl, the root of the  \link ConnectionObject ConnectionObject \endlink  tree
					@see      ConnectionObject
					@see      ModularWidget
			*/
			static MainControl* getMainControl(const QObject* object)
				throw();
			
			/** Remove a menu entry.
					Remove a menu entry from {\em *this} mainControl. \par
					<b>Note:</b> not yet implemented.
					@param ID the menu ID which menu should be removed
					@param name the name of the menu entry
					@param receiver the object to which the menu action is connected
					@param slot the function that will is called by activation of the menu entry
					@param accel the acceleration key
					@param entry_ID the id of the menu entry
					@see   insertMenuEntry
			*/
			void removeMenuEntry (int ID, const String& name, const QObject* receiver = 0, 
														const char* slot = 0, int accel = 0, int entry_ID = -1)
				throw();
 
			/** Insert a new menu entry.
					Insert a new menu entry into {\em *this} mainControl to menu {\em ID} 
					(creates a new menu if {\em ID} not existent).
					See the documentation of the qt library for more information concerning menu creation.
					@param ID the menu ID to which the new menu entry should be inserted
					@param name the name of the new menu entry
					@param receiver the object to which the menu action will be connected
					@param slot the function that will be called by activation of the menu entry
					@param accel the acceleration key
					@param entry_ID the id for the new menu entry (default: -1, will create a new one)
					@return int the new entry_ID
					@see   removeMenuEntry
			*/
			int insertMenuEntry (int ID, const String& name, const QObject* receiver = 0, 
													 const char* slot = 0, int accel = 0, int entry_ID = -1)
				throw();
			
			/**	Initialize a popup menu.
					Initialize a new popup menu {\em ID}. If {\em *this} mainControl has already
					the popup menu {\em ID} that  \link QPopupMenu QPopupMenu \endlink  is returned.
					See the documentation of the qt library for more information concerning the class QPopupMenu.
					@param    ID the ID of the menu entry to be created.
					@return   QPopupMenu* a pointer to the created  \link QPopupMenu QPopupMenu \endlink 
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
					This method creates the preferences tab  \link MainControlPreferences MainControlPreferences \endlink  for
					{\em *this} mainControl and inserts it into the  \link Preferences Preferences \endlink  dialog.
					This method is called automatically by the  \link show show \endlink  method at the
					start of the application. This method is used in the same manner as the
					corresponding method in the  \link ModularWidget ModularWidget \endlink  class. See  \link ModularWidget ModularWidget \endlink 
					for more information concerning preferences tabs. \par
					<b>Note:</b> if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are initialized.
					@param  preferences the  \link Preferences Preferences \endlink  dialog for {\em *this} mainControl
					@see    show
					@see    MainControlPreferences
					@see    Preferences
			*/
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method removes the  \link MainControlPreferences MainControlPreferences \endlink  tab from the 
					 \link Preferences Preferences \endlink  dialog of {\em *this} mainControl.
					This method is called automatically by the  \link aboutToExit aboutToExit \endlink 
					method at the end of the application. This method is used in the same manner as the
					corresponding method in the  \link ModularWidget ModularWidget \endlink  class. See  \link ModularWidget ModularWidget \endlink 
					for more information concerning preferences tabs. \par
					<b>Note:</b> if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are finalized.
					@param  preferences the  \link Preferences Preferences \endlink  dialog for {\em *this} mainControl
					@see    aboutToExit
					@see    MainControlPreferences
					@see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();
			
			/** Apply the preferences of the specific tab.
					This method applies the preferences of the own tab  \link MainControlPreferences MainControlPreferences \endlink 
					to {\em *this} mainControl.
					This method is called automatically by the  \link applyPreferencesTab applyPreferencesTab \endlink 
					This method is used in the same manner as the
					corresponding method in the  \link ModularWidget ModularWidget \endlink  class. See  \link ModularWidget ModularWidget \endlink 
					for more information concerning preferences tabs. \par
					<b>Note:</b> if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are applied.
					@param  preferences the  \link Preferences Preferences \endlink  dialog for {\em *this} mainControl
					@see    applyPreferencesTab
					@see    MainControlPreferences
					@see    Preferences
			*/
			virtual void applyPreferences(Preferences &preferences)
				throw();
			
			/** Fetch the widgets preferences from the inifile.
					This method fetches the general preferences of {\em *this} mainControl and
					the preferences of  \link MainControlPreferences MainControlPreferences \endlink  from the {\em inifile}. \par
					<b>Note:</b> if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are fetched.
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed values
					@see    INIFile
					@see    MainControlPreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the widgets preferences to the inifile.
					This method writes the general preferences of {\em *this} mainControl and
					the preferences of  \link MainControlPreferences MainControlPreferences \endlink  to the {\em inifile}. \par
					<b>Note:</b> if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are written.
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed values
					@see    INIFile
					@see    MainControlPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
			
			/** Add new modular widget.
					Add a new  \link ModularWidget ModularWidget \endlink  to {\em *this} mainControl.
					This method will be called internally by the modular widget registration process.
					@param  widget the  \link ModularWidget ModularWidget \endlink  to be inserted into {\em *this} mainControl
					@see    ModularWidget
			*/
			void addModularWidget(ModularWidget* widget)
				throw();

			/** Remove modular widget.
					Remove a new  \link ModularWidget ModularWidget \endlink  from {\em *this} mainControl.
					This method will be called internally by the modular widget registration process.
					@param  widget the  \link ModularWidget ModularWidget \endlink  to be removed from {\em *this} mainControl
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
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise.
					Calls  \link CompositeDescriptor::isValid CompositeDescriptor::isValid \endlink 
					@return			bool <tt>true</tt> if the internal state of {\em *this} mainControl is correct
			*/
			virtual bool isValid() const
				throw();

			/** Internal state dump.
					Dump the current internal state of {\em *this} mainControl to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link CompositeDescriptor::dump CompositeDescriptor::dump \endlink 
					@param   s output stream where to output the internal state of {\em *this} mainControl
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
							
			//@}
			/**	@name	Accessors and Settings
			*/
			//@{
			
			/** Get the HashSet with the selected composites
			 */
			const HashSet<Composite*>& getSelection() const
				throw();

			/** Get the selection of the control (not the selection with checkboxes).
			 */
			List<Composite*>& getControlSelection()
				throw();

			/** Return true if exact one system is selected in the control and nothing else
			 */
			System* getSelectedSystem()
				throw();

			/** Select the composite parents of the geometric objects.
			 		The GeometricObjectSelectionMessage is sent by the Scene.
			 */
			void selectComposites_(GeometricObjectSelectionMessage& message)
				throw();

			/**	Select a composite recursive and add all atoms and atomcontainers to the selection.
			 */
			void selectCompositeRecursive(Composite* composite, bool state)
				throw();

			/** Print some informations for the selection.
			 		Called by selectComposites_.
					If one atom is selected, its position is printed.
					If two atoms are selected, their distance,
					for three atoms their angle and
					for four atoms their torsion angle.
					Else the number of items is printed.
			*/
			void printSelectionInfos()
				throw();
			
			/** Sets the text in the statusbar.
			 		The statusbar has a label, whose text is set to the given argument
			*/
			void setStatusbarText(const String& text)
				throw();

			//@}
			
			protected:

			/*_ List with the selected composites
			*/
			HashSet<Composite*> 				selection_;

			/*_ List with the selected composites of the control.
			 		(Not the one with the checkboxes!)
			*/
			List<Composite*>						control_selection_;		

			/*_ Message label in the statusbar
			 		@see setStatusbarText
			*/
			QLabel* 										message_label_;
	
			private:

			/*_	Create a unique item ID.
			*/
			int getNextID_()
				throw();

			CompositeDescriptor* insert_(const Composite& composite, const String& name, const Vector3& v)
				throw();

			CompositeDescriptor* insert_(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			void selectRecursive_(Composite* composite)
				throw();

			void deselectRecursive_(Composite* composite)
				throw();

			typedef HashMap<void*, List<CompositeDescriptor*>::Iterator> ListIteratorHashMap;

			ListIteratorHashMap 				composite_map_;
			ListIteratorHashMap 				descriptor_map_;
			List<CompositeDescriptor*> 	descriptors_;

			MainControlPreferences* 		main_control_preferences_;
			Preferences*								preferences_dialog_;
			int 			 									preferences_id_;
			INIFile		 									preferences_;
			
			static int 									current_id_;

		
			// used to acces the global instance of MainControl
			static MainControl* 				theMainControl_;

			/*_	A list containing all modular widgets.
					This list is modified by \Ref{addModularWidget} and
					\Ref{removeModularWidget}.
			*/
			List<ModularWidget*>				modular_widgets_;
		};

#ifndef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/VIEW/GUI/KERNEL/mainControl.iC>
#endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
