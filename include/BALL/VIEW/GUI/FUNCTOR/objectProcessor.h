// $Id: objectProcessor.h,v 1.3 2000/09/23 14:15:03 hekl Exp $

#ifndef BALL_VIEW_GUI_FUNCTOR_OBJECTPROCESSOR_H
#define BALL_VIEW_GUI_FUNCTOR_OBJECTPROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#include <vector>


namespace BALL
{
	
	namespace VIEW
	{

		class ObjectProcessor
		{
			public:

			BALL_CREATE_DEEP(ObjectProcessor)

			/**	@name	 Constructors and Destructors	
			*/
			//@{

			ObjectProcessor();

			ObjectProcessor(const ObjectProcessor &o, bool deep = true);

			virtual ~ObjectProcessor();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Accessors
			*/
			//@{
			void setValue(int address, int value);
		
			int getValue(int address);

			bool hasValue(int address, int value);

			virtual void applyOn(Composite &c);

			virtual void calculateCenter(Composite &c);

			void setViewCenter(const Vector3 &vector3);

			Vector3 getViewCenter() const;
	
			void setViewDirection(int view_direction);

			// muss noch verbessert werden (VIEW_DIRECTION)
			int getViewDirection() const;
					
			void setViewDistance(Real view_distance);

			Real getViewDistance() const;
			//@}

			/**	@name	Operators
			*/
			//@{
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

			Vector3 view_center_vector_;
			int view_direction_;
			Real view_distance_;
			vector<int> address_array_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/FUNCTOR/objectProcessor.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_FUNCTOR_OBJECTPROCESSOR_H
