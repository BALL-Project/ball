// $Id: atomBondModelConnector.h,v 1.1 2000/06/25 19:13:04 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif

using namespace BALL;
using namespace BALL::VIEW;

 
namespace BALL
{

	namespace MOLVIEW
	{
		
		/**
		*/
		class AtomBondModelConnector
			: public BaseModelConnector
		{
			public:
			
			/**	@name	Type definitions
			*/
			//@{
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			AtomBondModelConnector();

			AtomBondModelConnector
				(const AtomBondModelConnector& connector, bool deep = true);

			virtual ~AtomBondModelConnector();

			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			class MissingProperty
				:	public Exception::GeneralException
			{
				public:

				MissingProperty(const char* file, int line, const string &message);
			};

			class MemoryAllocationFailed
				:	public Exception::GeneralException
			{
				public:

				MemoryAllocationFailed(const char* file, int line, const string &message);
			};
			//@}

			/**	@name	Asignment
			*/
			//@{
			//@}

			
			/**	@name	Accessors
			*/
			//@{
			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			//@}

			protected:

			Property getModel_(Atom& atom);
			

  		private:

			void createLineRepresentation_(Bond& bond);
			void createTubeRepresentation_(Bond& bond);

			virtual Line*           createLine_();
			virtual TwoColoredLine* createTwoColoredLine_();
			virtual Tube*           createTube_();
			virtual TwoColoredTube* createTwoColoredTube_();
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H
