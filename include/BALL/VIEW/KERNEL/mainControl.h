// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.h,v 1.18 2003/11/18 14:45:39 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
#define BALL_VIEW_KERNEL_MAINCONTROL_H

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H
#	include <BALL/VIEW/KERNEL/primitiveManager.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMPOSITEMANAGER_H
#	include <BALL/VIEW/KERNEL/compositeManager.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
 #include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_VIEW_KERNEL_QTTIMER_H
 #include <BALL/VIEW/KERNEL/QTTimer.h>
#endif

#include <qmainwindow.h>

class QLabel;

namespace BALL
{
	class Composite;
	class System;

	namespace VIEW
	{
		class ModularWidget;
		class Preferences;
		class MainControlPreferences;
		class GeometricObjectSelectionMessage;

		/** Timer class to clear the statusbar of the MainControl after a given time
		*/
		class StatusbarTimer
			: public QTTimer
		{
			public:

				///
				StatusbarTimer(QObject* parent=0)
					throw();
				
				/// Set the label of the statusbar (e.g. of the MainControl) which is cleared by the timer.
				void setLabel(QLabel* label)
					throw();

			protected:
				virtual void timer()
					throw();

			private:
				QLabel* label_;
		};

		/**	MainControl is the main adminstration unit for a program and must be
				used by all	applications.
				It is a storage facility for both Composite objects, the graphical
				Representation and the inserted ModularWidget.
				This class is also the root ConnectionObject. 
				Therefore all messages will be handled from this class. \par
				It is also derived from the QT::QMainWindow and therefore the main
				widget of an application must be derived from this class. Further it has the
				necessary interface methods to create and update the menus of the main application.
				\par
				It handles also the general preferences tab Preferences of the main application and notifies all
				registered ModularWidget objects if the preferences have changed. \par
				The preferences of the application are stored in an INIFile.
				The default name of this file is ".molview".
				<b>Caveat:</b> Due to a peculiarity of the QT Meta Object Compiler (MOC)
				you have to specify the full namsespace qualified name of this classes when deriving from it. \par
				So don't use\par 
				<tt> class foo : public MainControl </tt>; but \par
				<tt> class foo : public BALL::VIEW::MainControl </tt> instead. 
		\ingroup ViewKernelConnectivity
		*/
		class MainControl
			: public QMainWindow,
				public ConnectionObject,
				public Embeddable
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(MainControl)

			/**	@name Enumerations
			*/
			//@{
			/**	Standard Popup menu IDs.
					This enum defines symbolic names for the menu IDs of the most common popup menus.
					The popups are created, if a menu entry is requested for any of the popups.
					\see insertMenuEntry
			*/
			enum PopUpID
			{
				/// File menu
				FILE        = 10001,

				/// File menu sub menu open
				FILE_OPEN,

				/// File menu sub menu import
				FILE_IMPORT,

				/// File menu sub menu export
				FILE_EXPORT,

				/// Edit menu
				EDIT = 10100,

				/// Build menu 
				BUILD,

				/// Display menu 
				DISPLAY,

				/// Display Viewpoint submenu
				DISPLAY_VIEWPOINT,

				/// Tools menu
				TOOLS,

				/// Create grid submenu in Tools
				TOOLS_CREATE_GRID,

				/// Python submenu in Tools
				TOOLS_PYTHON,

				/// Windows menu
				WINDOWS,

				/// Help menu
				HELP,

				/// Userdefined menus
				USER
			};
			//@}
			/**	@name	Constructors and Destructor
			*/
			//@{

			/** Default Constructor.
					Reads the the INIFile <tt>inifile</tt> and connects the qt signal 
					<b> aboutToQuit </b>with the slot aboutToExit().
					The state of the MainControl is:
						-  no Composite objects stored
						-  no general Preferences dialog added
						-  no MainControlPreferences dialog added
					\par
					\param  parent the new parent widget
					\param  name the new name of the widget
					\param  inifile the new preferences filename
					\see    INIFile
					\see    Preferences
			*/
			MainControl(QWidget* parent = 0, const char* name = 0 , String inifile = ".BALL.preferences")
				throw();

