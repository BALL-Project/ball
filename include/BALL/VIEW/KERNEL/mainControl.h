// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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

#ifndef BALL_VIEW_KERNEL_REPRESENTATIONMANAGER_H
#	include <BALL/VIEW/KERNEL/representationManager.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMPOSITEMANAGER_H
#	include <BALL/VIEW/KERNEL/compositeManager.h>
#endif

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#endif

#ifndef BALL_VIEW_KERNEL_UIOPERATIONMODE_H
#include <BALL/VIEW/KERNEL/UIOperationMode.h>
#endif

#ifndef BALL_VIEW_KERNEL_UIOPERATIONMODE_H
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#include <QtGui/QKeySequence>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>

class QCloseEvent;

namespace BALL
{
	namespace VIEW
	{
		class ModelInformation;
		class ModularWidget;
		class Preferences;
		class MainControlPreferences;
		class OpenSavePreferences;
		class NetworkPreferences;
		class WebEnginePreferences;
		class GeometricObjectSelectionMessage;
		class SimulationThread;

		/**	MainControl is the main administration unit for a program and must be
				used by all	applications.
				MainControl is also derived from Qt::QMainWindow and therefore the main
				widget of an application must be derived from this class. Further it has the
				necessary interface methods to create and update the menus of the main application.
				<br>
				<br>
				MainControl is also a storage facility for Composite objects, the graphical
				Representation and the inserted ModularWidget.
				The interface for the Composite administration is implemented in CompositeManager, and for
				the Representation 's in RepresentationManager.
				Specialized methods exists for the adding, updateing and removing of Composites as well
				as Representations. (see the update, insert and remove methods)
				<br>	
				<br>	
				This class is also the root ConnectionObject and thus
				it is responsible for the handling of all messages.
				To add ModularWidgets all that is necessary are the creation of the derived modular
				widgets with the MainControl as their parents like e.g.<br>
				new DisplayProperties(this, "DisplayProperties");<br>
				For examples have a look at BALL/source/APPLICATIONS/mainframe.C
				<br>
				<br>
				MainControl also handles the Preferences dialog (and it's tab entry for some general 
				preferences) 
				and notifies all registered ModularWidgets if the preferences have changed.
				The content of the Preferences dialog with all setup options is stored in an INIFile.
				The default name of this file is ".BALLView".<br>
				@see writePreferences
				@see fetchPreferences
				@see saveBALLViewProjectFile
				@see loadBALLViewProjectFile
				<br><br>
				<b>Caveat:</b> Due to a peculiarity of the QT Meta Object Compiler (MOC)
				you have to specify the full namespace qualified name of this class when deriving from it. \par
				So don't use<br>
				<tt> class foo : public MainControl </tt>; but <br>
				<tt> class foo : public BALL::VIEW::MainControl </tt> instead. 
		\ingroup ViewKernelConnectivity
		*/
		class BALL_VIEW_EXPORT MainControl
			: public QMainWindow,
				public ConnectionObject,
				public Embeddable
		{
			friend class RepresentationManager;
			friend class SimulationThread;

			Q_OBJECT

			public:

			BALL_EMBEDDABLE(MainControl,Embeddable)

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

				/// File menu sub menu open grid
				FILE_OPEN_GRID,

				/// File menu sub menu import [currently unused]
				FILE_IMPORT,

				/// File menu sub menu export
				FILE_EXPORT,

				/// 
				FILE_MONITOR,

				/// Edit menu
				EDIT = 10100,

				/// Build menu 
				BUILD = 10200,

				/// Display menu 
				DISPLAY = 10300,

				/// Display Create submenu
				DISPLAY_CREATE,
				
				/// Display Viewpoint submenu
				DISPLAY_VIEWPOINT,

				/// Display Stereo submenu
				DISPLAY_STEREO,
				
				/// Display Animation submenu
				DISPLAY_ANIMATION,

#ifdef BALL_HAS_RTFACT	
				/// Display Raytracing Continuos Loop submenu
				DISPLAY_CONTINUOUSLOOP,
#endif				
				/// Simulations menu
				MOLECULARMECHANICS = 10400,
				
				/// Molmec submenu for force field selection
				CHOOSE_FF,

				/// Tools menu
				TOOLS = 10500,

				/// Grid submenu in Tools
				TOOLS_GRID,

				/// Python submenu in Tools
				TOOLS_PYTHON,

				/// Windows menu
				WINDOWS = 10600,

				/// Userdefined menus
				USER = 10700,

				/// Macros e.g. for Testing
				MACRO = 10750,

				/// Help menu
				HELP = 10800
			};

