// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.h,v 1.10 2004/02/02 17:12:50 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_MODULARWIDGET_H
#define BALL_VIEW_WIDGETS_MODULARWIDGET_H

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
# include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

class QObject;
class QMenuBar;

namespace BALL
{
	class INIFile;
	class FragmentDB;

	namespace VIEW
	{
		class Preferences;
		class MainControl;

		/**	ModularWidget is a base class.
				BALLVIEW provides a simple mechanism for Rapid Application Prototyping based 
				on <b>modular widgets</b>.	Using the modular widgets and the MainControl class
				(or more precisely, classes derived from MainControl) applications can be developed
				in a plug&play style.	The most important parts of a visualization application are	
				implemented as classes derived from ModularWidget.
				Inserting an instance of one of these widgets automatically creates the required
				menus in the menubar of the main window, sets up the required connections,
				and registers the class instance for use from the embedded Python interpreter 
				(if desired).\par
				When implementing classes derived from ModularWidget and you want to give access 
				to the class from Python, please remember	to include the BALL_EMBEDDABLE 
				macro in the public section	of your class declaration. Also make sure that the QT 
				class you	derive from (e.g. QWidget) is the <b>first</b> base class and 
				ModularWidget	second. \par
				<b>Remember:</b> A ModularWidget is not notified by the Messages it sends itself!
				\see MainControl
				\see Embeddable
				\see PyInterpreter
				\see PyWidget
			\ingroup ViewKernelConnectivity
		*/
		class ModularWidget
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
			ModularWidget(const char* name = "<ModularWidget>")
				throw();
				
			/**	Copy constructor.
			*/
			ModularWidget(const ModularWidget& widget)
				throw();
				
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ModularWidget()
				throw();

			///
			virtual void destroy()
				throw() {};

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			//@}	
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/**	Register the widget <b>mwidget</b> to the MainControl. ModularWidget
					objects must always be created with MainControl as parent and must have
					this method in their constructors. This method connects them to the
					MainControl object.
					\param  mwidget the ModularWidget to be registered to the MainControl
			*/
			static void registerWidget(ModularWidget* mwidget)
				throw();
				
			/**	Initialize the widget.
					This method is called automatically	immediately before the main application 
					is started. It should add the widget's menu entries and connections (if required).
					This method will be called by MainControl::show.
					\param main_control the MainControl object to be initialized with this ModularWidget
					\see   finalizeWidget()
			*/
			virtual void initializeWidget(MainControl& main_control);
			
			/**	Remove the widget.
					This method should reverse all actions performed in initializeWidget
					(remove menu entries and connections of this ModularWidget).
					This method will be called by MainControl::aboutToExit().
					\param main_control the MainControl object to be finalized with this ModularWidget
					\see   initializeWidget
			*/
			virtual void finalizeWidget(MainControl& main_control);
			
			/**	Menu checking method.
					This method is called MainControl::checkMenus before a popup menu is shown.
					It should be used to update the state of menu entries (e.g. disable or enable entries).
					\param main_control the MainControl object whose menus should be checked
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			/** Initialize a preferences tab for the widget (if needed).
					This method can be used to create preferences widgets that can be inserted
					into the Preferences dialog with the method insertTab.
					This method is called automatically by MainControl::show at the start of the application.
					\param preferences the Preferences dialog of the MainControl
			*/
			virtual void initializePreferencesTab(Preferences& preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method can remove a preferences widget (if created in 
					initializePreferencesTab)	from the Preferences dialog of the MainControl.
					This method is called automatically by MainControl::aboutToExit() at the end of the application.
					\param  preferences the Preferences dialog of the MainControl
			*/
			virtual void finalizePreferencesTab(Preferences& preferences)
				throw();
			
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
			virtual void applyPreferences(Preferences& preferences)
				throw();

			/// Method is called when the Cancel button of the Preferences is pressed.
			virtual void cancelPreferences(Preferences& /*preferences*/)
				throw() {};

			/** Set default values for the current page in Preferences
					Automatically called by the default button in thre Preferences dialog.)
			*/
			virtual void defaultPreferences(Preferences& /*preferences*/)
				throw() {};
			
			/** Fetch the widgets preferences from the INIFile.
					This method is called automatically by MainControl::show() at the start of the application.
					\param  inifile the INIFile that contains the needed values
					\see    writePreferences
			*/
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			/** Writes the widgets preferences to the INIFile.
					This method is called automatically by MainControl::aboutToExit at the end of the application.
					\param  inifile the INIFile to contain the values
					\see    fetchPreferences
			*/
			virtual void writePreferences(INIFile& inifile)
				throw();

			/** Set the text of the statusbar of the main application.
			 		<b>Note:</b> The ModularWidget must be registered to a MainControl.
					Implemented for convenience.
			 */
			virtual void setStatusbarText(String text)
				throw();

			/** Return the MainControl of this ModularWidget
					Implemented for convenience.
			*/
			MainControl* getMainControl() const
				throw();

			/** Return the FragmentDB.
			 		<b>Note:</b> The ModularWidget must be registered to a MainControl.
					Implemented for convenience.
			*/
			FragmentDB& getFragmentDB() const
				throw();

			/// Wrapper for MainControl::insertMenuEntry
			int insertMenuEntry(int ID, const String& name, const QObject* receiver, const char* slot, 
																 int accel, int entry_ID, String hint)
				throw();

			/// Wrapper for MainControl::menuBar()
			QMenuBar* menuBar() 
				throw();

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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
					
			//@}

			protected:
			
			//_ id in the menubar entry "WINDOWS" for every widget
			Index window_menu_entry_id_;

			//_ should there be an entry to switch the window on and off?
			bool show_window_enty_;

			//_ should the widget be visible, if no config file entry exists?
			bool default_visible_;
			//@}			
		}; 
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_MODULARWIDGET_H
