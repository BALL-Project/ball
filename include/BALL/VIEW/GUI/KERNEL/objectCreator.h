// $Id: objectCreator.h,v 1.3 2000/09/23 14:15:05 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#define BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H       
# include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCET_TEXTPERSISTENCEMANAGER_H
# include <BALL/CONCEPT/textPersistenceManager.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

#include <vector>


namespace BALL
{
	
	namespace VIEW
	{

		class ObjectCreator
		{
			public:

			BALL_CREATE_DEEP(ObjectCreator)

			/**	@name	 Constructors and Destructors	
			*/
			//@{

			ObjectCreator();

			ObjectCreator(const ObjectCreator& objectCreator, bool deep = true);

			virtual ~ObjectCreator();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Accessors
			*/
			//@{
			void setValue(int address, int value);
		
			int getValue(int address);

			bool hasValue(int address, int value);

			virtual void initPersistenceManager(TextPersistenceManager &pm);

			virtual Composite *convertObject(PersistentObject &po);
			//@}

			/**	@name	Operators
			*/
			//@{
			Composite *operator() (IOStreamSocket &iostream_socket);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/	
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			
			protected:

			private:
			
			bool init_;
			TextPersistenceManager pm_;
			vector<int> address_array_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/objectCreator.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