			//@}
			/**	@name	Constructors and Destructor
			*/
			//@{

			/** Default Constructor.
					Reads the the INIFile <tt>inifile</tt> and connects the qt signal 
					<b> aboutToQuit </b> with the slot aboutToExit().
					The state of the MainControl is:
						-  no Composite objects stored
						-  no Representation objects stored
						-  no general Preferences dialog added
						-  no MainControlPreferences dialog added
					\par
					\param  parent the new parent widget
					\param  name the new name of the widget
					\param  inifile the new preferences filename
					\see    INIFile
					\see    Preferences
			*/
			MainControl(QWidget* parent = 0, const char* name = 0 , String inifile = ".BALL.preferences");

			/** Destructor.
					Calls clear
			*/
			virtual ~MainControl();
					
			// copy ctor needed for Python support only!
			MainControl(const MainControl& main_control);

			/** Clear all data fast, to be called at exit only!
			*/
			virtual void clear();

			/** Clear all data, can be called at any time
			*/
			void clearData();

			//@}
			/**	@name	Methods to manage Representation(s)
			*/
			//@{

			/** Get the primitive manager.
					The class RepresentationManager contains all Representation objects and GeometricObject.
			*/
			RepresentationManager& getRepresentationManager()
				{ return primitive_manager_;}

			/** Insert a Representation
			 		The Representation must be created on the heap!!!
			 		A RepresentationMessage with type NEW is send.
					\return false if the RepresentationManager contains the Representation
			*/
			bool insert(Representation& rep);

			/** Remove a Representation
			 		A RepresentationMessage with type REMOVE is send.
					\return false if the RepresentationManager doesnt contain the Representation
			*/
			bool remove(Representation& rep);

			/** Update a Representation
			 		A RepresentationMessage with type UPDATE and a SceneMessage is send.
					\return false if the RepresentationManager doesnt contain the Representation
			*/
			bool update(Representation& rep);

			/** Redraws all Representation objects for a Composite.
					If the Composite is not inserted into this MainControl <tt>false</tt> will be returned.
					updateRepresentationsOf() is called after receiving a CompositeMessage with type CHANGED_COMPOSITE in onNotify().
					It sends a RepresentationMessage with type UPDATE for every Representation, which was build for the 
					Composite.  After this a SceneMessage is send to redraw the Scene.
					\par
					Notes:
					If you changed a Composite in MainControl or a derived class, the MainControl doesnt get 
					notified, from the CompositeMessage it sends. So you have to call this function instead 
					of sending the message.
					\param  composite the Composite that should be updated
					\param  rebuild if set to true, the model is rebuilded, otherwise just the coloring is updated
					\param  force is set to true, also rebuild non surface models (only usefull with rebuild = true)
					\return true if an update was performed
			*/
			bool updateRepresentationsOf(const Composite& composite, bool rebuild = true, bool force = false);

			/** Redraws all inserted Representation, but doesnt change the Models.
					\param rebuild_display_lists set to true lets the Scene rebuild the GLDisplayList objects.
					\see updateRepresentationsOf
			*/
			void redrawAllRepresentations(bool rebuild_display_lists = false);
			
			//@}
			/**	@name	Methods to manage Shortcuts
			*/
			//@{
			/** Get the shortcut registry.
					The class ShortcutRegistry is the owner of all shortcuts.
			*/
			ShortcutRegistry& getShortcutRegistry() { return shortcut_registry_;}

					
			//@}
			//
			/**	@name	Methods to manage Composite(s)
			*/
			//@{