			/** Destructor.
					Calls clear
			*/
			virtual ~MainControl()
				throw();
					
			// copy ctor needed for Python support only!
			MainControl(const MainControl& main_control)
				throw();

			/** Explicit default initialization.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Accessors
			*/
			//@{

			/** Get the primitive manager.
					The class PrimitiveManager contains all Representation objects and GeometricObject.
			*/
			PrimitiveManager& getPrimitiveManager()
				throw() { return primitive_manager_;}

			/** Get the composite manager.
					The class CompositeManager is the owner of all Composite objects.
			*/
			CompositeManager& getCompositeManager()
				throw() { return composite_manager_;}
			
			/** Redraws all Representation objects for a Composite.
					If the Composite is not inserted into this MainControl <tt>false</b> will be returned.
					updateRepresentationsOf() is called after receiving a CompositeChangedMessage in onNotify().
					It sends a RepresentationChangedMessage for every Representation, which was build for the Composite.
					After this a SceneMessage is send to redraw the Scene.
					Remember:
					If you changed a composite in MainControl or a derived class, the MainControl doesnt get notified,
					from the CompositeChangedMessage, it sends. So you have to call this function instead of sending the message.
					\param  composite the Composite that should be updated
					\param  rebuild if set to true, the model is rebuilded, otherwise just the coloring is updated
					\return false if the CompositeManager doesnt contain the Composite
			*/
			bool updateRepresentationsOf(const Composite& composite, bool rebuild = true)
				throw();

			/** Redraws all inserted objects.
					Same as update but all Representation objects are redrawn.
					\param rebuild_display_lists set to true lets the Scene rebuild the GLDisplayList objects.
					\see update
			*/
			void updateAllRepresentations(bool rebuild_display_lists = false)
				throw();

			/** Update a Composite in all ModularWidget.
			 		A CompositeMessage with type CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL is send and
					updateRepresentationsOf(composite) is called.
					\return false if the CompositeManager doesnt contain the Composite
			*/
			bool update(Composite& composite)
				throw();

			/** Insert a Composite and notify all ModularWidget.
			 		The Composite has to be created on the heap!!!
			 		A CompositeMessage with type NEW_COMPOSITE is send and
					CompositeManager::insert called.
					\return false if the CompositeManager contains the Composite
			*/
			bool insert(Composite& composite)
				throw();

			/** Remove a Composite and notify all ModularWidget.
			 		A CompositeMessage with type REMOVED_COMPOSITE is send and
					CompositeManager::remove called.
					\return false if the CompositeManager doesnt contain the Composite
			*/
			bool remove(Composite& composite)
				throw();

			/** Update a Representation
			 		A RepresentationMessage with type UPDATE is send.
					\return false if the PrimitiveManager doesnt contain the Representation
			*/
			bool update(Representation& rep)
				throw();

			/** Insert a Representation
			 		The Representation must be created on the heap!!!
			 		A RepresentationMessage with type NEW is send.
					\return false if the PrimitiveManager contains the Representation
			*/
			bool insert(Representation& rep)
				throw();

			/** Remove a Representation
			 		A RepresentationMessage with type REMOVE is send.
					\return false if the PrimitiveManager doesnt contain the Representation
			*/
			bool remove(Representation& rep)
				throw();

				
			/** Mutable inspection of the preferences dialog.
					\return   Preferences* a pointer to the Preferences dialog, (<tt> 0</tt> if not present)
			*/
			Preferences* getPreferences()
				throw();

			/** Mutable inspection of the INIFile.
			*/
			INIFile& getINIFile()
				throw();

			/** Non-mutable inspection of the INIFile.
			*/
			const INIFile& getINIFile() const
				throw();

			/** Message handling method.
					Handles messages sent by other registered ModularWidget objects.
					Virtual function for overriden the message handling system. 
					Take care to call this function in your own ModularWidget.
					There is no need to call this function, because it will be called from the 
					message handling mechanism.
					\param message the pointer to the message that should be processed
					\see   ModularWidget
					\see   Message
			*/
			virtual void onNotify(Message *message)
				throw();

			public slots:

			//@}
			/** @name Public slots
			*/
			//@{

			/**	Initialize all registered ModularWidget objects.
					It initializes the menu structure, the preferences dialogs and connects
					every ModularWidget with the MainControl.
					This method also creates the first menu entry <b>FILE</b> with its subentry
					<b>EXIT</b> to exit the application.
					See ModularWidget for further information concerning menu structure creation
					and preferences handling. \par
					Calls registerConnectionObject() \par
					Calls initializePreferencesTab() \par
					Calls fetchPreferences() \par
					Calls applyPreferences() \par
					Calls insertMenuEntry() \par
					Calls ModularWidget::initializeWidget() \par
					Calls ModularWidget::initializePreferencesTab() \par
					Calls ModularWidget::fetchPreferences() \par
					Calls ModularWidget::applyPreferences() \par
					Calls Preferences::fetchPreferences() \par
					Calls QMainWindow::show() \par
					<b>Note: <\b>Call this method to start the application.
					\exception  GeneralException thrown if the Preferences dialogs cannot be allocated.
			*/
			virtual void show();

			/** Menu checking method.
					This method checks, enables or disables all inserted menu entries of the
					registered ModularWidget objects.
					If this method is overridden make sure that it will be called at the end of 
					the new <b>checkMenus()</b> method.
					See ModularWidget for further information concerning menu structure creation.\par
					<b>Note:</b> This method will be called internally whenever the menu structure needs an update.
					Calls ModularWidget::checkMenu\par
					\see        ModularWidget::checkMenu
			*/
			virtual void checkMenus();

			/** Apply preferences.
					This method calls the method <b>ModularWidget::applyPreferences</b> of all registered
					ModularWidget objects if the apply button of the Preferences dialog is pressed. 
					<b>Note:</b> This method will be called internally whenever the apply button
					of the Preferences dialog	is pressed.  \par
					Calls ModularWidget::applyPreferences\par
					Calls applyPreferences\par
					\see        ModularWidget::applyPreferences
					\see        applyPreferences()
			*/
			virtual void applyPreferencesTab();

			/** Called after canceling the Preferences dialog
					Resets all in the Preferences dialog inserted tabs.
			*/
			virtual void cancelPreferencesTab()
				throw();

			/** Last second cleanup.
					This method will be called internally if the MainControl is about to be destroyed.
					This method stores the preferences and finalizes all ModularWidget objects
					and the MainControl.
					Must be called after your own cleanup routine if you override this method.\par
					Calls ModularWidget::writePreferences \par
					Calls ModularWidget::finalizePreferencesTab \par
					Calls ModularWidget::finalizeWidget \par
					Calls Preferences::writePreferences \par
					Calls writePreferences \par
					Calls finalizePreferencesTab \par
					Calls removeModularWidget \par
					Calls INIFile::write \par
			*/
			virtual void aboutToExit();

			/** Slot that is called when a menu item is highlighted.
					It is used to show a hint for every menu entry.
					@see setMenuHint
					@see getMenuHint
			*/
			void menuItemHighlighted(int id)
				throw();
			
			public:
			
			//@}
			/**	@name	Automatic module registration, menu construction and preferences handling.
			*/
			//@{

			/** Return the MainControl of an QObject.
					This method returns the MainControl that should be the root of the
					ConnectionObject tree from a given widget or dialog QObject.
					Because we use the qt library, every widget or dialog has 
					QObject as a base class. MainControl is the main application and therefore
					all widgets and dialogs are its children. We use the qt
					QObject tree mechanism to return the MainControl for a given QObject.\par
					<b>Note</b>: This method is used internally from the ModularWidget registration process.
					\return   MainControl* the root of the ConnectionObject tree
					\see      ConnectionObject
					\see      ModularWidget
			*/
			static MainControl* getMainControl(const QObject* object)
				throw();
			
