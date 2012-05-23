// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_MODULARWIDGET_H
#define BALL_VIEW_WIDGETS_MODULARWIDGET_H

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
# include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_UIOPERATIONMODE_H
# include <BALL/VIEW/KERNEL/UIOperationMode.h>
#endif

#include <QtGui/QKeySequence>
#include <QtWidgets/QToolBar>

class QObject;
class QMenuBar;
class QWidget;
class QAction;

namespace BALL
{
	class INIFile;
	class FragmentDB;

	namespace VIEW
	{
		class Preferences;
		class MainControl;

		/**	ModularWidget is a base class for widely independent widget that realize one feature domain.
				The VIEW framework provides a simple mechanism for Rapid Application Prototyping based 
				on <b>modular widgets</b>.	Using these modular widgets and the MainControl class
				(or more precisely, classes derived from MainControl) applications can be developed
				in a plug&play style.	
				Inserting an instance of one of these widgets automatically creates the required
				menus and menu entries in the menubar of the main window, sets up the required connections,
				and registers the class instance for usage from the embedded Python interpreter 
				(if desired).
				<br>
				When implementing classes derived from ModularWidget and access 
				to the class from Python is desired, please remember to include the BALL_EMBEDDABLE 
				macro in the public section	of your class declaration. Also make sure that the QT 
				class you	derive from (e.g. QWidget) is the <b>first</b> base class and 
				ModularWidget	second. 
				<br>
				<br>
				<b>Remember:</b> A ModularWidget is not notified by the Messages it sends itself!
				\see MainControl
				\see Embeddable
				\see PyWidget
			\ingroup ViewKernelConnectivity
		*/
		class BALL_VIEW_EXPORT ModularWidget
			: public Embeddable,	
				public ConnectionObject
		{
		  public:
			
			BALL_EMBEDDABLE(ModularWidget,Embeddable)
			BALL_CREATE(ModularWidget)
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Set the name of this ModularWidget to <b>name</b>.
					\param      name the name of this modularWidget
			*/
			ModularWidget(const char* name = "<ModularWidget>");
				
			/**	Copy constructor.
			*/
			ModularWidget(const ModularWidget& widget);
				
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			 		If the ModularWidget was registered, it will call MainControl::removeModularWidget.
			*/
			virtual ~ModularWidget();

			///
			virtual void destroy()
				{ clear(); };

			/** Explicit default initialization.
			 		Currently does nothing.
			*/
			virtual void clear();

			//@}	
			/**	@name	Registering 
			*/
			//@{
			
			/**	Register the widget <b>mwidget</b> to the MainControl. ModularWidget
					objects must always be created with MainControl as parent and must have
					this method in their constructors. This method connects them to the
					MainControl object.
					\param  mwidget the ModularWidget to be registered to the MainControl
			*/
			static void registerWidget(ModularWidget* mwidget)
				throw(Exception::NullPointer);
				
			/**	Initialize the widget.
					This method is called automatically	immediately before the main application 
					is started. It should add the widget's menu entries and connections (if required).
					This method will be called by MainControl::show.
					\param main_control the MainControl object to be initialized with this ModularWidget
					\see   finalizeWidget()
			*/
			virtual void initializeWidget(MainControl& main_control);
			
			/**	Remove the widget custom items, e.g all menu entries.
					This method should reverse all actions performed in initializeWidget
					(remove menu entries and connections of this ModularWidget).
					Call this method also in derived classes finalizeWidget to remove the menu entries.
					This method will be called by MainControl::aboutToExit().
					\param main_control the MainControl object to be finalized with this ModularWidget
					\see   initializeWidget
			*/
			virtual void finalizeWidget(MainControl& main_control);

			/** Test if this ModularWidget can handle a given file format
			 		@param fileform short string with the file extension (e.g. PDB)
					@see openFile
					@see MainControl::openFile
			*/
			virtual bool canHandle(const String& /*fileformat*/) const { return false;}

			/** Tell this ModularWidget to open a given file.
			 		@see canHandle
					@see MainControl::openFile
			*/
			virtual bool openFile(const String& /*filename*/) { return false;}

			//@}	
			/**	@name	Management of menu and toolbar entries
			*/
			//@{
	
			/**	Menu checking method.
			 *	This method is called MainControl::checkMenus before a popup menu is shown.
			 *	It should be used to update the state of menu entries (e.g. disable or enable entries).
			 *	@param main_control the MainControl object whose menus should be checked
			*/
			virtual void checkMenu(MainControl& main_control);

			///
			QAction* insertMenuEntry(Position parent_id,   const QString& name,            const QObject* receiver = 0, 
													     const char* slot = 0, const String& description = "", QKeySequence accel = QKeySequence(),
															 const QString& menu_hint = QString(""), 
															 UIOperationMode::OperationMode minimal_mode = UIOperationMode::MODE_ALL);

			/** Set the help URL for the given action
			 *	@see insertMenuEntry
			 *	@see HelpViewer
			 *  @param action the action to process (it is safe to pass a null pointer)
			 *  @param url the url to add
			 */
			void setMenuHelp(QAction* action, const String& url);

			/** Register an QObject for the help system.
			 		@see HelpViewer
			*/
			virtual void registerForHelpSystem(const QObject* object, const String& url);

			/** Add the widgets actions to the (main) toolbar.
			 		This method is needed to enable ordering the entries.
					It is called in Mainframe.
			*/
			virtual void addToolBarEntries(QToolBar* main_tb);

			/** Set the icon for the given QAction.
			 *	The file is searched in BALL/data/graphics.
			 *  @param action the action to process (it is safe to pass a null pointer)
			 *  @param filename the name of the icon file
			 *  @param add_to_main_toolbar add the icon to the main toolbar
			 */
			void setIcon(QAction* action, const String& filename, bool add_to_main_toolbar = true);


			//@}	
			/**	@name	Preferences handling
			*/
			//@{
			
			/** Initialize a preferences tab for the widget (if needed).
					This method can be used to create preferences widgets that can be inserted
					into the Preferences dialog with the method insertTab.
					This method is called automatically by MainControl::show at the start of the application.
					\param preferences the Preferences dialog of the MainControl
			*/
			virtual void initializePreferencesTab(Preferences& preferences);
			
			/**	Remove the preferences tab.
					This method can remove a preferences widget (if created in 
					initializePreferencesTab)	from the Preferences dialog of the MainControl.
					This method is called automatically by MainControl::aboutToExit() at the end of the application.
					\param  preferences the Preferences dialog of the MainControl
			*/
			virtual void finalizePreferencesTab(Preferences& preferences);
			
			/** Apply the preferences of the specific tab.
					In this method the widget can extract any changed values from
					its preferences tab (if required).
					This method is called automatically by the applyPreferencesTab from the
					MainControl object if the apply button in the Preferences dialog
					is pressed.
					\param  preferences the Preferences dialog of the MainControl
					\see    initializePreferencesTab
					\see    finalizePreferencesTab
					\see    applyPreferencesTab
			*/
			virtual void applyPreferences(){};

			/** Fetch the widgets preferences from the INIFile.
					This method is called automatically by MainControl::show() at the start of the application.
					\param  inifile the INIFile that contains the needed values
					\see    writePreferences
			*/
			virtual void fetchPreferences(INIFile& inifile);
			
			/** Writes the widgets preferences to the INIFile.
					This method is called automatically by MainControl::aboutToExit at the end of the application.
					\param  inifile the INIFile to contain the values
					\see    fetchPreferences
			*/
			virtual void writePreferences(INIFile& inifile);

			//@}	
			/**	@name	Preferences handling
			*/
			//@{
			
			/** Try to get an exclusive lock on the Composites, so that they can not be altered by
			 		any other ModularWidget.
			*/
			bool lockComposites();

			/// Unlock the Composites.
			bool unlockComposites();

			//@}	
			/**	@name	Convenience methods
			*/
			//@{

			/** Return the MainControl of this ModularWidget
					Implemented for convenience.
			*/
			MainControl* getMainControl() const;

			/** Set the text of the statusbar of the main application.
			 		<b>Note:</b> The ModularWidget must be registered to a MainControl.
					Implemented for convenience.
			 */
			virtual void setStatusbarText(const String& text, bool important = false);

			/// @copydoc ModularWidget::setStatusbarText
			virtual void setStatusbarText(const QString& text, bool important = false);

			///	Implemented for convenience.
			String getWorkingDir();

			///	Implemented for convenience.
			void setWorkingDir(const String& dir);

			/** Return the FragmentDB.
			 		<b>Note:</b> The ModularWidget must be registered to a MainControl.
					Implemented for convenience.
			*/
			FragmentDB& getFragmentDB() const;

			//@}
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** Internal state dump.
					Dump the current internal state of this mainControl to 
					the output ostream <b>s</b> with dumping depth <b>depth</b>.
					Calls ConnectionObject::dump.
					\param   s output stream where to output the internal state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}

			void setWorkingDirFromFilename_(String filename);

			virtual void showHelp(const String& url);

			protected:

			//_ id in the menubar entry "WINDOWS" for every widget
			QAction* window_menu_entry_;

			//_ should there be an entry to switch the window on and off?
			bool show_window_enty_;

			//_ should the widget be visible, if no config file entry exists?
			bool default_visible_;

			QList<QAction*> main_toolbar_actions_;
		}; 
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_MODULARWIDGET_H