			/** Get the composite manager.
					The class CompositeManager is the owner of all Composite objects.
			*/
			CompositeManager& getCompositeManager()
				{ return composite_manager_;}
	
			/** Insert a Composite and notify all ModularWidget.
			 		The Composite has to be created on the heap!!!
			 		A CompositeMessage with type NEW_COMPOSITE is send and
					CompositeManager::insert called.
					\return false if the CompositeManager contains the Composite
			*/
			bool insert(Composite& composite, String name = "", bool normalize = true);

			/** Remove a Composite and notify all ModularWidget.
			 		A CompositeMessage with type REMOVED_COMPOSITE is send and
					CompositeManager::remove called.
					@param update update Representations if needed
					\return false if the CompositeManager doesnt contain the Composite
			*/
			bool remove(Composite& composite, bool to_delete = true, bool update = true);
	
			/** Update a Composite in all ModularWidget.
			 		This method differs wheter the composites hierarchy was changed or not.
					The update is faster if the hierarchy is unchanged, because e.g. the
					MolecularControl doesnt have to rebuild the ListViewItem tree.
			 		A CompositeMessage with type CHANGED_COMPOSITE or CHANGED_COMPOSITE_HIERARCHY is send and
					updateRepresentationsOf(composite) is called.
					\return false if the CompositeManager doesnt contain the Composite
			*/
			void update(Composite& composite, bool changed_hierarchy = true);
			
			/// Get the HashSet with the selected (e.g. picked) Composite objects (const)
			const HashSet<Composite*>& getSelection() const;

			/// Get the HashSet with the selected (e.g. picked) Composite objects
			HashSet<Composite*>& getSelection() ;

			/// Get the selection (highlighted items) of the MolecularControl (not the selection with checkboxes)
			std::list<Composite*>& getMolecularControlSelection();

			/// If exactly one System is selected in the Control, return a pointer to this system, otherwise 0.
			System* getSelectedSystem();

			///	Select a Composite recursive and add all Atom and AtomContainer objects to the selection.
			void selectCompositeRecursive(Composite* composite, bool first_call=false);

			/// Select a Composite recursive and add all Atom and AtomContainer objects to the selection.
			void deselectCompositeRecursive(Composite* composite, bool first_call=false);

			/** Print some informations for the selection in the statusbar.
					Called by selectComposites_().
					If one Atom is selected, its position is printed.
					If two Atom objects are selected, their distance,
					for three Atom 's their angle and
					for four Atom 's their torsion angle.
					Else the number of items is printed.
			*/
			void printSelectionInfos();


			//@}
			/**	@name	Preferences and Configuration files
			*/
			//@{
			
			/// Save the current configuration, structures and representations to a BALLView project file (*.bvp)
			void saveBALLViewProjectFile(const String& filename, bool binary = true);
			
			/// Load a BALLView project file
			void loadBALLViewProjectFile(const String& filename);

			/** Fetch the preferences from the INIfile.
					Calls fetchPreferences() for all registered ModularWidgets.
					<b>Note:</b>If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are fetched.
					\param  inifile the INIFile that contains the needed values
			*/
			virtual void fetchPreferences(INIFile &inifile);
			
			/** Writes the widgets preferences to the INIFile.
					Calls writePreferences() for all registered ModularWidgets and
					Preferences::savePreferences().
					<b>Note:</b> If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are written.
					\param  inifile the INIFile that contains the needed values
			*/
			virtual void writePreferences(INIFile &inifile);
			
			/// Restore the positions the main window and of all DockWindow's from the INIFile assigned to this instance.
			virtual void restoreWindows();
	
			/// Restore the positions the main window and of all DockWindow's from a given inifile
			virtual void restoreWindows(const INIFile& inifile);
	
			/** Mutable inspection of the INIFile.
			*/
			INIFile& getINIFile();

			/** Non-mutable inspection of the INIFile.
			*/
			const INIFile& getINIFile() const;
			