			/** Insert a new menu entry into menu <b>ID</b> 
					(creates a new menu if <b>ID</b> not existent).
					See the documentation of the qt library for more information concerning menu creation.
					\param ID the menu ID to which the new menu entry should be inserted
					\param name the name of the new menu entry
					\param receiver the object to which the menu action will be connected
					\param slot the function that will be called by activation of the menu entry
					\param accel the acceleration key
					\param entry_ID the id for the new menu entry (default: -1, will create a new one)
					\return int the new entry_ID
			*/
			int insertMenuEntry (int ID, const String& name, const QObject* receiver = 0, 
													 const char* slot = 0, int accel = 0, int entry_ID = -1, String hint = "")
				throw();

			/// Not implemented yet.
			void removeMenuEntry (int ID, const String& name, const QObject* receiver = 0, 
												 const char* slot = 0, int accel = 0, int entry_ID = -1)
			throw();
			
			/**	Initialize a new popup menu <b> ID</b>. 
					If the MainControl has already the popup menu <b>ID</b> that QPopupMenu is returned.
					See the documentation of the qt library for more information concerning the class QPopupMenu.
					\param    ID the ID of the menu entry to be created.
					\return   QPopupMenu* a pointer to the created QPopupMenu
					\see      PopUpID
			*/	
			virtual QPopupMenu* initPopupMenu(int ID)
				throw();

			/** Insert a separator into the popup menu <b> ID</b>. 
					If the menu <b>ID</b> is not existent, it will be created first.
					\param ID the id of the menu to which a separator will be inserted
					\see   PopUpID
			*/
			void insertPopupMenuSeparator(int ID)
				throw();

			/** Initialize a preferences tab for the MainControl.
					This method creates the preferences tab MainControlPreferences for
					the MainControl and inserts it into the Preferences dialog.
					It is called automatically by the show method at the
					start of the application. This method is used in the same manner as the
					corresponding method in the ModularWidget class. See ModularWidget
					for more information concerning preferences tabs.\par
					<b>Note:</b> If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are initialized.
					\param  preferences the Preferences dialog for the MainControl
					\see    show
					\see    MainControlPreferences
					\see    Preferences
			*/
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method removes the MainControlPreferences tab from the 
					Preferences dialog of the MainControl.
					This method is called automatically by aboutToExit()
					at the end of the application. It is used in the same manner as the
					corresponding method in the ModularWidget class. See ModularWidget
					for more information concerning preferences tabs.\par
					<b>Note:</b> If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are finalized.
					\param  preferences the Preferences dialog for this MainControl
					\see    MainControlPreferences
					\see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();
			
			/** Apply the preferences of the own tab MainControlPreferences.
					This method is called automatically by applyPreferencesTab().
					It is used in the same manner as the
					corresponding method in the ModularWidget class. See ModularWidget
					for more information concerning preferences tabs.\par
					<b>Note:</b> If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are applied.
					\param  preferences the Preferences dialog for this MainControl
					\see    applyPreferencesTab
					\see    MainControlPreferences
					\see    Preferences
			*/
			virtual void applyPreferences(Preferences &preferences)
				throw();
			
			/** Fetch the widgets preferences from the INIfile.
					This method fetches the general preferences of the MainControl and
					the preferences of MainControlPreferences from <tt>inifile</tt>.\par
					<b>Note:</b>If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are fetched.
					\param  inifile the INIFile that contains the needed values
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the widgets preferences to the INIFile.
					This method writes the general Preferences of this MainControl and
					the preferences of MainControlPreferences to the <tt>inifile</tt>.\par
					<b>Note:</b> if this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are written.
					\param  inifile the INIFile that contains the needed values
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
			
			/// Restore the positions of all DockWindows from the INIFile
			virtual void restoreWindows()
				throw();
			
			/** Add a new ModularWidget to this MainControl.
					This method will be called internally by the ModularWidget registration process.
					\param  widget the ModularWidget to be inserted into this mainControl
			*/
			void addModularWidget(ModularWidget* widget)
				throw();

			/** Remove a new ModularWidget from the MainControl.
					This method will be called internally by the ModularWidget registration process.
					\param  widget the ModularWidget to be removed
			*/
			void removeModularWidget(ModularWidget* widget)
				throw();

			//@}
			/**	@name	Accessors and Settings
			*/
			//@{
			
