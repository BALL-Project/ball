// $Id: server.h,v 1.4 2000/11/12 15:31:11 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_SERVER_H
#define BALL_VIEW_GUI_KERNEL_SERVER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_QTTIMER_H
#	include <BALL/VIEW/KERNEL/QTTimer.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#	include <BALL/VIEW/GUI/KERNEL/objectCreator.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
 #include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
 #include <BALL/VIEW/GUI/WIDGETS/serverPreferences.h>
#endif

#include <qlabel.h>

namespace BALL
{
	
	namespace VIEW
	{

		class Server
			: public QTTimer,
   			public ModularWidget
		{
			public:

			/**	@name	 Constructors and Destructors	
			*/
			//@{

			/** default constructor. creates an instance of server.
			*/
			Server(QWidget* parent = 0, const char* name = 0);

			/** default destructor. destroys an instance of server.
			*/
			virtual ~Server();

			/** clear methode. resets the timer used to control the server
			*/
			virtual void clear();
			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			/** Composite Exception. returned if the creation of a received object failed.
			*/
			class CompositePointerIsNull
				:	public Exception::GeneralException
			{
				public:

				CompositePointerIsNull(const char* file, int line);
			};
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Activates the server. Must be called before other methods!
			*/
			void activate();

			/** Deactivates the server. Must be called before other methods!
			*/
			void deactivate();

			/**	Set the server port.
			*/
			void setPort(const int);

			/**	Return the server port.
			*/
			int getPort() const;

			/** returns the composite received from client
			*/
			const Composite* getReceivedComposite() const;

			/** registers a ObjectCreator that is used for converting presistent objects
					into composites.
			*/
			void registerObjectCreator(const ObjectCreator& s);

			/** resets the object creator. After calling this methode object will be converted
					using default object creator.
			*/
			void deregisterObjectCreator();

			/** handels notification requests
			*/
			//			virtual void onNotify(Message *message);

			/**	Initialize the server widget.
					This method is called automatically
					immediately before the main application 
					is started. It adds the widget's 
					menu entries, connections and icons.
			*/
			virtual void initializeWidget(MainControl& main_control);
			
			/**	Remove the server widget.
					This method is called before the widget's destructor is called.
					It reverses all actions performed in 
					initializeWidget (remove menu entries and connections and icons).
			*/
			virtual void finalizeWidget(MainControl& main_control);
			
			/**	Update all menu entry states.
					This method is called just before a popup menu
					is shown (via the QT signal aboutToShow()).
					It should be used to update the state of 
					menu entries (e.g. disable or enable entries).
			*/
			virtual void checkMenu(MainControl& main_control);
			
			/** Initialize a preferences tab for the server.
					This method is called automatically
					immediately before the main application 
					is started. It adds the widget's preferences tabs.
			*/
			virtual void initializePreferencesTab(Preferences &preferences);
			
			/**	Remove the preferences tab.
					This method is called before the widget's destructor is called.
					It reverses all actions performed in 
					initializePreferencesTab (remove tabs).
			*/
			virtual void finalizePreferencesTab(Preferences &preferences);
			
			/** Apply the preferences of the specific tab.
					In this method the widget can extract any changed values from
					its preferences tab (if required).
					This method is called automatically if the apply button in the
					preferences dialog is pressed.
			*/
			virtual void applyPreferences(Preferences &preferences);
			
			/** Fetch the server preferences from the inifile.
					This method extracts the default values from the given
					inifile.
					This method is called automatically
					immediately before the main application 
					is started. It fetches the widget's initial values from the inifile. 
			*/
			virtual void fetchPreferences(INIFile &inifile);
			
			/** Writes the server preferences to the inifile.
					This method is called before the widget's destructor is called.
					It writes all needed values to the given inifile (as read from
					the inifile in the fetchPreferences method).
			*/
			virtual void writePreferences(INIFile &inifile);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** returns true if the underlying timer is valid.
		  */
			virtual bool isValid() const;

			/** dumps the timer states.
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Not yet implemented.
			*/
			virtual void read(std::istream& s);

			/** Not yet implemented.
			*/
			virtual void write(std::ostream& s) const;
			//@}

			
			protected:

			/** overridden timer call.
			*/
			virtual void timer();


			private:

			/** private methodes used for reacting to client requests.
			*/
			void sendObject(IOStreamSocket& iostream_socket);

			void setCreatorValue(IOStreamSocket& iostream_socket);
			void getCreatorValue(IOStreamSocket& iostream_socket);
			void hasCreatorValue(IOStreamSocket& iostream_socket);


			/** private storage variables.
			*/
			ObjectCreator *object_creator_;

			Composite* received_composite_;

			typedef HashMap<unsigned long, Composite *> CompositeHashMap;

			CompositeHashMap composite_hashmap_;

			IOStreamSocket*	iostream_socket_;
			SockInetBuf*		sock_inet_buf_;
				
			// the port to bind to
			int							port_; 

			ServerPreferences  *server_preferences_;
			QLabel 					   *server_icon_;
			static const char  *mini_ray_xpm_[];
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/server.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_SERVER_H