			/** Mutable inspection of the preferences dialog.
					\return   Preferences* a pointer to the Preferences dialog, (<tt> 0</tt> if not present)
			*/
			Preferences* getPreferences();
			
			/** Apply all preferences.
					This method is called automatically by applyPreferencesClicked() and calls
					applyPreferences() for all registered ModularWidgets.
					<b>Note:</b> If this method is overridden, call this method at the end of the
					overriden method to make sure that the general preferences are applied.
					\see    ModularWidget
					\see    Preferences
			*/
			virtual void applyPreferences();
			
		
			//@}
			/**	@name	Management of ModularWidget(s) and Message(s)
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
			static MainControl* getMainControl(const QObject* object);
			
			/** Add a new ModularWidget to this MainControl.
					This method will be called internally by the ModularWidget registration process.
					So, if you dont know exactly what this method does, you will not need it!
					\param  widget the ModularWidget to be inserted into this mainControl
			*/
			void addModularWidget(ModularWidget* widget);

			/** Remove a ModularWidget from the MainControl.
					This method will be called internally by the ModularWidget registration process.
					So, if you dont know exactly what this method does, you will not need it!
					\param  widget the ModularWidget to be removed
			*/
			void removeModularWidget(ModularWidget* widget);

			/** Message handling method.
					Handles messages sent by other registered ModularWidget objects.
					Virtual function for overriden the message handling system. 
					Take care to call this function in your own ModularWidget.
					There is no need to call this function, because it will be called from the 
					message handling mechanism.
					<b>Remember:</b> A ModularWidget is not notified by the Messages it sends itself!
					\param message the pointer to the message that should be processed
					\see   ModularWidget
					\see   Message
			*/
			virtual void onNotify(Message *message);

			/** Send a Message from Python.
					Otherwise, you should prefer to use ModularWidget::notify_.
					The MainControl itself also reacts to a Message, send with this method.
					The Message will be deleted, after it was send to all ModularWidget's.
			*/
			void sendMessage(Message& message);


			//@}
			/**	@name	Menu entries handling
			*/
			//@{
		
			/** Insert a new menu entry into menu <b>ID</b> 
			 *	(creates a new menu if <b>ID</b> not existent).
			 *	See the documentation of the qt library for more information concerning menu creation.
			 *	@param ID the menu ID to which the new menu entry should be inserted
			 *	@param name the name of the new menu entry
			 *	@param receiver the object to which the menu action will be connected
			 *	@param slot the function that will be called by activation of the menu entry
			 *	@param description a unique descriptive string for the action
			 *	@param accel the acceleration key
			 *  @param minimal_mode the least restrictive mode for which this entry should be added
			 *	@return int the new entry_ID
			 */
			QAction* insertMenuEntry(Position parent_id,   const String& name,             const QObject* receiver = 0, 
													     const char* slot = 0, const String& description = "", QKeySequence accel = QKeySequence(),
															 UIOperationMode::OperationMode minimal_mode = UIOperationMode::MODE_ALL);

			/** Insert a new menu entry into menu <b>ID</b> 
			 *	(creates a new menu if <b>ID</b> not existent).
			 *	See the documentation of the qt library for more information concerning menu creation.
			 *	@param ID the menu ID to which the new menu entry should be inserted
			 *	@param name the name of the new menu entry
			 *	@param receiver the object to which the menu action will be connected
			 *	@param slot the function that will be called by activation of the menu entry
			 *	@param description a unique descriptive string for the action
			 *	@param accel the acceleration key
			 *  @param minimal_mode the least restrictive mode for which this entry should be added
			 *	@return int the new entry_ID
			 */
			QAction* insertMenuEntry(Position parent_id,   const QString& name,             const QObject* receiver = 0,
													     const char* slot = 0, const QString& description = "", QKeySequence accel = QKeySequence(),
															 UIOperationMode::OperationMode minimal_mode = UIOperationMode::MODE_ALL);
			
			///
			virtual QMenu* addMenu(const QString& title, UIOperationMode::OperationMode mode = UIOperationMode::MODE_ALL);

			/// 
			void removeMenuEntry (Index parent_id, QAction* action);
			