			/// Get the HashSet with the selected (e.g. picked) Composite objects
			const HashSet<Composite*>& getSelection() const
				throw();

			/// Get the selection of the MolecularControl (not the selection with checkboxes).
			List<Composite*>& getControlSelection()
				throw();

			/// If exactly one System is selected in the Control, return a pointer to this system, otherwise 0.
			System* getSelectedSystem()
				throw();

			///	Select a Composite recursive and add all Atom and Atomcontainer objects to the selection.
			void selectCompositeRecursive(Composite* composite, bool first_call=false)
				throw();

			/// Select a Composite recursive and add all Atom and Atomcontainer objects to the selection.
			void deselectCompositeRecursive(Composite* composite, bool first_call=false)
				throw();

			/** Print some informations for the selection in the statusbar.
					Called by selectComposites_().
					If one Atom is selected, its position is printed.
					If two Atom objects are selected, their distance,
					for three Atom their angle and
					for four Atom their torsion angle.
					Else the number of items is printed.
			*/
			void printSelectionInfos()
				throw();

			/** Sets the text in the statusbar.
					The statusbar has a label, whose text is set to the given argument
			*/
			void setStatusbarText(const String& text)
				throw();

			///
			void setMenuHint(Index id, const String& hint)
				throw();

			///
			const String& getMenuHint(Index id) const
				throw();
			
			///
			const FragmentDB& getFragmentDB() const
				throw() { return fragment_db_;}

			/** Set a properties, which defines, if the stored composites can be changed at the moment.
			 		This is used e.g. to prevent changes in the composite hierarchy while a simulation is running.
					@see compositesAreMuteable
			*/
			void setCompositesMuteable(bool state) {composites_muteable_ = state;}

			/** Check wheter the stored composites can be modified at the moment.
					This method returns false e.g. while a MD simulation is running.
			*/
			bool compositesAreMuteable() {return composites_muteable_;}
					
			//@}
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** Internal state dump.
					Dump the current internal state of this mainControl to 
					the output ostream <b>s</b> with dumping depth <b>depth</b>.
					\param   s output stream where to output the internal state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
					
			//@}
			
			protected:

			/*_ Remove a composite.
					Every Representation, which was created for the Composite is deleted, by sending a 
					RepresentationMessage with type RepresentationMessage::REMOVE.\par
					Calls updateAllRepresentations()
					\return bool <tt>true</tt> if the CompositeManager has the Composite
			*/
			bool remove_(Composite& composite)
				throw();

			/*_	Create a unique item ID for a menuentry by adding 1 to current_id_
			*/
			int getNextID_()
				throw();

			void selectRecursive_(Composite* composite)
				throw();

			void deselectRecursive_(Composite* composite)
				throw();

			/*_ Select the composite parents of the geometric objects.
					The GeometricObjectSelectionMessage is sent by the Scene.
			 */
			void selectComposites_(GeometricObjectSelectionMessage& message)
				throw();

			//_
			FragmentDB fragment_db_;

			/*_ List with the selected composites
			*/
			HashSet<Composite*> 				selection_;

			/*_ List with the selected composites of the control.
					(Not the one with the checkboxes!)
			*/
			List<Composite*>						control_selection_;		

			/*_ Message label in the statusbar
					\see setStatusbarText
			*/
			QLabel* 										message_label_;


			PrimitiveManager 						primitive_manager_;
			CompositeManager 						composite_manager_;

			MainControlPreferences* 		main_control_preferences_;
			Preferences*								preferences_dialog_;
			int 			 									preferences_id_;
			INIFile		 									preferences_;
			
			static int 									current_id_;
			bool 												composites_muteable_;

			// used to acces the global instance of MainControl
			static MainControl* 				theMainControl_;

			/*_	A list containing all modular widgets.
					This list is modified by addModularWidget and
					removeModularWidget.
			*/
			List<ModularWidget*>				modular_widgets_;

			HashMap<Index, String>      menu_entries_hints_;

			StatusbarTimer* 						timer_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/mainControl.iC>
#		endif 
    
		}	// namespace VIEW
	} // namespace BALL

#endif // BALL_VIEW_KERNEL_MAINCONTROL_H
