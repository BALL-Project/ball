// $Id: geometricConvertor.h,v 1.1 2000/06/04 17:56:10 hekl Exp $

#ifndef BALL_MOLVIEW_KERNEL_GEOMETRICCONVERTOR_H
#define BALL_MOLVIEW_KERNEL_GEOMETRICCONVERTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif
        
#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#	include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
#	include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;


namespace BALL
{

	namespace MOLVIEW
	{

		/**
		 */
		class GeometricConvertor
			: public ConnectionObject
		{
		public:
			
			/**	@name	Enums
			 */
			//@{
			//@}
			
			/**	@name	Type Definitions
			 */
			
			/**	@name	Constructors and Destructors
			 */
			//@{
			
			GeometricConvertor();
			
			virtual ~GeometricConvertor();
			//@}
			
			/**	@name Exceptions
			 */
			//@{
			//@}
			
			/**	@name	Accessors
			 */
			//@{
			void onNotify(Message *message);
			//@}
			
			
		private:
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/geometricConvertor.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_GEOMETRICCONVERTOR_H_