			/**	Initialize a new popup menu <b> ID</b>. 
					If the MainControl has already the popup menu <b>ID</b> that QPopupMenu is returned.
					See the documentation of the qt library for more information concerning the class QPopupMenu.
					\param    ID the ID of the menu entry to be created.
					\return   QPopupMenu* a pointer to the created QPopupMenu
					\see      PopUpID
			*/	
			virtual QMenu* initPopupMenu(int ID, UIOperationMode::OperationMode mode = UIOperationMode::MODE_ALL);

			/** Insert a separator into the popup menu <b> ID</b>. 
					If the menu <b>ID</b> is not existent, it will be created first.
					\param ID the id of the menu to which a separator will be inserted
					\see   PopUpID
			*/
			void insertPopupMenuSeparator(int ID, UIOperationMode::OperationMode mode = UIOperationMode::MODE_ALL);

			/// Set a hint for a menu entry
			void setMenuHint(QAction* id, const String& hint);

			/// Get the hint for a menu entry
			String getMenuHint(QAction* id) const;

			/** Enable the delete entry for GenericControls.
					Called by a GenericControl, if it has a selection, that can be deleted.
			*/
			void setDeleteEntryEnabled(bool state);
	
			/** Insert the delete entry for GenericControls.
					Called by all GenericControls.
			*/
			void insertDeleteEntry();

			/// Get the ID of the last highlighted menu entry (used for the HelpViewer)
			QAction* getLastHighLightedMenuEntry() { return last_highlighted_menu_entry_;}
				
			//@}
			/**	@name	Methods for multithreading
			*/
			//@{

			/** Check wheter the stored composites can be modified at the moment.
					This method returns true e.g. while a MD simulation is running.
			*/
			bool compositesAreLocked() const;

			/** Lock the Composites for a given Modular Widget.
					This allows exclusive acces e.g. to delete or modify Composites and prevents those
					nasty segfaults if an other thread works on the Composites.
					@retrun true if the exclusive lock on the composites could be obtained
			*/
			bool lockCompositesFor(ModularWidget* widget);

			/// Lock the Composites for a given Modular Widget
			bool unlockCompositesFor(ModularWidget* widget);

			/// Get the ModularWidget with excluse access to the Composites
			ModularWidget* getLockingWidget();

			/// Return true if Representations are (re)calculated
			bool updateOfRepresentationRunning();
					
			/// Returns true, if the simulation was told to stop, but hasnt done this so far.
			bool stopedSimulation() { return stop_simulation_;}

			/** Set the simulation thread.
			 		The instance of SimulationThread will be deleted after it
					has finished. If an other simulation is still running, this
					method returns false.
			*/
			bool setSimulationThread(SimulationThread* thread);

			/** Get the currently running SimulationThread or
			 		zero pointer if no simulation running.
			*/
			SimulationThread* getSimulationThread();

			/** Method to query if multithreading is enabled.
			 		Multithreaded code is used for serveral functions:
			    - Update of Representations
					- Simulations
					- Download PDB files
					<br><br>
					To debug such code it is often usefull to to be able to run it in
					a singlethreaded mode. Every piece of multithreaded code should
					therefore call this method and decide if it should run without multiple threads.
					Furthermore most of the time, valid benchmark results can only be achived 
					with one single thread.
			*/
			bool useMultithreading();

			/// See above
			void setMultithreading(bool state)
				{multi_threading_mode_ = state;}

			///
			bool isBusy() const;

			/// Wait until the MainControl is not busy anymore
			void wait();

			/// Added overloaded method from QApplication for access in Python
			void processEvents(Size ms);

	
			//@}
			/**	@name	Accessors and Settings
			*/
			//@{
			
			/** Sets the text in the statusbar.
					The statusbar has a label, whose text is set to the given argument.
					It is possible to notify the user with a beep sound.
					@param important If true, the message is also logged in the LogView, marked red in the statusbar and
								 shown there for a longer time
					@param beep if true a beep tone is played to inform the user about a critical event
			*/
			void setStatusbarText(const String& text, bool important = false, bool beep = false);

