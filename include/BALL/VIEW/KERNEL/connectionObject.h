// $Id:

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#define BALL_VIEW_KERNEL_CONNECTIONOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#include <qobject.h>

#include <string>
#include <typeinfo>

using std::string;
using namespace BALL;

namespace BALL
{

	namespace VIEW
	{
		/**
		*/
		class ConnectionObject
		{
			public:
			
			BALL_CREATE_DEEP(ConnectionObject)
			
			/**	@name	Enums
			*/
			//@{
			//@}

			/**	@name	Constructors and Destructors
			*/	
			//@{

			ConnectionObject();
				
			ConnectionObject(const ConnectionObject& object, bool deep = true);

			virtual ~ConnectionObject();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			void registerConnectionObject(ConnectionObject &object);

			void unregisterConnectionObject(ConnectionObject &object);

			bool isConnectionObjectRegistered(const ConnectionObject &object);


			ConnectionObject *getParent() const;

			ConnectionObject *getRoot();


			virtual Options *getOptions();

			virtual void onNotify(Message *message);
			//@}
		
			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			
			protected:

			void notify_(Message *message);

			void notify_(Message &message);


			private:

			void setParent_(ConnectionObject &object);
			void clearParent_();

			void onNotify_(Message* message);


			bool processing_message_queue_;
			List<Message *> message_queue_;

			ConnectionObject *parent_;
			List<ConnectionObject *> children_connection_objects_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/connectionObject.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
