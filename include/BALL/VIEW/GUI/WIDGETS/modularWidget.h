// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.h,v 1.16 2003/03/14 11:50:19 sturm Exp $

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#define BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
# include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
#	include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
# include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

namespace BALL
{
	namespace VIEW
	{
    /**  \addtogroup  ViewGuiWidgets
     *  @{
     */
		/**	The ModularWidget class.
				The base class for all modular widgets.
				BALLVIEW provides a simple mechanism for Rapid Application Prototyping based 
				on {\em modular widgets}.	Using the modular widgets and the  \link MainControl MainControl \endlink  class
				(or more precisely, classes derived from MainControl) applications can be developed
				in a plug&play style.	The most important of a visualization application are	
				implemented as classes derived from ModularWidget.
				Inserting an instance of one of these widgets automatically creates the required
				menus in the menubar of the main window, sets up the required connections,
				and registers the class instance for use from the embedded Python interpreter 
				(if desired). \par
				When implementing classes derived from ModularWidget and you want to give access 
				to the class (from Python), please remember	to include the  \link BALL_EMBEDDABLE BALL_EMBEDDABLE \endlink  
				macro in the public section	of your class declaration. Also make sure that the QT 
				class you	derive from (e.g. QWidget) is the {\em first} base class and 
				ModularWidget	second.  \par
				
				@see MainControl
				@see Embeddable
				@see PyInterpreter
				@see PyWidget
		*/
		class ModularWidget
			: public Embeddable,	
				public ConnectionObject
		{
		  public:
			
			BALL_EMBEDDABLE(ModularWidget)			
			BALL_CREATE(ModularWidget)
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new modularWidget.
					Set the name of {\em *this} modularWidget to {\em name}.
					@param      name the name of {\em *this} modularWidget
					@return     ModularWidget new constructed modularWidget
					@see        Embeddable
					@see        ConnectionObject
			*/
			ModularWidget(const char* name = "<ModularWidget>")
				throw();
				
			/**	Copy constructor.
					Construct new modularWidget by copying the modularWidget {\em widget}.
					The state of {\em *this} modularWidget is initialized to the state of the
					modularWidget {\em widget}. \par
					@param       widget the modularWidget to be copied
					@return      ModularWidget new constructed modularWidget copied from {\em widget}
					@see         Embeddable
					@see         ConnectionObject
			*/
			ModularWidget(const ModularWidget& widget)
				throw();
				
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} modularWidget.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~ModularWidget()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			
			//@}	
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/**	Register the widget.
					Register the widget {\em mwidget} to the  \link MainControl MainControl \endlink . Modular widgets
					objects must always be created with  \link MainControl MainControl \endlink  as parent and must have
					this method in their constructors. This method connects {\em mwidget} to the
					 \link MainControl MainControl \endlink  object.
					@param  mwidget the modularWidget to be registered to the  \link MainControl MainControl \endlink 
			*/
			static void registerWidget(ModularWidget* mwidget)
				throw();
				
			/**	Initialize the widget.
					This method is called automatically	immediately before the main application 
					is started. It should add the widget's menu entries and connections (if required).
					This method will be called by  \link show show \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be initialized with {\em *this} modularWidget
					@see   finalizeWidget
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control);
			//	throw();
			
			/**	Remove the widget.
					This method should reverse all actions performed in  \link initializeWidget initializeWidget \endlink 
					(remove menu entries and connections of {\em *this} modularWidget).
					This method will be called by  \link aboutToExit aboutToExit \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be finalized with {\em *this} modularWidget
					@see   initializeWidget
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control);
			//	throw();
			
			/**	Menu checking method.
					This method is called by the method  \link checkMenus checkMenus \endlink  from the
					 \link MainControl MainControl \endlink  object before a popup menu is shown.
					It should be used to update the state of menu entries (e.g. disable or enable entries).
					@param main_control the  \link MainControl MainControl \endlink  object whose menus should be checked
					@see   initializeWidget
					@see   finalizeWidget
					@see   show
					@see   checkMenus
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			/** Initialize a preferences tab.
					Initialize a preferences tab for the widget (if needed).
					This method can be used to create preferences widgets that can be inserted
					into the  \link Preferences Preferences \endlink  dialog with the method  \link insertTab insertTab \endlink .
					This method is called automatically by the method  \link show show \endlink  from the 
					 \link MainControl MainControl \endlink  object at the start of the application.
					@param preferences the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink 
					@see   finalizePreferencesTab
					@see   show
					@see   Preferences
			*/
			virtual void initializePreferencesTab(Preferences& preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method can remove a preferences widget (if created in 
					 \link initializePreferencesTab initializePreferencesTab \endlink )	from the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink .
					This method is called automatically by the method  \link aboutToExit aboutToExit \endlink  from the
					 \link MainControl MainControl \endlink  object at the end of the application.
					@param  preferences the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink 
					@see    initializePreferencesTab
					@see    aboutToExit
					@see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences& preferences)
				throw();
			
			/** Apply the preferences of the specific tab.
					In this method the widget can extract any changed values from
					its preferences tab (if required).
					This method is called automatically by the  \link applyPreferencesTab applyPreferencesTab \endlink  from the
					 \link MainControl MainControl \endlink  object if the apply button in the  \link Preferences Preferences \endlink  dialog
					is pressed.
					@param  preferences the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink 
					@see    initializePreferencesTab
					@see    finalizePreferencesTab
					@see    MainControl
					@see    applyPreferencesTab
					@see    Preferences
			*/
			virtual void applyPreferences(Preferences& preferences)
				throw();
			
			/** Fetch the widgets preferences from the inifile.
					This method extracts the default values from the given  \link INIFile INIFile \endlink 
					{\em inifile} (as written to the inifile in the  \link fetchPreferences fetchPreferences \endlink  method).
					This method is called automatically by the method  \link show show \endlink  from the 
					 \link MainControl MainControl \endlink  object at the start of the application.
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed values
					@see    writePreferences
					@see    initializePreferencesTab
					@see    finalizePreferencesTab
					@see    INIFile
					@see    MainControl
					@see    show
			*/
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			/** Writes the widgets preferences to the inifile.
					This method should write all needed values to the given inifile (as read from
					the inifile in the  \link fetchPreferences fetchPreferences \endlink  method).
					This method is called automatically by the method  \link aboutToExit aboutToExit \endlink  from the 
					 \link MainControl MainControl \endlink  object at the end of the application.
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed values
					@see    fetchPreferences
					@see    initializePreferencesTab
					@see    finalizePreferencesTab
					@see    INIFile
					@see    MainControl
					@see    aboutToExit
			*/
			virtual void writePreferences(INIFile& inifile)
				throw();

			/** Set the text of the statusbar.
			 */
			virtual void setStatusbarText(String text)
				throw();
			//@}			
		}; 
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