			/// @copydoc MainControl::setStatusbarText
			void setStatusbarText(const QString& text, bool important = false, bool beep = false);

			///
			String getStatusbarText() const;
	
			/// Get a const reference for the fragment database
			const FragmentDB& getFragmentDB() const
				{ return fragment_db_;}

			///
			const ModelInformation& getModelInformation() const;

			///
			void setModelInformation(ModelInformation* mi);

			/** BALLView stores the directory of the last file operation, e.g. when a PDB file is opened or saved.
			 		This saves the user some time, since he doesnt have to change the folders in the file dialogs as often.
					This method returns the last directory.
			*/
			String getWorkingDir() const
				{ return working_dir_;}

			/// Set the working directory for the next file dialog and file operation to the given directory.
			void setWorkingDir(const String& dir);

			/** This enables logging to an file for all messages send per LogStream .e.g. Log().error()
			*/
			void enableLoggingToFile();
			
			/** This disables logging to an file for all messages send per LogStream .e.g. Log().error()
			*/	
			void disableLoggingToFile();

			/** Set the name for the logging file (see above) to the given name. 
					This file is stored in the users home dir.
			*/
			void setLoggingFilename(const String& string);

			/// See above
			const String& getLoggingFilename() const;

			///
			bool isAboutToQuit() { return about_to_quit_;}
		

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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
					
			/** Open a file.
			 		This method is called to parse any command line arguments.
			 		It iterates over all ModularWidgets and calls ModularWidget::canHandle
					with the files extension (the suffix after the dot in the filename).
					If one ModularWidget can handle the format, ModularWidget::openFile
					is called.
			*/
			virtual void openFile(const String& file) ;


			/** Create the standard popup menu.
			 */
			virtual QMenu* createPopupMenu();

			//@}
			/** @name Public slots
			*/
			//@{
			
			public slots:

			/**	Initialize all registered ModularWidget objects.
					It initializes the menu structure, the preferences dialogs and connects
					every ModularWidget with the MainControl.
					This method also creates the first menu entry <b> FILE </b> with its subentry
					<b> EXIT </b> to exit the application.
					See ModularWidget for further information concerning menu structure creation
					and preferences handling. \par
					Calls registerConnectionObject() \par
					Calls fetchPreferences() \par
					Calls applyPreferences() \par
					Calls insertMenuEntry() \par
					Calls ModularWidget::initializeWidget() \par
					Calls QMainWindow::show() \par
					Note: Call this method to start the application.
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

			/// Stop a currently running calculation
			void stopSimulation();
	
			///
			void complementSelection();

			/** Clear Selection
			 		Deselect all Composites and clear the selection list in the MainControl
			*/
			void clearSelection();

			/** Last second cleanup.
					This method will be called internally if the MainControl is about to be destroyed.
					This method stores the preferences and finalizes all ModularWidget objects
					and the MainControl.
					Must be called after your own cleanup routine if you override this method.\par
					Calls ModularWidget::finalizePreferencesTab \par
					Calls ModularWidget::finalizeWidget \par
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
			void menuItemHighlighted(QAction* action);
			
			/// Interface to QT events, e.g. to communicate with other threads
			virtual bool event(QEvent* e);

			/// Make the program exit
			virtual void quit(int return_value = 0);
		
			/// overloaded from QT for Python Interface
			virtual void resize(int w, int h );

			///
			void setContentSize(int w, int h);

			///
			void toggleFullScreen();

			/// Create a BALLView project file with the name quick.bvp in the users home dir
			void quickSave();

			/// Quickload quick.bvp in the users home die (see above)
			void quickLoad();
			
			///
			void saveBALLViewProjectFile(); 

			///
			void loadBALLViewProjectFile();

			///
			void quickLoadConfirm();

			//@}
			
			protected slots:

			/*_ This slot is called internally whenever the apply button
					of the Preferences dialog	is pressed.
					It calls among other things the method applyPreferences().
			*/
			virtual void applyPreferencesClicked_();
			
			/*_ This slot is called internally whenever the ok button
					of the Preferences dialog	is pressed.
					It calls among other things the method applyPreferences().
			*/
			virtual void okPreferencesClicked_();

			//_ Called by timer to clear the text in the statusbar
			void clearStatusBarText_();

			// Connected to the delete entry
			virtual void deleteClicked();

			void updateRepLabel_();

			protected:

			virtual void initializePreferencesTab_();

			//_  Called after receiving an SimulationThreadFinished event
			void stopedSimulation_();

			///
			void lockComposites_();

			/*_ Remove a composite.
					Every Representation, which was created for the Composite is deleted, by sending a 
					RepresentationMessage with type RepresentationMessage::REMOVE.\par
					Redraws representations of the parent of the Composite, if wished.
					\return bool <tt>true</tt> if the CompositeManager has the Composite
			*/
			bool remove_(Composite& composite, bool update_representations_of_parent = true, 
																				 bool to_delete = true);

			/*_ Select the composite parents of the geometric objects.
					The GeometricObjectSelectionMessage is sent by the Scene.
			 */
			void selectComposites_(GeometricObjectSelectionMessage& message);

			void reduceSelection_(Composite* const composite);

			//_ Called by constructors
			void setup_();

			void complementSelectionHelper_(Composite& c);

			/** Show a busy cursor and a busy icon in the statusbar.
			*/
			void setBusyMode_(bool state);

			//_
			void setPreferencesEnabled_(bool state);

			void init_();

			virtual void closeEvent(QCloseEvent* evt);

			bool 								about_to_quit_;
			bool								multi_threading_mode_;	

			//_
			FragmentDB 									fragment_db_;

			ModelInformation* 					model_information_;

			/*_ List with the selected composites
			*/
			HashSet<Composite*> 				selection_;

			/*_ List with the selected composites of the control.
					(Not the one with the checkboxes!)
			*/
			std::list<Composite*>						control_selection_;

			/*_ Message label in the statusbar
					\see setStatusbarText
			*/
			QLabel* 										message_label_;

			RepresentationManager 			primitive_manager_;
			CompositeManager 						composite_manager_;	
			ShortcutRegistry            shortcut_registry_;

			MainControlPreferences* 		main_control_preferences_;
			OpenSavePreferences*				open_save_preferences_;
			NetworkPreferences* 				network_preferences_;
			WebEnginePreferences*				webengine_preferences_;
			Preferences*								preferences_dialog_;
			INIFile		 									preferences_file_;
			
			bool 												composites_locked_;
			ModularWidget*							locking_widget_;
			bool 											  stop_simulation_;

			SimulationThread* 					simulation_thread_;

			/*_	A list containing all modular widgets.
					This list is modified by addModularWidget and
					removeModularWidget.
			*/
			std::list<ModularWidget*>				modular_widgets_;
		
			QLabel*             simulation_icon_;
			QLabel*             rep_label_;
			static const char  *simulation_running_xpm_[];
			static const char  *simulation_stoped_xpm_[];
			Position 					  rep_label_nr_;

			String 							working_dir_;

			String 							logging_file_name_;
			bool 								logging_to_file_;
			File 								logging_file_;

			bool 								important_text_in_statusbar_;
			bool 								was_not_busy_;
			Index 							rep_label_delta_;
			QTimer 							timer_;
			QTimer 							render_timer_;
			Mutex 							composites_locked_mutex_;

			QAction* stop_simulation_action_;
			QAction* fullscreen_action_;
			QAction* complement_selection_action_;
			QAction* clear_selection_action_;
			QAction* open_action_;
			QAction* preferences_action_;
			QAction* delete_action_;

			QAction* last_highlighted_menu_entry_;
			HashMap<Position, QMenu*> id_to_menu_;
			QPoint 				last_point_;
			QSize 				last_size_;
			QByteArray 		last_state_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/mainControl.iC>
#		endif 
    
		}	// namespace VIEW
	} // namespace BALL

#endif // BALL_VIEW_KERNEL_MAINCONTROL_H
